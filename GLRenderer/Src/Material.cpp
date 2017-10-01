#include "Material.h"
#include <iostream>


void Material::SetTexturesFromFolder(std::string FolderPath)
{
	Albedo.Load(FolderPath + "/Albedo.png", GL_NEAREST);
	Normal.Load(FolderPath + "/Normal.png", GL_NEAREST);
	AO.Load(FolderPath + "/AO.png", GL_NEAREST);
	Metallic.Load(FolderPath + "/Metallic.png", GL_NEAREST);
	Roughness.Load(FolderPath + "/Roughness.png", GL_NEAREST);
}