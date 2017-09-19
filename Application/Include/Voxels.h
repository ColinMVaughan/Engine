#ifndef VOXEL_CONTAINTER_H
#define VOXEL_CONTAINTER_H

#include <PhysicsSystem.h>
#include <ECS.h>
#include <Renderer.h>


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

private:
	void ConstructVoxelMesh(Uint32* VoxelMatrix, Mesh* mesh, size_t sizeX, size_t sizeY, size_t sizeZ);



	Voxel voxels[10][10][10];
	physx::PxRigidStatic* m_Body;


	Material m_Material;
	Mesh m_Mesh;
};


//--------------------------------------------------------------------------------------
template<typename T> 
class RingBuffer
{

	struct bufferData { T data; bool active(false); };
public:
	RingBuffer(size_t a_MaxSize)
		:m_MaxSize(a_MaxSize)
	{
		m_Begining = new bufferData[a_MaxSize];
		m_Front = m_Begining;
		m_Back = m_Front;
	}

	//Allocate Memory in the ring buffer and return a pointer to the new object
	T* Allocate()
	{

	}

	//Reset element at index and mark is as free
	//pointers to this object should be considered invalid
	void Deallocate(unsigned int index)
	{

	}

private:
	size_t m_MaxSize;
	bufferData* m_Begining;

	bufferData* m_Front;
	bufferData* m_Back;
};




class VoxelDestructionSystem: public System<VoxelContainer, Transform>
{
public:
	void PreUpdate(double deltaTime) override
	{
	}
	void Update(double deltaTime, unsigned int entity) override
	{
		if (true)
		{

		}

	}
	void PostUpdate(double deltaTime) override
	{
	}


	void OnTrigger()
	{

	}

private:
	PxRigidBody* m_RigidbodyPool[100];

};
#endif