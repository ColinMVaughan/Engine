#pragma once 
#include <vector>
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
class Mesh
{
public:
	
	typedef glm::fvec3 vec3;
	typedef glm::fvec2 vec2;
public:
	Mesh();
	~Mesh();

	bool LoadFromFile(const std::string &file);
	void Unload();

	unsigned int GetNumFaces() const;
	unsigned int GetNumVertices() const;


	bool SetInstancing(float* dataBuffer, size_t bufferSize)
	{
		//if the vertex attrib object has not been created, 
		//we can not generate our instance buffer
		//if (VAO == 0)
		//	return false;

		//glGenBuffers(1, &VBO_Instance);
		//glBindVertexArray(VAO);

		//glEnableVertexAttribArray(3);
		//glBindBuffer(GL_ARRAY_BUFFER, VBO_Instance);

	}

	GLuint VBO_Verticies = 0;
	GLuint VBO_UVs		 = 0;
	GLuint VBO_Normals	 = 0;
	GLuint VBO_Instance  = 0;
	GLuint VAO			 = 0;

private:
	unsigned int _NumFaces = 0;
	unsigned int _NumVertices = 0;

};