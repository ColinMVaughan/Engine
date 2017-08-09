#pragma once
#include <GL/glew.h>
#include <string>

class Texture
{
public:
	Texture() = default;
	~Texture();

	bool Load(const std::string &file, GLuint SampleType = GL_LINEAR);
	bool LoadHDR(const std::string &file);
	
	bool CreateCubeMap(GLsizei size, bool mipmap);
	void Unload();

	void Bind();
	void UnBind();

	GLuint TexObj = 0;
};