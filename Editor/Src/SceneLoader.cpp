#include "SceneLoader.h"
#include <iostream>
#include <fstream>
#include <ComponentReflection.h>


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
		offset += strlen(name);
	}
	//--------------------------------------------------------

	for (int i = 0; i < numEntities; ++i)
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

	unsigned int numEntities = a_scene.GetNumEntities();
	unsigned int numComponents = 0;

	//Write number of entities
	File.write((char*)&numEntities, sizeof(unsigned int));

	//Write number of Registered components
	File.write((char*)&numComponents, sizeof(unsigned int));

	//Write the name of each Registered component
	for (int i = 0; i < numComponents; ++i)
	{
		File.write((char*)a_scene.GetNumEntities(), sizeof(unsigned int));
	}

	//For each entity, search for it's components and add their index & data
	for (int i = 0; i < numEntities; ++i)
	{
		//Get Number of Components registered to the entity
		
	}
}
