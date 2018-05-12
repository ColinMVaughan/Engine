#include "VoxelRenderSystem.h"
#include <Utilities.h>
#include <iostream>
#include <fstream>
#include <glm\gtc\matrix_transform.hpp>

#define CODEFLAG 2
#define NEXTSLICEFLAG 6

void VoxelContainer::ReadQubicBinaryFile(std::string file)
{
	//char* data;
	//Uint32 offset;

	//Uint32 version;
	//Uint32 colourFormat;
	//Uint32 zAxisOrientation;
	//Uint32 compressed;
	//Uint32 visibilityMaskEncoded;
	//Uint32 numMatricies;

	////Using a pointer to these elements for the sake of readability
	////and so that I can use these in a loop
	//Uint32* HeaderData[6];
	//HeaderData[0] = &version;
	//HeaderData[1] = &colourFormat;
	//HeaderData[2] = &zAxisOrientation;
	//HeaderData[3] = &compressed;
	//HeaderData[4] = &visibilityMaskEncoded;
	//HeaderData[5] = &numMatricies;

	////Open the file and allocate a buffer large enough to accomodate it
	//std::ifstream MyFile;
	//std::streampos size;
	//MyFile.open(file, std::ios::binary|std::ios::ate);
	//if (MyFile.is_open())
	//{
	//	size = MyFile.tellg();
	//	data = new char[size];

	//	MyFile.seekg(0, std::ios::beg);
	//	MyFile.read(data, size);
	//	MyFile.close();
	//}
	//else
	//{
	//	std::cout << "\nCould Not open Qubic Binary File.";
	//	return;
	//}
	////Loop through header data, increasing offset as we collect the data
	//offset = 0;
	//for (int i = 0; i < 6; ++i)
	//{
	//	*HeaderData[i] = *reinterpret_cast<Uint32*>(data + offset);
	//	offset += 4;
	//}

	//std::cout << "\nQB HEADER TEST";
	//std::cout << "\n--------------------";
	//std::cout << "\nVersion: "<<version;
	//std::cout << "\nColour Format: " << colourFormat;
	//std::cout << "\ncompressed: " << zAxisOrientation;
	//std::cout << "\nVisibility Mask Encoded: " << visibilityMaskEncoded;
	//std::cout << "\nNum Matricies: " << numMatricies;
	//std::cout << "\n--------------------";
	////---------------------------------------------------
	////Collect Matrix Data

	////Get the length of the matrix name and skip past it because i dont care
	//Uint8 nameLength = *reinterpret_cast<Uint8*>(data + offset);
	//offset += (nameLength + 1);

	////collect size of matrix
	//const Uint32 matrixSizeX = *reinterpret_cast<Uint32*>(data + offset);
	//offset += 4;
	//const Uint32 matrixSizeY = *reinterpret_cast<Uint32*>(data + offset);
	//offset += 4;
	//const Uint32 matrixSizeZ = *reinterpret_cast<Uint32*>(data + offset);
	//offset += 4;

	////Skipping past the position data of the matrix because I dont care
	//offset += 12;

	//Uint32* Matrix = new Uint32[matrixSizeX * matrixSizeY * matrixSizeZ];
	//for (int z = 0; z < matrixSizeZ; ++z)
	//{
	//	for (int y = 0; y < matrixSizeY; ++y)
	//	{
	//		for (int x = 0; x < matrixSizeX; ++x)
	//		{
	//			Matrix[x + y*matrixSizeX + z*matrixSizeX*matrixSizeY] = *reinterpret_cast<Uint32*>(data + offset);
	//			offset += 4;
	//		}
	//	}
	//}

	//ConstructVoxelMesh(Matrix, nullptr, matrixSizeX, matrixSizeY, matrixSizeZ);
	//delete[] Matrix;
	//delete[] data;
}

//Reads the VOX file format to load in voxel data from magicavoxel
void VoxelContainer::ReadVoxFile(std::string file)
{
	//----------------------------------------------------------------------
	//DATA
	//----------------------------------------------------------------------
	//pointer to binary data and pointer offset
	char* data;
	Uint32 offset = 0;

	//Header Data
	char ID[4];
	int32_t version;

	//--------------------------------------------------------------------
	//READING
	//---------------------------------------------------------------------
	//Open the file and allocate a buffer large enough to accomodate it
	std::ifstream MyFile;
	std::streampos size;
	MyFile.open(file, std::ios::binary | std::ios::ate);
	if (MyFile.is_open())
	{
		size = MyFile.tellg();
		data = new char[size];

		MyFile.seekg(0, std::ios::beg);
		MyFile.read(data, size);
		MyFile.close();
	}
	else
	{
		std::cout << "\nCould Not open Vox File.";
		return;
	}

	//Get Header Data
	//collect size of matrix
	for (int i = 0; i < 3; ++i)
	{
		ID[i] = data[i];
	}
	offset += 4;

	//version number
	offset += 4;

	//Reads the voxel MAIN chunk
	ReadChunk(data, offset);
	//Reads the voxel SIZE chunk
	ReadChunk(data, offset);
	//Reads the voxel XYZI chunk
	ReadChunk(data, offset);
	//Reads the voxel RGBA chunk
	ReadChunk(data, offset);

	delete[] data;
	return;
}

bool VoxelContainer::ReadChunk(char * data, uint32_t & offset)
{
	//Chunk Data
	char chunkID[4];
	int32_t chunkDataSize;
	//uint32_t chunkChildNum;
	
	//Chunk'SIZE'
	int32_t sizeX;
	int32_t sizeY;
	int32_t sizeZ;

	//Chunk 'XYZI'
	int32_t numVoxels;
	std::vector<int8_t> voxels;
	//-----------------------------------------------------------

	//Read chunk id
	for (int i = 0; i < 4; ++i)
	{
		chunkID[i] = data[i + offset];
	}
	offset += 4;

	//read size of the chunk in bytes
	chunkDataSize = *reinterpret_cast<int32_t*>(data + offset);
	offset += 4;

	//read number of child chunks
	unsigned int chunkChildNum = *reinterpret_cast<int32_t*>(data + offset);
	offset += 4;

	//
	switch (chunkID[3])
	{
	case 'N': //MAIN
		return false;
		break;

	case 'K': //PACK
		offset += 4;
		break;

	case 'E': //SIZE
	{
		ReadChunkSIZE(data, offset);
		break;
	}
	case 'I': //XYZI
	{
		ReadChunkXYZI(data, offset);
		break;
	}
	case 'A': //RGBA
	{
		ReadChunkRGBA(data, offset);
		return false;
		break;
	}

	case 'T': //MATT (we want to ignore)
		return false;
		break;

	}


	//for (int i = 0; i < chunkChildNum; ++i)
	//{
	//	ReadChunk(data, offset);
	//}

	return true;
}

bool VoxelContainer::ReadChunkRGBA(char * data, uint32_t & offset)
{
	std::map<unsigned int, std::vector<unsigned int*>> matIndexMap;
	for (int i = 0; i< m_Voxels.size(); ++i)
	{
		matIndexMap[m_Voxels[i].m_MaterialIndex].push_back(&m_Voxels[i].m_MaterialIndex);
	}

	unsigned int matCounter = 0;
	for (auto it = matIndexMap.begin(); it != matIndexMap.end(); ++it)
	{
		//load the appropriate material
		unsigned int materialIndex = it->first * 4 + offset - 4;
		uint8_t r = *reinterpret_cast<int8_t*>(data + materialIndex);
		uint8_t g = *reinterpret_cast<int8_t*>(data + materialIndex + 1);
		uint8_t b = *reinterpret_cast<int8_t*>(data + materialIndex + 2);
		uint8_t a = *reinterpret_cast<int8_t*>(data + materialIndex + 3);
		m_Palette.push_back(glm::vec3(r, g, b));

		//changed the index of each block using this material to the new index
		for (int i = 0; i<it->second.size(); ++i)
		{
			*it->second[i] = matCounter;
		}

		matCounter++;
	}

	return false;
}

bool VoxelContainer::ReadChunkSIZE(char * data, uint32_t & offset)
{
	m_Width= *reinterpret_cast<int32_t*>(data + offset);
	offset += 4;

	m_Height = *reinterpret_cast<int32_t*>(data + offset);
	offset += 4;

	m_Depth = *reinterpret_cast<int32_t*>(data + offset);
	offset += 4;

	return false;
}

bool VoxelContainer::ReadChunkXYZI(char * data, uint32_t & offset)
{
	int32_t numVoxels = *reinterpret_cast<int32_t*>(data + offset);
	offset += 4;
	for (int32_t i = 0; i < numVoxels; ++i)
	{
		int8_t x = *reinterpret_cast<int8_t*>(data + offset);
		offset += 1;

		int8_t y = *reinterpret_cast<int8_t*>(data + offset);
		offset += 1;

		int8_t z = *reinterpret_cast<int8_t*>(data + offset);
		offset += 1;

		unsigned int index = *reinterpret_cast<uint8_t*>(data + offset);
		offset += 1;

		m_Voxels.push_back(VoxelData(glm::translate(glm::mat4(), glm::vec3(x, y, z)), index));

	}

	return false;
}




























//-----------------------------------------------------------------------------------------------------------------------------

//void VoxelContainer::ConstructVoxelMesh(Uint32* VoxelMatrix, Mesh* mesh, size_t sizeX, size_t sizeY, size_t sizeZ)
//{
//
//	size_t blockNumber = 0;
//
//	//-------------------------------------------------------------------------
//	// iterate through each potential block in the matrix
//	//--------------------------------------------------------------------------
//	for (int z = 0; z < sizeZ; ++z)
//	{
//		for (int y = 0; y < sizeY; ++y)
//		{
//			for (int x = 0; x < sizeX; ++x)
//			{
//				//get the RGBA value of the block and extract the alpha value
//				Uint32 temp = VoxelMatrix[x + y*sizeX + z*sizeX*sizeY];
//				Uint8* tempPtr = (Uint8*)&temp;
//
//				//if the block is visible, add its offset to the vector
//				if ((int)tempPtr[3] != 0)
//				{
//					glm::mat4 tempMat;
//					tempMat = glm::translate(tempMat, glm::fvec3(x, y, z));
//					m_Matricies.push_back(tempMat);
//
//					blockNumber++;
//				}
//			}
//		}
//	}

	//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------
	// For each voxel, search it's surrounding neighbours and chech their 
	//---------------------------------------------------------------------------
	//for (int z = 0; z < sizeZ; ++z)
	//{
	//	for (int y = 0; y < sizeY; ++y)
	//	{
	//		for (int x = 0; x < sizeX; ++x)
	//		{
	//			//get the RGBA value of the block and extract the alpha value
	//			Uint32 temp = VoxelMatrix[x + y*sizeX + z*sizeX*sizeY];
	//			Uint8* tempPtr = (Uint8*)&temp;

	//			//if the block is visible, add its offset to the vector
	//			if ((int)tempPtr[3] != 0)
	//			{
	//				glm::mat4 tempMat;
	//				tempMat = glm::translate(tempMat, glm::fvec3(x, y, z));
	//				m_Matricies.push_back(tempMat);

	//				blockNumber++;
	//			}
	//		}
	//	}
	//}


	////push the positions from all visible blocks to the instance buffer
	//if (!mesh->SetInstancing(m_Matricies.data(), m_Matricies.size()))
	//{
	//	std::cout << "\nSomething went wrong with the instancing";
	//}

	////Push Material Data to instance buffer
	//if (!mesh->SetInstancing(m_Matricies.data(), m_Matricies.size()))
	//{
	//	std::cout << "\nSomething went wrong with the instancing";
	//}
//}

#define BUFFER_OFFSET(i) ((char *)0 + (i))


bool VoxelMesh::ConstructVoxelMesh()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{

	}


	if (!m_VoxelMesh.VAO)
		LoadAsCube(m_VoxelMesh); //create base mesh that we will instance.

	glBindVertexArray(m_VoxelMesh.VAO);

	if (m_VoxelMesh.VBO_Instance == 0)
		glGenBuffers(1, &m_VoxelMesh.VBO_Instance); //createInstanceBuffer

	GLsizei vec4Size = sizeof(glm::vec4);

	glBindBuffer(GL_ARRAY_BUFFER, m_VoxelMesh.VBO_Instance);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(VoxelContainer::VoxelData)) * m_VoxelContainer.m_Asset.m_Voxels.size(), &m_VoxelContainer.m_Asset.m_Voxels[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer((GLuint)3, 4, GL_FLOAT, GL_FALSE, sizeof(VoxelContainer::VoxelData), BUFFER_OFFSET(offsetof(VoxelContainer::VoxelData, m_Transform)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer((GLuint)4, 4, GL_FLOAT, GL_FALSE, sizeof(VoxelContainer::VoxelData), BUFFER_OFFSET(vec4Size + offsetof(VoxelContainer::VoxelData, m_Transform)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer((GLuint)5, 4, GL_FLOAT, GL_FALSE, sizeof(VoxelContainer::VoxelData), BUFFER_OFFSET(2 * vec4Size + offsetof(VoxelContainer::VoxelData, m_Transform)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer((GLuint)6, 4, GL_FLOAT, GL_FALSE, sizeof(VoxelContainer::VoxelData), BUFFER_OFFSET(3 * vec4Size + offsetof(VoxelContainer::VoxelData, m_Transform)));

	glEnableVertexAttribArray(7);
	glVertexAttribPointer((GLuint)7, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(VoxelContainer::VoxelData), BUFFER_OFFSET(offsetof(VoxelContainer::VoxelData, m_MaterialIndex)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);

	glBindVertexArray(0);

	m_VoxelMesh.InstanceNumber = m_VoxelContainer.m_Asset.m_Voxels.size();


	if ((err = glGetError()) != GL_NO_ERROR)
	{
		// Process/log the error.
		std::cout << "ebekhf";
	}


	return false;
}
