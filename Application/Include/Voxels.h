#ifndef VOXEL_CONTAINTER_H
#define VOXEL_CONTAINTER_H

#include <PhysicsSystem.h>
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


//--------------------------------------------------------------------------------------
//							VOXEL DESTRUCTION SYSTEM
//
//	VDS allows voxel containers to be destroyed dynamically. when a voxel container is destroyed,
//	its voxels are connected with a rigidbody untill the voxel becomes stationary.

//	Be careful with performance: any dynamic voxel container will have it's mesh instance buffer 
//	resubmitted to the GPU each frame.
//	
//--------------------------------------------------------------------------------------

class VoxelDestructionSystem: public ECS::System<VoxelContainer, Mesh, Transform>
{
public:
	VoxelDestructionSystem(ECS::ComponentManager* a_CompManager)
		:System(a_CompManager) {}

	void Initalize(PhysicsSystem* m_PhysicsSystem)
	{
		PxMaterial* shapeMaterial = m_PhysicsSystem->GetPhysics()->createMaterial(0.5, 0.5, 0.5);
		m_CollisionShape = m_PhysicsSystem->GetPhysics()->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *shapeMaterial, false);
		//Generate collection of Rigidbodies and Assign them Pointers
		for (int i = 0; i< 350; ++i)
		{

			m_RigidbodyPool[i] = m_PhysicsSystem->GetPhysics()->createRigidDynamic(PxTransform((float)i,(float)i,(float)i));
			m_PhysicsSystem->GetScene()->addActor(*m_RigidbodyPool[i]);
			m_RigidbodyPool[i]->attachShape(*m_CollisionShape);
			m_RigidbodyPool[i]->setMass(100.0f);
			m_RigidbodyPool[i]->putToSleep();


		}

		m_CollisionShape->release();

		std::cout << "\nInitalization Finished...";
	}

	void PreUpdate(double deltaTime) override
	{
		//Iterates through each active rigidbody and sets its 
		//corrisponding matrix to the updated pose.

		for (int i = 0; i < m_RigidbodyNumber; ++i)
		{
			//userdata holds a void* to the voxel transform matrix it is connected to.
			glm::mat4* matrix = static_cast<glm::mat4*>(m_RigidbodyPool[i]->userData);
			
			auto temp = PxMat44(m_RigidbodyPool[i]->getGlobalPose()).front();

			*matrix = glm::make_mat4(temp);
			//std::cout << "what";
		}
	}

	void Update(double deltaTime, unsigned int entity) override
	{
		//Collect entity's mesh and voxelContainer components
		auto mesh   = m_CManager->GetComponent<Mesh>(entity);
		auto voxels = m_CManager->GetComponent<VoxelContainer>(entity);

		//Submit the new Voxel matricies to the gpu.
		//Currently the entire instance buffer will have to be resubmitted
		if (voxels->dynamic)
		{
			mesh->BufferSubData(mesh->VBO_Instance, 0, voxels->m_Matricies.size() * sizeof(glm::mat4), (void*)voxels->m_Matricies.data());
		}
	}

	void PostUpdate(double deltaTime) override
	{
		
	}


	void KeyDown(unsigned char key)
	{
		if (key == 'F')
		{
			//Loop through each affected voxel and set a rigidbody to their position.
			for (int i = 0; i < TestCase->m_Matricies.size(); ++i)
			{
				//this could be a problem because of local scope
				//if none of the objects move, it could be because of that.

				//PxMat44 mat(); //convert the GLM mat4 into a PhysX mat4
				PxTransform transform(PxVec3(TestCase->m_Matricies[i][3].x, TestCase->m_Matricies[i][3].y, TestCase->m_Matricies[i][3].z)); //Initalize a physx transform with the voxel transform matrix


				//Sets the Rigidbody to the position/orientation/size of the voxel and wake it up
				m_RigidbodyPool[i]->setGlobalPose(transform, true);
				m_RigidbodyPool[i]->wakeUp();

				m_RigidbodyPool[i]->addForce(PxVec3(500, 0, 0));
				//attach a pointer to the voxel matrix to the rigidbody
				//so that we can update the location of the matrix
				m_RigidbodyPool[i]->userData = &TestCase->m_Matricies[i];
				TestCase->dynamic = true;

				//Incrase the number of active rigidbodies.
				//This will be replaced soon
				m_RigidbodyNumber++;

			}
			std::cout << "\nCompleted.";
		}

	}

	void AddTestCase(VoxelContainer* a_VC)
	{
		TestCase = a_VC;
	}


private:
	//To Be Replaced with a ringbuffer
	//Idea: it would be nice to have the number of rigidbodies be user definable
	PxRigidDynamic* m_RigidbodyPool[400];
	unsigned int m_RigidbodyNumber = 0;

	PxShape* m_CollisionShape;	//Every voxel rigidbody will share this shape
	VoxelContainer* TestCase;
};
#endif