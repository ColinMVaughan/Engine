#define _CRT_SECURE_NO_WARNINGS
#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <imgui.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define CHAR_BUFFER_SIZE 128
#define BUFFER_OFFSET(i) ((char *)0 + (i))




void ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<glm::vec3> VertexPos;
	std::vector<glm::vec3> VertexNorm;
	std::vector<glm::vec2> VertexUV;

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		glm::vec3 tempPos;
		tempPos.x = mesh->mVertices[i].x;
		tempPos.y = mesh->mVertices[i].y;
		tempPos.z = mesh->mVertices[i].z;
		VertexPos.push_back(tempPos);

		glm::vec3 tempNorm;
		tempNorm.x = mesh->mVertices[i].x;
		tempNorm.y = mesh->mVertices[i].y;
		tempNorm.z = mesh->mVertices[i].z;
		VertexNorm.push_back(tempNorm);

		glm::vec2 tempUV;
		tempUV.x = mesh->mVertices[i].x;
		tempUV.y = mesh->mVertices[i].y;
		VertexUV.push_back(tempUV);


	}

	std::cout << "DONE";
}


void ProcessNode(aiNode* node, const aiScene *scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}


void loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ASSIMP ERROR" << importer.GetErrorString() << "\n";
		return;
	}

	std::string directory = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
}











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

	//Unique data
	std::vector<vec3> vertexData;
	std::vector<vec2> textureData;
	std::vector<vec3> normalData;
	std::vector<vec3> weights;
	std::vector<glm::ivec3> boneIndecies;

	//face indecies
	std::vector<unsigned int> indecies; 
	


	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file, 0 /*aiProcess_Triangulate | aiProcess_FlipUVs*/);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ASSIMP ERROR" << importer.GetErrorString() << "\n";
		return false;
	}


	aiMesh* mesh = scene->mMeshes[0];

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		glm::vec3 tempPos;
		tempPos.x = mesh->mVertices[i].x;
		tempPos.y = mesh->mVertices[i].y;
		tempPos.z = mesh->mVertices[i].z;
		vertexData.push_back(tempPos);

		glm::vec3 tempNorm;
		tempNorm.x = mesh->mNormals[i].x;
		tempNorm.y = mesh->mNormals[i].y;
		tempNorm.z = mesh->mNormals[i].z;
		normalData.push_back(tempNorm);

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 tempUV;
			tempUV.x = mesh->mTextureCoords[0][i].x;
			tempUV.y = mesh->mTextureCoords[0][i].y;
			textureData.push_back(tempUV);
		}
		else
		{
			glm::vec2 tempUV;
			tempUV.x = 0.0f;
			tempUV.y = 0.0f;
			textureData.push_back(tempUV);
		}

		//reserve bone weights that we will fill with values later
		if (mesh->HasBones())
		{
			boneIndecies.push_back(glm::ivec3(0, 0, 0));
			weights.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}

	//loop th
	//for (unsigned int i = 0; i < mesh->mNumBones(); ++i)
	//{
	//	for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights(); ++j)
	//	{
	//		unsigned int index = mesh->mBones[i]->mWeights[j].mVertexId;

	//		if (boneIndecies[index].x == 0)
	//		{
	//			boneIndecies[index].x = i;
	//			weights[index].x = mesh->mBones[i]->mWeights[j].mWeight;
	//		}
	//		else if (boneIndecies[index].y == 0)
	//		{
	//			boneIndecies[index].y = i;
	//			weights[index].y = mesh->mBones[i]->mWeights[j].mWeight;
	//		}
	//		else
	//		{
	//			boneIndecies[index].z = i;
	//			weights[index].z = mesh->mBones[i]->mWeights[j].mWeight;
	//		}
	//			
	//		
	//	}
	//	
	//}
	

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indecies.push_back(face.mIndices[j]);
		}
	}


	_NumFaces = indecies.size();
	_NumVertices = _NumFaces * 3;

	//Send Data to GPU//

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_Verticies);
	glGenBuffers(1, &VBO_UVs);
	glGenBuffers(1, &VBO_Normals);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0); //Vertex
	glEnableVertexAttribArray(1); //UVs
	glEnableVertexAttribArray(2); //Normals

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Verticies);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(float)* 3, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_UVs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)* textureData.size(), &textureData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, sizeof(float)* 2, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* normalData.size(), &normalData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(float)* 3, BUFFER_OFFSET(0));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indecies.size() * sizeof(unsigned int), &indecies[0], GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Cleanup

	glBindVertexArray(0);
	
	vertexData.clear();
	textureData.clear();
	normalData.clear();

	Filepath = file;
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

	if(VBO_Instance == 0)
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
	char buff[128] = "./Assets/Models/";
	if (ImGui::InputText("Mesh File Path", buff, 128, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		LoadFromFile(std::string(buff));
	}
}




