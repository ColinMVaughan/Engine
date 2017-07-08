#pragma once 
#include <vector>
#include <GL\glew.h>
#include <GMath/MathLibrary.h>

class Mesh
{
public:
	typedef GMath::vec3f vec3;
	typedef GMath::vec2f vec2;
public:
	Mesh();
	~Mesh();

	bool LoadFromFile(const std::string &file);
	void Unload();

	unsigned int GetNumFaces() const;
	unsigned int GetNumVertices() const;

	GLuint VBO_Verticies = 0;
	GLuint VBO_UVs		 = 0;
	GLuint VBO_Normals	 = 0;
	GLuint VAO			 = 0;

private:
	unsigned int _NumFaces = 0;
	unsigned int _NumVertices = 0;

};