#define _CRT_SECURE_NO_WARNINGS
#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <imgui.h>

#define CHAR_BUFFER_SIZE 128
#define BUFFER_OFFSET(i) ((char *)0 + (i))

struct MeshFace
{
	MeshFace()
	{
		verticies[0] = 0;
		verticies[1] = 0;
		verticies[2] = 0;

		textureUVs[0] = 0;
		textureUVs[1] = 0;
		textureUVs[2] = 0;

		normals[0] = 0;
		normals[1] = 0;
		normals[2] = 0;
	}

	MeshFace(unsigned v1, unsigned v2, unsigned v3,
		unsigned t1, unsigned t2, unsigned t3,
		unsigned n1, unsigned n2, unsigned n3)
	{
		verticies[0] = v1;
		verticies[1] = v2;
		verticies[2] = v3;

		textureUVs[0] = t1;
		textureUVs[1] = t2;
		textureUVs[2] = t3;

		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;
	}

	unsigned verticies[3];
	unsigned textureUVs[3];
	unsigned normals[3];

};

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

bool Mesh::LoadFromFile(const std::string &file)
{
	std::ifstream input;
	input.open(file);

	if (!input)
	{
		std::cout << "Could Not Open File." << std::endl;
		return false;
	}

	char inputString[CHAR_BUFFER_SIZE];

	//Unique data
	std::vector<vec3> vertexData;
	std::vector<vec2> textureData;
	std::vector<vec3> normalData;
	//Index / Face data
	std::vector<MeshFace> faceData;
	//OpenGL Ready
	std::vector<float> unPackedVertexData;
	std::vector<float> unPackedTextureData;
	std::vector<float> unPackedNormalData;

	while (!input.eof())
	{
		input.getline(inputString, CHAR_BUFFER_SIZE);

		if (std::strstr(inputString, "#") != nullptr)
		{
			//this line is a comment 
			continue;
		}
		else if (std::strstr(inputString, "vt") != nullptr)
		{
			vec2 temp;
			std::sscanf(inputString, "vt %f %f", &temp[0], &temp[1]);
			textureData.push_back(temp);
		}
		else if (std::strstr(inputString, "vn") != nullptr)
		{
			vec3 temp;
			std::sscanf(inputString, "vn %f %f %f", &temp[0], &temp[1], &temp[2]);
			normalData.push_back(temp);
		}
		else if (std::strstr(inputString, "v") != nullptr)
		{
			vec3 temp;
			std::sscanf(inputString, "v %f %f %f", &temp[0], &temp[1], &temp[2]);
			vertexData.push_back(temp);
		}
		else if (std::strstr(inputString, "f") != nullptr)
		{
			MeshFace temp;

			std::sscanf(inputString, "f %u/%u/%u %u/%u/%u %u/%u/%u",
				&temp.verticies[0], &temp.textureUVs[0], &temp.normals[0],
				&temp.verticies[1], &temp.textureUVs[1], &temp.normals[1],
				&temp.verticies[2], &temp.textureUVs[2], &temp.normals[2]);

			faceData.push_back(temp);
		}

	}

	for (unsigned i = 0; i < faceData.size(); i++)
	{
		for (unsigned j = 0; j < 3; j++)
		{
			unPackedVertexData.push_back(vertexData[faceData[i].verticies[j] - 1][0]);
			unPackedVertexData.push_back(vertexData[faceData[i].verticies[j] - 1][1]);
			unPackedVertexData.push_back(vertexData[faceData[i].verticies[j] - 1][2]);

			unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1][0]);
			unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1][1]);

			unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1][0]);
			unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1][1]);
			unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1][2]);
		}
	}


	_NumFaces = faceData.size();
	_NumVertices = _NumFaces * 3;

	//Send Data to GPU//

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_Verticies);
	glGenBuffers(1, &VBO_UVs);
	glGenBuffers(1, &VBO_Normals);

	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0); //Vertex
	glEnableVertexAttribArray(1); //UVs
	glEnableVertexAttribArray(2); //Normals

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Verticies);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* unPackedVertexData.size(), &unPackedVertexData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(float)* 3, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_UVs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* unPackedTextureData.size(), &unPackedTextureData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, sizeof(float)* 2, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* unPackedNormalData.size(), &unPackedNormalData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(float)* 3, BUFFER_OFFSET(0));

	//Cleanup
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	vertexData.clear();
	textureData.clear();
	normalData.clear();
	faceData.clear();
	unPackedVertexData.clear();
	unPackedTextureData.clear();
	unPackedNormalData.clear();

	return true;
}

void Mesh::Unload()
{
	glDeleteBuffers(1, &VBO_Normals);
	glDeleteBuffers(1, &VBO_UVs);
	glDeleteBuffers(1, &VBO_Verticies);
	//glDeleteBuffers(1, &VBO_Instance);
	glDeleteVertexArrays(1, &VAO);

	VBO_Normals		= 0;
	VBO_UVs			= 0;
	VBO_Verticies	= 0;
	VAO				= 0;

	_NumFaces		= 0;
	_NumVertices	= 0;
}

unsigned int Mesh::GetNumFaces() const
{
	return _NumFaces;
}

unsigned int Mesh::GetNumVertices() const
{
	return _NumVertices;
}

bool Mesh::SetInstancing(vec3* dataBuffer, size_t bufferSize)
{
	//if the vertex attrib object has not been created, 
	//we can not generate our instance buffer
	if (VAO == 0)
		return false;

	glGenBuffers(1, &VBO_Instance);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Instance);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)* bufferSize, &dataBuffer[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), BUFFER_OFFSET(0));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glVertexAttribDivisor(3, 1);
	glBindVertexArray(0);

	IsInstanced = true;
	InstanceNumber = bufferSize;
	return true;
}

bool Mesh::SetInstancing(mat4* dataBuffer, size_t bufferSize)
{
	if (VAO == 0)
		return false;

	GLsizei vec4Size = sizeof(glm::vec4);

	glGenBuffers(1, &VBO_Instance);
	glBindVertexArray(VAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO_Instance);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mat4)* bufferSize, &dataBuffer[0][0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer((GLuint)3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer((GLuint)4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, BUFFER_OFFSET(vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer((GLuint)5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, BUFFER_OFFSET(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer((GLuint)6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, BUFFER_OFFSET(3 * vec4Size));
	

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

	IsInstanced = true;
	InstanceNumber = bufferSize;
	return true;
}

void Mesh::ExposeToEditor()
{
	char buff[128] = "";
	if (ImGui::InputText("Mesh File Path", buff, 128, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		LoadFromFile(std::string(buff));
	}
}