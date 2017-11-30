#pragma once 
#include <vector>
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <cereal\types\string.hpp>

class Mesh
{
public:
	
	typedef glm::fvec3 vec3;
	typedef glm::fvec2 vec2;
	typedef glm::mat4 mat4;
public:
	Mesh();
	~Mesh();

	bool LoadFromFile(const std::string &file);
	void Unload();

	unsigned int GetNumFaces() const;
	unsigned int GetNumVertices() const;


	bool SetInstancing(vec3* dataBuffer, size_t bufferSize);
	bool SetInstancing(mat4* dataBuffer, size_t bufferSize);


	inline void BufferSubData(GLuint targetBuffer, GLintptr offset, GLsizeiptr size, const void* data)
	{
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, targetBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	void ExposeToEditor();

	template<typename Archive>
	void serialize(Archive & arc)
	{
		arc(Filepath);
		LoadFromFile(Filepath);
	}

	GLuint VBO_Verticies = 0;
	GLuint VBO_UVs		 = 0;
	GLuint VBO_Normals	 = 0;
	GLuint VBO_Instance  = 0;
	GLuint VAO			 = 0;

	bool IsInstanced = false;
	unsigned int InstanceNumber = 0;
	std::string Filepath;

private:
	unsigned int _NumFaces = 0;
	unsigned int _NumVertices = 0;

};