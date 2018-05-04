#include "VoxelRenderSystem.h"
#include <iostream>
#include <fstream>
#include <glm\gtc\matrix_transform.hpp>

#define CODEFLAG 2
#define NEXTSLICEFLAG 6

void VoxelContainer::ReadQubicBinaryFile(std::string file)
{
	char* data;
	Uint32 offset;

	Uint32 version;
	Uint32 colourFormat;
	Uint32 zAxisOrientation;
	Uint32 compressed;
	Uint32 visibilityMaskEncoded;
	Uint32 numMatricies;

	//Using a pointer to these elements for the sake of readability
	//and so that I can use these in a loop
	Uint32* HeaderData[6];
	HeaderData[0] = &version;
	HeaderData[1] = &colourFormat;
	HeaderData[2] = &zAxisOrientation;
	HeaderData[3] = &compressed;
	HeaderData[4] = &visibilityMaskEncoded;
	HeaderData[5] = &numMatricies;

	//Open the file and allocate a buffer large enough to accomodate it
	std::ifstream MyFile;
	std::streampos size;
	MyFile.open(file, std::ios::binary|std::ios::ate);
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
		std::cout << "\nCould Not open Qubic Binary File.";
		return;
	}
	//Loop through header data, increasing offset as we collect the data
	offset = 0;
	for (int i = 0; i < 6; ++i)
	{
		*HeaderData[i] = *reinterpret_cast<Uint32*>(data + offset);
		offset += 4;
	}

	std::cout << "\nQB HEADER TEST";
	std::cout << "\n--------------------";
	std::cout << "\nVersion: "<<version;
	std::cout << "\nColour Format: " << colourFormat;
	std::cout << "\ncompressed: " << zAxisOrientation;
	std::cout << "\nVisibility Mask Encoded: " << visibilityMaskEncoded;
	std::cout << "\nNum Matricies: " << numMatricies;
	std::cout << "\n--------------------";
	//---------------------------------------------------
	//Collect Matrix Data

	//Get the length of the matrix name and skip past it because i dont care
	Uint8 nameLength = *reinterpret_cast<Uint8*>(data + offset);
	offset += (nameLength + 1);

	//collect size of matrix
	const Uint32 matrixSizeX = *reinterpret_cast<Uint32*>(data + offset);
	offset += 4;
	const Uint32 matrixSizeY = *reinterpret_cast<Uint32*>(data + offset);
	offset += 4;
	const Uint32 matrixSizeZ = *reinterpret_cast<Uint32*>(data + offset);
	offset += 4;

	//Skipping past the position data of the matrix because I dont care
	offset += 12;

	Uint32* Matrix = new Uint32[matrixSizeX * matrixSizeY * matrixSizeZ];
	for (int z = 0; z < matrixSizeZ; ++z)
	{
		for (int y = 0; y < matrixSizeY; ++y)
		{
			for (int x = 0; x < matrixSizeX; ++x)
			{
				Matrix[x + y*matrixSizeX + z*matrixSizeX*matrixSizeY] = *reinterpret_cast<Uint32*>(data + offset);
				offset += 4;
			}
		}
	}

	ConstructVoxelMesh(Matrix, nullptr, matrixSizeX, matrixSizeY, matrixSizeZ);
	delete[] Matrix;
	delete[] data;
}

//Reads the VOX file format to load in voxel data from magicavoxel
void VoxelContainer::ReadVoxFile(std::string file, Mesh * mesh)
{
	if (!mesh)
	{
		std::cout << "Could not read Vox file: Invalid mesh pointer";
		return;
	}
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
	char ID[4];
	for (int i = 0; i < 3; ++i)
	{
		ID[i] = data[i];
	}
	offset += 4;

	//Read Chunks
	while (true)
	{
		ReadChunk(data, offset);
	}

}

bool VoxelContainer::ReadChunk(char * data, Uint32 & offset)
{
	//Chunk Data
	char chunkID[4];
	int32_t chunkDataSize;
	int32_t chunkChildNum;

	//Chunk'SIZE'
	int32_t sizeX;
	int32_t sizeY;
	int32_t sizeZ;

	//Chunk 'XYZI'
	int32_t numVoxels;
	std::vector<int8_t> voxels;
	//-----------------------------------------------------------

	//Read chunk id
	for (int i = 0; i < 3; ++i)
	{
		chunkID[i] = data[i + offset];
	}
	offset += 4;

	//read size of the chunk in bytes
	chunkDataSize = *reinterpret_cast<int32_t*>(data + offset);
	offset += 4;

	//read number of child chunks
	chunkChildNum = *reinterpret_cast<int32_t*>(data + offset);
	offset += 4;

	//
	switch (chunkID[0])
	{
	case 'N': //MAIN
		break;

	case 'K': //PACK
		offset += 4;
		break;

	case 'E': //SIZE
		int32_t sizeX = *reinterpret_cast<int32_t*>(data + offset);
		offset += 4;

		int32_t sizeY = *reinterpret_cast<int32_t*>(data + offset);
		offset += 4;

		int32_t sizeZ = *reinterpret_cast<int32_t*>(data + offset);
		offset += 4;

		m_Matricies.reserve(sizeX*sizeY*sizeZ);
		m_MaterialIndex.reserve(sizeX*sizeY*sizeZ);

		for (int i = 0; i < sizeX*sizeY*sizeZ; ++i)
		{
			m_Matricies.push_back(glm::mat4());
			m_MaterialIndex.push_back(unsigned int());
		}
		break;

	case 'I': //XYZI
		int32_t numVoxels = *reinterpret_cast<int32_t*>(data + offset);
		offset += 4;
		for (int32_t i = 0; i< numVoxels; ++i)
		{
			int8_t x = *reinterpret_cast<int8_t*>(data + offset);
			offset += 1;

			int8_t y = *reinterpret_cast<int8_t*>(data + offset);
			offset += 1;

			int8_t z = *reinterpret_cast<int8_t*>(data + offset);
			offset += 1;

			int8_t index = *reinterpret_cast<int8_t*>(data + offset);
			offset += 1;

			m_Matricies[i] = glm::translate(m_Matricies[i], glm::vec3(x,y,z));
			m_MaterialIndex[i] = index;
		}
		break;

	case 'A': //RGBA
		offset += (4 * 256);
		break;

	case 'T': //MATT
		return false;
		break;

	}

	return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

void VoxelContainer::ConstructVoxelMesh(Uint32* VoxelMatrix, Mesh* mesh, size_t sizeX, size_t sizeY, size_t sizeZ)
{

	size_t blockNumber = 0;

	//-------------------------------------------------------------------------
	// iterate through each potential block in the matrix
	//--------------------------------------------------------------------------
	for (int z = 0; z < sizeZ; ++z)
	{
		for (int y = 0; y < sizeY; ++y)
		{
			for (int x = 0; x < sizeX; ++x)
			{
				//get the RGBA value of the block and extract the alpha value
				Uint32 temp = VoxelMatrix[x + y*sizeX + z*sizeX*sizeY];
				Uint8* tempPtr = (Uint8*)&temp;

				//if the block is visible, add its offset to the vector
				if ((int)tempPtr[3] != 0)
				{
					glm::mat4 tempMat;
					tempMat = glm::translate(tempMat, glm::fvec3(x, y, z));
					m_Matricies.push_back(tempMat);

					blockNumber++;
				}
			}
		}
	}

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
}