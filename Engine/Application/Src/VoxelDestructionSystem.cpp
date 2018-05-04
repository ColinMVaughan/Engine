//#include "VoxelDestructionSystem.h"
//
//VoxelDestructionSystem::VoxelDestructionSystem(ECS::ComponentManager* a_CompManager, EventManager& a_eManager)
//	:System(a_CompManager, a_eManager)
//{
//	
//	//Voxel container changed
//}
//
//
//
//void VoxelDestructionSystem::Initalize(PhysXWrapper* m_PhysicsSystem)
//{
//	PxMaterial* shapeMaterial = m_PhysicsSystem->GetPhysics()->createMaterial(0.5, 0.5, 0.5);
//	m_CollisionShape = m_PhysicsSystem->GetPhysics()->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *shapeMaterial, false);
//	//Generate collection of Rigidbodies and Assign them Pointers
//	for (int i = 0; i< 350; ++i)
//	{
//
//		m_RigidbodyPool[i] = m_PhysicsSystem->GetPhysics()->createRigidDynamic(PxTransform((float)i, (float)i, (float)i));
//		m_PhysicsSystem->GetScene()->addActor(*m_RigidbodyPool[i]);
//		m_RigidbodyPool[i]->attachShape(*m_CollisionShape);
//		m_RigidbodyPool[i]->setMass(100.0f);
//		m_RigidbodyPool[i]->putToSleep();
//
//
//	}
//
//	m_CollisionShape->release();
//
//	std::cout << "\nInitalization Finished...";
//}
//
//
//void VoxelDestructionSystem::PreUpdate(double deltaTime)
//{
//	//Iterates through each active rigidbody and sets its 
//	//corrisponding matrix to the updated pose.
//
//	for (int i = 0; i < m_RigidbodyNumber; ++i)
//	{
//		//userdata holds a void* to the voxel transform matrix it is connected to.
//		glm::mat4* matrix = static_cast<glm::mat4*>(m_RigidbodyPool[i]->userData);
//
//		auto temp = PxMat44(m_RigidbodyPool[i]->getGlobalPose()).front();
//
//		*matrix = glm::make_mat4(temp);
//		//std::cout << "what";
//	}
//}
//
//
//void VoxelDestructionSystem::Update(double deltaTime, ECS::Entity& entity)
//{
//	//Collect entity's mesh and voxelContainer components
//	auto mesh = entity.GetComponent<Mesh>();
//	auto voxels = entity.GetComponent<VoxelContainerFilter>();
//
//
//	//Submit the new Voxel matricies to the gpu.
//	//Currently the entire instance buffer will have to be resubmitted
//	if (voxels->dynamic)
//	{
//		mesh->BufferSubData(mesh->VBO_Instance,
//			0, 
//			voxels->m_VoxelContainer.m_Asset.m_Matricies.size() * sizeof(glm::mat4),
//			(void*)voxels->m_VoxelContainer.m_Asset.m_Matricies.data());
//	}
//}
//
//void VoxelDestructionSystem::PostUpdate(double deltaTime)
//{
//
//}
//
//void VoxelDestructionSystem::Start(ECS::Entity& entity)
//{
//	auto voxels = entity.GetComponent<VoxelContainerFilter>();
//	auto transform = entity.GetComponent<Transform>();
//
//	//For each block in  the voxel container
//	for each(glm::mat4 matrix in voxels->m_VoxelContainer.m_Asset.m_Matricies)
//	{
//		//create a rigidbody at the position of the associated block
//		PxTransform voxelTransform = PxTransform(PxMat44(matrix[0][0]));
//		transform->GetTransform()->transform(voxelTransform);
//		PxRigidDynamic* body = m_Physics->GetPhysics()->createRigidDynamic(voxelTransform);
//
//		//attach shape to block
//
//	}
//}
//
//
//void VoxelDestructionSystem::KeyDown(unsigned char key)
//{
//	if (key == 'F')
//	{
//		////Loop through each affected voxel and set a rigidbody to their position.
//		//for (int i = 0; i < TestCase->m_Matricies.size(); ++i)
//		//{
//		//	//this could be a problem because of local scope
//		//	//if none of the objects move, it could be because of that.
//
//		//	//PxMat44 mat(); //convert the GLM mat4 into a PhysX mat4
//		//	PxTransform transform(PxVec3(TestCase->m_Matricies[i][3].x, TestCase->m_Matricies[i][3].y, TestCase->m_Matricies[i][3].z)); //Initalize a physx transform with the voxel transform matrix
//
//
//		//																																//Sets the Rigidbody to the position/orientation/size of the voxel and wake it up
//		//	m_RigidbodyPool[i]->setGlobalPose(transform, true);
//		//	m_RigidbodyPool[i]->wakeUp();
//
//		//	m_RigidbodyPool[i]->addForce(PxVec3(500, 0, 0));
//		//	//attach a pointer to the voxel matrix to the rigidbody
//		//	//so that we can update the location of the matrix
//		//	m_RigidbodyPool[i]->userData = &TestCase->m_Matricies[i];
//		//	TestCase->dynamic = true;
//
//		//	//Incrase the number of active rigidbodies.
//		//	//This will be replaced soon
//		//	m_RigidbodyNumber++;
//
//		//}
//		//std::cout << "\nCompleted.";
//	}
//
//}
//
//
//void VoxelDestructionSystem::EntityRegistered(ECS::Entity& entity)
//{
//	//get relevent components
//	auto mesh = entity.GetComponent<MeshFilter>();
//	auto vox = entity.GetComponent<VoxelContainerFilter>();
//
//	//Set up voxel mesh/instancing
//	LoadAsCube(vox->m_Mesh);
//	vox->m_Mesh.SetInstancing(&vox->m_VoxelContainer.m_Asset.m_Matricies[0],
//		vox->m_VoxelContainer.m_Asset.m_Matricies.size());
//
//	//set entity's mesh to this new voxel one.
//	mesh->m_Mesh.m_Asset = vox->m_Mesh;
//	mesh->m_Mesh.m_AssetName = "";
//
//}
//
