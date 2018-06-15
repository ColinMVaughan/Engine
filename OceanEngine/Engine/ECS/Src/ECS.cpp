#include "ECS.h"
#include <iostream>
#include <fstream>
#include <ComponentReflection.h>
#include <Detail.h>


unsigned ECS::Entity::GetID()
{
	return m_ID;
}



//----------------------------------------
//Converts data buffer into a stream for the 
//----------------------------------------
struct serialBuf : public std::streambuf
{
	serialBuf(char* begin, char* end)
	{
		this->setg(begin, begin, end);
	}

	serialBuf()
	{

	}

	size_t Size() const
	{
		return m_Size;
	}

private:
	int_type overflow(int_type C)
	{
		return m_Size++;
	}

private:
	size_t m_Size = 0;
};


EventManager& ECS::Scene::GetEventManager()
{
	return *m_EventManager;
}

//--------------------------------------------------------------------
// Saves the scene to a file
//
// DATA SAVED TO FILE:
// Number of entities + their components, components registered 
//
//--------------------------------------------------------------------
bool ECS::Scene::SaveScene(std::string filePath)
{
	std::ofstream File;
	File.open(filePath, std::ios::out | std::ios::binary);

	auto registry = ECS::detail::GetComponentRegistry(); //component registry
	uint32_t numEntities = GetNumEntities(); //number of entities in scene
	uint32_t numComponents = registry.size(); //Number of registered Components
	std::vector<std::string> componentNames;

	//Write number of entities
	File.write((char*)&numEntities, sizeof(uint32_t));

	//Write number of Registered components
	File.write((char*)&numComponents, sizeof(uint32_t));

	//iterate through each registered component type
	for (auto it = registry.begin(); it != registry.end(); ++it)
	{
		componentNames.push_back(it->first);
		//Write name of component (it->first == string)
		File.write(it->first.c_str(), it->first.size() + 1);
	}

	//Create serialization
	cereal::BinaryOutputArchive archive(File);
	m_BinaryOutput = &archive;

	//For each entity, search for it's components and add their index & data
	for (int i = 0; i < numEntities; ++i)
	{
		//Write the name of the entity to file
		std::string entityName = GetEntity(i).GetName();
		File.write(entityName.c_str(), entityName.length()+1);

		uint32_t compIndex = 0;
		std::vector<uint32_t> components;

		//Get Number of Components registered to the entity
		//--------------------------------------------------------------------------------
		for (auto it = registry.begin(); it != registry.end(); ++it)
		{
			//If the ectity has the component type, add it's index to the vector
			if (ECS::CheckComponentFromString(it->first, this, GetEntity(i)))
				components.push_back(compIndex);

			compIndex++;
		}
		//Write number of components attached to entity
		uint32_t numcomp = components.size();
		File.write((char*)&numcomp, sizeof(uint32_t));
		//---------------------------------------------------------------------------------

		//Write the index of the component and add the serialized component data if applicable
		for (int num = 0; num < components.size(); ++num)
		{
			//Write component index
			File.write((char*)&components[num], sizeof(uint32_t));

			//Create a new archive to test the size of the serialized component
			serialBuf Buffer;
			std::basic_ostream<char> sizeStream(&Buffer);
			cereal::BinaryOutputArchive sizeTest(sizeStream);

			//Test the size of the component and write it to the file.
			//The reason we do this instead of sizeof(component), is because we can't garentee
			//that all the component's member data will be serialized.
			m_BinaryOutput = &sizeTest;
			ECS::SerializeComponent(componentNames[components[num]], this, GetEntity(i));
			size_t dataSize = Buffer.Size();
			File.write((char*)&dataSize, sizeof(size_t));

			//Serialize Component Data if Applicable
			m_BinaryOutput = &archive;
			ECS::SerializeComponent(componentNames[components[num]], this, GetEntity(i));

		}


	}

	m_BinaryOutput = nullptr;
	File.close();
	return true;
}





bool ECS::Scene::SaveEntityPrefab(Entity& entity)
{
	std::ofstream File;
	File.open("./Assets/Prefabs/"+entity.GetName()+".prefab", std::ios::out | std::ios::binary);

	auto registry = ECS::detail::GetComponentRegistry(); //component registry
	uint32_t numEntities = GetNumEntities(); //number of entities in scene
	uint32_t numComponents = registry.size(); //Number of registered Components
	std::vector<std::string> componentNames;


	//Write number of Registered components
	File.write((char*)&numComponents, sizeof(uint32_t));

	//iterate through each registered component type
	for (auto it = registry.begin(); it != registry.end(); ++it)
	{
		componentNames.push_back(it->first);
		//Write name of component (it->first == string)
		File.write(it->first.c_str(), it->first.size() + 1);
	}


	//Create serialization
	cereal::BinaryOutputArchive archive(File);
	m_BinaryOutput = &archive;


	//Write the name of the entity to file
	std::string entityName = entity.GetName();
	File.write(entityName.c_str(), entityName.length() + 1);

	uint32_t compIndex = 0;
	std::vector<uint32_t> components;

	//Get Number of Components registered to the entity
	//--------------------------------------------------------------------------------
	for (auto it = registry.begin(); it != registry.end(); ++it)
	{
		//If the ectity has the component type, add it's index to the vector
		if (ECS::CheckComponentFromString(it->first, this, entity))
			components.push_back(compIndex);

		compIndex++;
	}

	//Write number of components attached to entity
	uint32_t numcomp = components.size();
	File.write((char*)&numcomp, sizeof(uint32_t));
	//---------------------------------------------------------------------------------

	//Write the index of the component and add the serialized component data if applicable
	for (int num = 0; num < components.size(); ++num)
	{
		//Write component index
		File.write((char*)&components[num], sizeof(uint32_t));

		//Create a new archive to test the size of the serialized component
		serialBuf Buffer;
		std::basic_ostream<char> sizeStream(&Buffer);
		cereal::BinaryOutputArchive sizeTest(sizeStream);

		//Test the size of the component and write it to the file.
		//The reason we do this instead of sizeof(component), is because we can't garentee
		//that all the component's member data will be serialized.
		m_BinaryOutput = &sizeTest;
		ECS::SerializeComponent(componentNames[components[num]], this, entity);
		size_t dataSize = Buffer.Size();
		File.write((char*)&dataSize, sizeof(size_t));

		//Serialize Component Data if Applicable
		m_BinaryOutput = &archive;
		ECS::SerializeComponent(componentNames[components[num]], this, entity);

	}

	m_BinaryOutput = nullptr;
	File.close();
	return true;
}




//--------------------------------------------------------------------
//
//
//
//--------------------------------------------------------------------
bool ECS::Scene::LoadScene(std::string filePath)
{
	char* fileData; //pointer to buffer that will hold binary data
	uint32_t offset = 0; //offset used to traverse buffer

						 //HEADER DATA (In order of appearence)
	uint32_t numEntities;
	uint32_t numComponents;
	std::vector<std::string> componentNames;


	//File
	std::ifstream File;
	std::streampos size;

	//Open Binary File
	File.open(filePath, std::ios::binary | std::ios::ate);
	if (File.is_open())
	{
		//Get the Size of the file and allocate buffer
		size = File.tellg();
		fileData = new char[size];

		//Read data to buffer
		File.seekg(0, std::ios::beg);
		File.read(fileData, size);
		File.close();
	}
	else
	{
		std::cout << "Coild not open Sceen file";
		return false;
	}

	//Header Data
	//------------------------------------------------------
	//Get the number of entities in the scene
	numEntities = *reinterpret_cast<uint32_t*>(fileData + offset);
	offset += 4; //increase the offset by 4 bytes (size of uint32_t)

	//Get the number of Component Types in the scene
	numComponents = *reinterpret_cast<uint32_t*>(fileData + offset);
	offset += 4;

	//Get the names of all unique component types
	for (int i = 0; i < numComponents; ++i)
	{
		char* name = reinterpret_cast<char*>(fileData + offset);
		componentNames.push_back(std::string(name));
		offset += (strlen(name) + 1);
	}
	//--------------------------------------------------------


	//Start with one to skip over the default user camera, which will have no effect on the game/scene.
	for (int i = 0; i < numEntities; ++i)
	{
		char* entname = reinterpret_cast<char*>(fileData + offset);
		offset += (strlen(entname) +1);

		//create new entity
		ECS::Entity entity = CreateEntity(entname);

		//Get the number of components registered to the entity
		uint32_t registeredComps = *reinterpret_cast<uint32_t*>(fileData + offset);
		offset += 4;

		//Loop through and get the name of its registered components based on an index value
		for (int c = 0; c < registeredComps; ++c)
		{
			//Get the index refering to a registered component
			uint32_t componentIndex = *reinterpret_cast<uint32_t*>(fileData + offset);
			offset += 4;

			//Get the size of the component
			size_t componentSize = *reinterpret_cast<size_t*>(fileData + offset);
			offset += sizeof(size_t);

			//Add Component to entity based on its name
			ECS::AddComponentFromString(componentNames[componentIndex], this, entity);


			//Get the length of the data from the component.
			//bool componentIsSerializable = *reinterpret_cast<bool*>(fileData + offset);
			//offset += 4;
			//If the length is greater than zero: serialize the component.
			if (componentSize > 0)
			{
				//create an istream from the pointer to the component binary data
				serialBuf data(&fileData[offset], &fileData[offset + componentSize]);
				std::istream stream(&data);


				//Pass the stream to the serialization archive
				cereal::BinaryInputArchive archive(stream);
				m_BinaryInput = &archive;

				//Set the component data from serialization data
				ECS::UnSerializeComponent(componentNames[componentIndex], this, entity);

				offset += componentSize;
				std::cout << "Bla\n";
			}

		}
		//End Component Loop
	}
	//End Entity Loop

	m_BinaryInput = nullptr;
	File.close();
}



bool ECS::Scene::LoadAndSpawnPrefabEntity(std::string prefabPath)
{
	char* fileData; //pointer to buffer that will hold binary data
	uint32_t offset = 0; //offset used to traverse buffer

						 //HEADER DATA (In order of appearence)
	uint32_t numEntities;
	uint32_t numComponents;
	std::vector<std::string> componentNames;


	//File
	std::ifstream File;
	std::streampos size;

	//Open Binary File
	File.open(prefabPath, std::ios::binary | std::ios::ate);
	if (File.is_open())
	{
		//Get the Size of the file and allocate buffer
		size = File.tellg();
		fileData = new char[size];

		//Read data to buffer
		File.seekg(0, std::ios::beg);
		File.read(fileData, size);
		File.close();
	}
	else
	{
		std::cout << "Coild not open Sceen file";
		return false;
	}

	//Header Data
	//------------------------------------------------------

	//Get the number of Component Types in the scene
	numComponents = *reinterpret_cast<uint32_t*>(fileData + offset);
	offset += 4;

	//Get the names of all unique component types
	for (int i = 0; i < numComponents; ++i)
	{
		char* name = reinterpret_cast<char*>(fileData + offset);
		componentNames.push_back(std::string(name));
		offset += (strlen(name) + 1);
	}
	//--------------------------------------------------------


	//Begin Loading the Entity
	char* entname = reinterpret_cast<char*>(fileData + offset);
	offset += (strlen(entname) + 1);

	//create new entity
	ECS::Entity entity = CreateEntity(std::string(entname) + std::to_string(EntityCounter));

	//Get the number of components registered to the entity
	uint32_t registeredComps = *reinterpret_cast<uint32_t*>(fileData + offset);
	offset += 4;

	//Loop through and get the name of its registered components based on an index value
	for (int c = 0; c < registeredComps; ++c)
	{
		//Get the index refering to a registered component
		uint32_t componentIndex = *reinterpret_cast<uint32_t*>(fileData + offset);
		offset += 4;

		//Get the size of the component
		size_t componentSize = *reinterpret_cast<size_t*>(fileData + offset);
		offset += sizeof(size_t);

		//Add Component to entity based on its name
		ECS::AddComponentFromString(componentNames[componentIndex], this, entity);


		//Get the length of the data from the component.
		//bool componentIsSerializable = *reinterpret_cast<bool*>(fileData + offset);
		//offset += 4;
		//If the length is greater than zero: serialize the component.
		if (componentSize > 0)
		{
			//create an istream from the pointer to the component binary data
			serialBuf data(&fileData[offset], &fileData[offset + componentSize]);
			std::istream stream(&data);

			//Pass the stream to the serialization archive
			cereal::BinaryInputArchive archive(stream);
			m_BinaryInput = &archive;

			//Set the component data from serialization data
			ECS::UnSerializeComponent(componentNames[componentIndex], this, entity);

			offset += componentSize;
			std::cout << "Bla\n";
		}

	}
	//End Component Loop
	//End Entity Loop

	m_BinaryInput = nullptr;
	File.close();
}


void ECS::Scene::Clear()
{
	m_ComponentManager->ClearAllComponents();
	m_SystemManager->ClearAllRegisteredEntities();

	m_EntityList.clear();
	m_EntityNames.clear();
	EntityCounter = 0;
}

void ECS::Scene::UnloadUserSystems()
{
	m_SystemManager->RemoveUserSystems();
}
