#ifndef GL_RENDERER_MATERIAL_H
#define GL_RENDERER_MATERIAL_H

#include "Texture.h"
#include <glm\mat4x4.hpp>
#include <iostream>
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
	void serialize(Archive & arc)
	{
		arc(m_folderPath);
		SetTexturesFromFolder(m_folderPath);
	}

	//Colour
	Texture Albedo;
	glm::vec3 AlbedoScale;

	Texture Normal;
	Texture AO;

	Texture Metallic;
	float MetallicScale = 1.0f;

	Texture Roughness;
	float RoughnessScale = 1.0f;

	std::string m_folderPath;

private:

	static Texture defaultTex;
};


#endif