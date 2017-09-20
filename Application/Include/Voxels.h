#ifndef VOXEL_CONTAINTER_H
#define VOXEL_CONTAINTER_H

#include <PhysicsSystem.h>
#include <ECS.h>
#include <Renderer.h>

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
	bool dynamic = false;

private:
	void ConstructVoxelMesh(Uint32* VoxelMatrix, Mesh* mesh, size_t sizeX, size_t sizeY, size_t sizeZ);

	Material m_Material;
	Mesh m_Mesh;

};


//--------------------------------------------------------------------------------------


class VoxelDestructionSystem: public System<VoxelContainer, Mesh, Transform>
{
public:
	VoxelDestructionSystem(ComponentManager* a_CompManager)
		:System(a_CompManager) {}

	void Initalize()
	{
		
	}

	void PreUpdate(double deltaTime) override
	{
	}

	void Update(double deltaTime, unsigned int entity) override
	{
		auto mesh   = m_CManager->GetComponent<Mesh>(entity);
		auto voxels = m_CManager->GetComponent<VoxelContainer>(entity);

		if (voxels->dynamic)
		{
			for (int i = 0; voxels->m_Matricies.size(); ++i)
			{
				auto temp = PxMat44(m_RigidbodyPool[i]->getGlobalPose()).front();
				voxels->m_Matricies[i] = glm::mat4(*temp);
			}
			mesh->BufferSubData(mesh->VBO_Instance, 0, voxels->m_Matricies.size(), (void*)voxels->m_Matricies.data());
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