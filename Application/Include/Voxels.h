#ifndef VOXEL_CONTAINTER_H
#define VOXEL_CONTAINTER_H

#include <PhysicsSystem.h>
#include <Entity.h>
#include <ECS.h>

#include <Renderer.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//template<typename T>
//class RingBuffer
//{
//
//	struct bufferData { T data; bool active(false); };
//public:
//	RingBuffer(size_t a_MaxSize)
//		:m_MaxSize(a_MaxSize)
//	{
//		m_Begining = new bufferData[a_MaxSize];
//		m_Front = m_Begining;
//		m_Back = m_Front;
//	}
//
//	//Allocate Memory in the ring buffer and return a pointer to the new object
//	T* Allocate()
//	{
//
//	}
//
//	//Reset element at index and mark is as free
//	//pointers to this object should be considered invalid
//	void Deallocate(unsigned int index)
//	{
//
//	}
//
//private:
//	size_t m_MaxSize;
//	bufferData* m_Begining;
//
//	bufferData* m_Front;
//	bufferData* m_Back;
//};

//--------------------------------------------------------------------------------------
//									VOXELS
//--------------------------------------------------------------------------------------

class Voxel
{
public:
	int x, y, z;
	int MaterialIndex = 0;
};

class VoxelContainer
{
public:

	void ReadQubicBinaryFile(std::string file, Mesh* mesh);
	
	std::vector<glm::mat4> m_Matricies;
	std::vector<glm::vec2> m_UVs;

	bool dynamic = false;

private:
	void ConstructVoxelMesh(Uint32* VoxelMatrix, Mesh* mesh, size_t sizeX, size_t sizeY, size_t sizeZ);
	void ConstructVoxelMaterial();
	Material m_Material;
	Mesh m_Mesh;

};



#endif