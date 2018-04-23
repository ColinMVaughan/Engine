#ifndef GL_RENDERER_MATERIAL_H
#define GL_RENDERER_MATERIAL_H

#include "Texture.h"
#include <glm\mat4x4.hpp>
#include <iostream>
#include<cereal\types\string.hpp>
#include <fstream>

//-----------------------------
// Material represents the look and feel of an object.
// use a combination of texture maps to achieve this.
//----------------------------
struct Material
{
	Material()
	:AlbedoScale(1.0, 1.0, 1.0),
		MetallicScale(1.0),
		RoughnessScale(1.0){}
	
	Material(std::string a_matFile)
		:AlbedoScale(1.0, 1.0, 1.0),
		MetallicScale(1.0),
		RoughnessScale(1.0)
	{
		
		std::string* names[5];
		names[0] = &albedoName;
		names[1] = &normalName;
		names[2] = &aoName;
		names[3] = &metallicName;
		names[4] = &roughnessName;

		std::ifstream file;
		file.open(a_matFile);

		for(int i=0; i< 5; ++i)
		{
			char buff[256] = "";
			file.getline(buff, 256);

			*names[i] = buff;
		}


	}

	~Material() {}


	void SetTexturesFromFolder(std::string FolderPath);
	static bool LoadDefaultTexture(std::string Filepath)
	{
		if (!defaultTex.Load(Filepath))
		{
			std::cout << "\nCould not load default Material";
			return false;
		}

		return true;
	}

	void ExposeToEditor();


	template<typename Archive>
	void save(Archive& arc) const
	{
		arc(m_folderPath);
	}

	template<typename Archive>
	void load(Archive& arc)
	{
		arc(m_folderPath);
		SetTexturesFromFolder(m_folderPath);
	}

	//Colour
	Texture Albedo;
	glm::vec3 AlbedoScale;
	std::string albedoName;

	Texture Normal;
	std::string normalName;

	Texture AO;
	std::string aoName;

	Texture Metallic;
	float MetallicScale = 1.0f;
	std::string metallicName;

	Texture Roughness;
	float RoughnessScale = 1.0f;
	std::string roughnessName;

	Texture Emissive;

	std::string m_folderPath;

private:

	static Texture defaultTex;
};


#endif