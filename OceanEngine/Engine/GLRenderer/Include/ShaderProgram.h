#pragma once
#include "GL\glew.h"
#include <string>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>

class ShaderProgram
{

public:
	typedef glm::fvec2 vec2;
	typedef glm::fvec3 vec3;
	typedef glm::fvec4 vec4;

public:
	ShaderProgram();
	~ShaderProgram();

	//loads a vertex shader and fragment shader, and places them in a program
	bool Load(const std::string &vertFile, const std::string &fragFile);
	bool Load(const std::string &vertFile, const std::string &fragFile, const std::string &geomFile);
	bool IsLoaded() const;

	//clear all data from OpenGL
	void UnLoad();
	bool LinkProgram();

	//use / detach the shader from use
	void Bind() const;
	void UnBind();

	//Requires a re-link before OpenGL will register the change
	void AddAttribute(unsigned int index, const std::string &attribName);

	//Returns 1 if the attribute does not exist
	int GetAttribLocation(const std::string &attribName);
	//Returns 1 if the uniform does not exist
	int GetUniformLocation(const std::string &uniformName);

	//Send data to shaders
	void SendUniform(const std::string &name, int integer);
	void SendUniform(const std::string &name, unsigned int unsigned_integer);
	void SendUniform(const std::string &name, float scalar);
	void SendUniform(const std::string &name, const vec2 &vector);
	void SendUniform(const std::string &name, const vec3 &vector);
	void SendUniform(const std::string &name, const vec4 &vector);
	void SendUniformArray(const std::string &name, const vec4 *vector, unsigned size);
	void SendUniformArray(const std::string &name, const vec3 *vector, unsigned size);
	void SendUniformArray(const std::string &name, const vec2 *vector, unsigned size);


	void SendUniformMat3(const std::string &name, float *matrix, bool transpose);
	void SendUniformMat4(const std::string &name, float *matrix,bool transpose);
	void SendUniformMat4(const std::string &name, const float *matrix, bool transpose);

private:
	bool _IsInit			= false;
	GLuint _VertexShader	= 0;
	GLuint _FragShader		= 0;
	GLuint _GeomShader		= 0;
	GLuint _Program			= 0;

	std::string ReadFile(const std::string &fileName) const;
	bool CompileShader(GLuint shader) const;
	void OutputShaderLog(GLuint shader) const;
	void OutputProgramLog() const;
	
};