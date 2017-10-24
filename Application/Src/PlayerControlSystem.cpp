#include "PlayerControlSystem.h"
#include <glm/gtc/type_ptr.hpp>

void PlayerControl::Initalize(PxController* controller)
{
	m_Controller = controller;
}

void PlayerControl::Move(PxVec3 moveDirection, double deltaTime)
{
	m_Controller->move(moveDirection, 0.0001f, deltaTime, filters);
}

PxRigidDynamic* PlayerControl::GetActor()
{
	return m_Controller->getActor();
}

//--------------------------------------------------------------------------------------------------

FPSControlSystem::FPSControlSystem(ECS::ComponentManager* a_cmanager)
	:System(a_cmanager)
{
	MoveDirection = PxVec3(0);
	rotations = PxVec2(0);
}

void FPSControlSystem::PreUpdate(double deltaTime)
{

}

//Moves the playercontroller in the accumulated direction 
void FPSControlSystem::Update(double deltaTime, ECS::Entity& entity)
{
	//Get Transform and camera components of entity
	auto transform	   = entity.GetComponent<Transform>();
	auto camera		   = entity.GetComponent<Camera>();
	auto playercontrol = entity.GetComponent<PlayerControl>();

	//Rotate camera by whatever ammout
	//transform->GetTransform()->q = PxQuat(PxIdentity);
	//transform->GetTransform()->q *= PxQuat(rotations.x, PxVec3(0, 1, 0));
	//transform->GetTransform()->q *= PxQuat(rotations.y, PxVec3(1, 0, 0));

	//Get Rotation Quaternion to orient the move direction along the Y axis.
	PxQuat dir = PxQuat(PxIdentity);
	dir *= PxQuat(rotations.x, PxVec3(0, 1, 0));
	
	//move the player by this rotated direction vector
	playercontrol->Move(dir.rotate(MoveDirection * deltaTime), deltaTime);
	
	//Add other rotation component and add rotation to transform
	//NOTE: We rotate the transform because the playerController's PhysX Actor 
	//doesn't store a rotation, so we need to add it manually.
	dir *= PxQuat(rotations.y, PxVec3(1, 0, 0));
	auto tempTransform = transform->GetTransform();

	tempTransform->q = dir;
	
	//Convert new transform to a translation matrix.
	camera->m_Transform = glm::make_mat4(PxMat44(*tempTransform).front());


	//Move Playercontroller to the desired direction
	//transform->GetTransform()->p += transform->GetTransform()->rotate(MoveDirection);

	// Convert physx::PxMat44 to glm::mat4 and send updated transform to camera component
	// (I would like to change this in the fututre)
	//camera->m_Transform = glm::make_mat4(transform->GetGlobalTransformMatrix().front());
	return;
}

void FPSControlSystem::PostUpdate(double deltaTime)
{
	//zero out rotation for next update
	rotation = PxQuat(PxIDENTITY::PxIdentity);

	//zero out move direction for next update
	MoveDirection *= 0;
	MoveDirection.y = -9.8f;
}


void FPSControlSystem::KeyDown(unsigned char key)
{
	switch (key)
	{
	case 'W':
		MoveDirection.z -= 5.5f;
		break;
	case 'A':
		MoveDirection.x -= 5.5f;
		break;
	case 'S':
		MoveDirection.z += 5.5f;
		break;
	case'D':
		MoveDirection.x += 5.5f;
		break;
	case 'E':
		MoveDirection.y += 5.5f;
		break;
	case 'Q':
		MoveDirection.y -= 5.5f;
		break;
	}
}

void FPSControlSystem::KeyUp(unsigned char key)
{

}

// If the Mouse is moved, Rotate the Camera's View Matrix 
// and change the foreward direction used for movement
// 
void FPSControlSystem::MouseMoved(float xVelocity, float yVelocity)
{
	rotations.x += xVelocity;
	rotations.y += yVelocity;

}