#pragma once
#include <PhysicsSystem.h>
#include <ECS.h>
#include <Voxels.h>

#include <Renderer.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "RenderSystem.h"

//--------------------------------------------------------------------------------------
//							VOXEL DESTRUCTION SYSTEM
//
//	VDS allows voxel containers to be destroyed dynamically. when a voxel container is destroyed,
//	its voxels are connected with a rigidbody untill the voxel becomes stationary.

//	Be careful with performance: any dynamic voxel container will have it's mesh instance buffer 
//	resubmitted to the GPU each frame.
//	
//--------------------------------------------------------------------------------------

class VoxelDestructionSystem : public ECS::System<VoxelContainerFilter, Mesh, Transform>
{
public:
	VoxelDestructionSystem(ECS::ComponentManager* a_CompManager, EventManager& a_eManager);

	void Initalize(PhysicsSystem* m_PhysicsSystem);

	void PreUpdate(double deltaTime) override;
	void Update(double deltaTime, ECS::Entity& entity) override;
	void PostUpdate(double deltaTime) override;


	void KeyDown(unsigned char key);

	
	void AddTestCase(VoxelContainer* a_VC)
	{
		TestCase = a_VC;
	}

	void ComponentAdded(ECS::Entity& entity) override
	{
		auto mesh = entity.GetComponent<MeshFilter>();
		auto voxels = entity.GetComponent<VoxelContainerFilter>();

		//if the entity has both mesh and voxel components
		if(mesh && voxels)
		{
			

			//Set Instancing for voxel container
			mesh->m_Mesh.m_Asset.SetInstancing(voxels->m_VoxelContainer.m_Asset.m_Matricies.data(), voxels->m_VoxelContainer.m_Asset.m_Matricies.size());
		}

		return;
	}

private:
	//To Be Replaced with a ringbuffer
	//Idea: it would be nice to have the number of rigidbodies be user definable
	PxRigidDynamic* m_RigidbodyPool[400];
	unsigned int m_RigidbodyNumber = 0;

	PxShape* m_CollisionShape;	//Every voxel rigidbody will share this shape
	VoxelContainer* TestCase;
};