#include "SceneLoader.h"
#include <iostream>
#include <fstream>
#include <ComponentReflection.h>
#include <Detail.h>


//--------------------------------------------------------
// Loads scene data from a file, Scene data includes
// the number of entities and their registered components,
// as well as any serializable component's data.
//
//--------------------------------------------------------
void LoadSceneFromFile(std::string filePath, ECS::Scene & a_scene)
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
		return;
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
		ECS::Entity entity = a_scene.CreateEntity();

		//Get the number of components registered to the entity
		uint32_t registeredComps = *reinterpret_cast<uint32_t*>(fileData + offset);
		offset += 4;

		//Loop through and get the name of its registered components based on an index value
		for (int c = 0; c < registeredComps; ++c)
		{
			uint32_t componentIndex = *reinterpret_cast<uint32_t*>(fileData + offset);
			offset += 4;

			//Add Component to entity based on its name
			ECS::AddComponentFromString(componentNames[componentIndex], &a_scene, entity);
			/*ADD SOME SORT OF COMPONENT SERIALIZATION HERE*/
		}

	}

	File.close();
}



//--------------------------------------------------------
// Saves scene to file
//
//--------------------------------------------------------
void SaveSceneToFile(std::string filePath, ECS::Scene & a_scene)
{
	std::ofstream File;
	File.open(filePath, std::ios::out | std::ios::binary);

	auto registry = ECS::detail::GetComponentRegistry(); //component registry
	uint32_t numEntities = a_scene.GetNumEntities(); //number of entities in scene
	uint32_t numComponents = registry.size(); //Number of registered Components

	//Write number of entities
	File.write((char*)&numEntities, sizeof(uint32_t));

	//Write number of Registered components
	File.write((char*)&numComponents, sizeof(uint32_t));

	//iterate through each registered component type
	for (auto it = registry.begin(); it != registry.end(); ++it)
	{
		//Write name of component (it->first == string)
		File.write(it->first.c_str(), it->first.size() + 1);
	}

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
			if (ECS::CheckComponentFromString(it->first, &a_scene, a_scene.GetEntity(i)))
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
			ECS::SerializeComponent();
		}


	}


	File.close();
}
