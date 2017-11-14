#include "SceneLoader.h"
#include <iostream>
#include <fstream>

void LoadSceneFromFile(std::string filePath, ECS::Scene & a_scene)
{
	char* fileData; //pointer to buffer that will hold binary data
	uint32_t offset = 0;

	//HEADER DATA (In order of appearence)
	uint32_t numComponents;
	std::vector<std::string> componentNames;
	uint32_t numEntities;

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


}

void SaveSceneToFile(ECS::Scene & a_scene)
{
}
