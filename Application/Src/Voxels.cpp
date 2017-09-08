#include "Voxels.h"
#include <iostream>
#include <fstream>

#define CODEFLAG 2
#define NEXTSLICEFLAG 6

void VoxelContainer::ReadQubicBinaryFile(std::string file, Mesh* mesh)
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

	ImportQB(Matrix, mesh, matrixSizeX, matrixSizeY, matrixSizeZ);
	delete[] Matrix;
	//Uint32 x = 0;
	//Uint32 y = 0;
	//Uint32 z = 0;
	//Uint32 MatData;
	//int loopCount = 0;
	//while (z < matrixSizeZ)
	//{
	//	z++;
	//	Uint32 index = 0;
	//	while (true)
	//	{
	//		MatData = *reinterpret_cast<Uint32*>(data + offset);
	//		offset += 4;

	//		if (MatData == NEXTSLICEFLAG)
	//			break;
	//		else if (MatData == CODEFLAG)
	//		{
	//			Uint32 count = *reinterpret_cast<Uint32*>(data + offset);
	//			offset += 4;
	//			Uint32 MatData = *reinterpret_cast<Uint32*>(data + offset);
	//			offset += 4;

	//			for (int i = 0; i < count; ++i)
	//			{
	//				x = index % matrixSizeX + 1;
	//				y = index / matrixSizeY + 1;
	//				index++;
	//				Matrix[x + y*matrixSizeX + z*matrixSizeX*matrixSizeY] = MatData;
	//			}
	//		}
	//		else
	//		{
	//			x = index % matrixSizeX + 1;
	//			y = index / matrixSizeY + 1;
	//			index++;
	//			Matrix[x + y*matrixSizeY + z*matrixSizeX*matrixSizeY] = MatData;
	//		}
	//		loopCount++;
	//	}
	//}


}

void VoxelContainer::ImportQB(Uint32* Matrix, Mesh* mesh, size_t sizeX, size_t sizeY, size_t sizeZ)
{

	std::vector<glm::fvec3> offsets;
	size_t blockNumber = 0;

	//iterate through each potential block in the matrix
	for (int z = 0; z < sizeZ; ++z)
	{
		for (int y = 0; y < sizeY; ++y)
		{
			for (int x = 0; x < sizeX; ++x)
			{
				//get the RGBA value of the block and extract the alpha value
				Uint32 temp = Matrix[x + y*sizeX + z*sizeX*sizeY];
				Uint8* tempPtr = (Uint8*)&temp;

				//if the block is visible, add its offset to the vector
				if ((int)tempPtr[3] != 0)
				{
					offsets.push_back(glm::fvec3(x, y, z));
					blockNumber++;
				}
			}
		}
	}

	//push the positions from all visible blocks to the instance buffer
	if (!mesh->SetInstancing(offsets.data(), blockNumber))
	{
		std::cout << "\nSomething went wrong with the instancing";
	}
}