#include "ECS.h"
#include <iostream>
#include <fstream>
#include <ComponentReflection.h>
#include <Detail.h>


//----------------------------------------
//Converts data buffer into a stream for the 
//----------------------------------------
struct serialBuf : std::streambuf
{
	serialBuf(char* begin, char* end)
	{
		this->setg(begin, begin, end);
	}
};




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
	for (int i = 1; i < numEntities; ++i)
	{
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

			//Serialize Component Data if Applicable
			ECS::SerializeComponent(componentNames[components[num]], this, GetEntity(i));
		}


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



	for (int i = 1; i < numEntities; ++i)
	{
		//create new entity
		ECS::Entity entity = CreateEntity();

		//Get the number of components registered to the entity
		uint32_t registeredComps = *reinterpret_cast<uint32_t*>(fileData + offset);
		offset += 4;

		//Loop through and get the name of its registered components based on an index value
		for (int c = 0; c < registeredComps; ++c)
		{
			//Get the index refering to a registered component
			uint32_t componentIndex = *reinterpret_cast<uint32_t*>(fileData + offset);
			offset += 4;
			//Add Component to entity based on its name
			ECS::AddComponentFromString(componentNames[componentIndex], this, entity);


			//Get the length of the data from the component.
			//bool componentIsSerializable = *reinterpret_cast<bool*>(fileData + offset);
			//offset += 4;
			//If the length is greater than zero: serialize the component.
			if (true)
			{
				//create an istream from the pointer to the component binary data
				serialBuf data(&fileData[offset], &fileData[offset + size]);
				std::istream stream(&data);

				//Pass the stream to the serialization archive
				cereal::BinaryInputArchive archive(stream);
				m_BinaryInput = &archive;

				//Set the component data from serialization data
				ECS::UnSerializeComponent(componentNames[componentIndex], this, entity);
			}

		}
		//End Component Loop
	}
	//End Entity Loop

	m_BinaryInput = nullptr;
	File.close();
}