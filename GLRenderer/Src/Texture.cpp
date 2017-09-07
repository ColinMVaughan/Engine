#include "Texture.h"
#include "stb_image.h"


#include <iostream>

Texture::~Texture()
{
	Unload();
}

bool Texture::Load(const std::string &file, GLuint SampleType)
{

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponrnts;
	unsigned char* image = stbi_load(file.c_str(), &width, &height, &nrComponrnts, 0);

	if (image)
	{
		GLenum format;
		if (nrComponrnts == 1)
			format = GL_RED;
		else if (nrComponrnts == 3)
			format = GL_RGB;
		else if (nrComponrnts == 4)
			format = GL_RGBA;
		else
			return false;


		glGenTextures(1, &TexObj);
		glBindTexture(GL_TEXTURE_2D, TexObj);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, SampleType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //U axis
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //V axis


		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}

	return false;
}

bool Texture::LoadHDR(const std::string& file)
{
	//SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float* data = stbi_loadf(file.c_str(), &width, &height, &nrComponents, 0);
	
	if(data)
	{
		glGenTextures(1, &TexObj);
		glBindTexture(GL_TEXTURE_2D, TexObj);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load HDR image." << std::endl;
	}

	return true;
}

bool Texture::CreateCubeMap(GLsizei size, bool mipmap)
{
	glGenTextures(1, &TexObj);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TexObj);
	for(int i=0; i< 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, size, size, 0, GL_RGB, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	if(mipmap)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	else
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(mipmap)
	{
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	return true;
}

void Texture::Unload()
{
	if (TexObj != 0)
	{
		glDeleteTextures(1, &TexObj);
		TexObj = 0;
	}
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, TexObj);
}

void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}