#include "VoxelDestructionSystem.h"

VoxelDestructionSystem::VoxelDestructionSystem(ECS::ComponentManager* a_CompManager)
	:System(a_CompManager) {}



void VoxelDestructionSystem::Initalize(PhysicsSystem* m_PhysicsSystem)
{
	PxMaterial* shapeMaterial = m_PhysicsSystem->GetPhysics()->createMaterial(0.5, 0.5, 0.5);
	m_CollisionShape = m_PhysicsSystem->GetPhysics()->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *shapeMaterial, false);
	//Generate collection of Rigidbodies and Assign them Pointers
	for (int i = 0; i< 350; ++i)
	{

		m_RigidbodyPool[i] = m_PhysicsSystem->GetPhysics()->createRigidDynamic(PxTransform((float)i, (float)i, (float)i));
		m_PhysicsSystem->GetScene()->addActor(*m_RigidbodyPool[i]);
		m_RigidbodyPool[i]->attachShape(*m_CollisionShape);
		m_RigidbodyPool[i]->setMass(100.0f);
		m_RigidbodyPool[i]->putToSleep();


	}

	m_CollisionShape->release();

	std::cout << "\nInitalization Finished...";
}


void VoxelDestructionSystem::PreUpdate(double deltaTime)
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


void VoxelDestructionSystem::Update(double deltaTime, ECS::Entity& entity)
{
	//Collect entity's mesh and voxelContainer components
	auto mesh = entity.GetComponent<Mesh>();
	auto voxels = entity.GetComponent<VoxelContainer>();

	//Submit the new Voxel matricies to the gpu.
	//Currently the entire instance buffer will have to be resubmitted
	if (voxels->dynamic)
	{
		mesh->BufferSubData(mesh->VBO_Instance, 0, voxels->m_Matricies.size() * sizeof(glm::mat4), (void*)voxels->m_Matricies.data());
	}
}

void VoxelDestructionSystem::PostUpdate(double deltaTime)
{

}


void VoxelDestructionSystem::KeyDown(unsigned char key)
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