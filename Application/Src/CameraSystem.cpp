#include "CameraSystem.h"

DebugCameraControlSystem::DebugCameraControlSystem(ECS::ComponentManager* a_cmanager)
	:System(a_cmanager)
{
	MoveDirection = PxVec3(0);
	rotations = PxVec2(0);
}

void DebugCameraControlSystem::PreUpdate(double deltaTime)
{

}


void DebugCameraControlSystem::Update(double deltaTime, unsigned int entity)
{
	//Get Transform and camera components of entity
	auto transform = m_CManager->GetComponent<Transform>(entity);
	auto camera = m_CManager->GetComponent<Camera>(entity);

	//Rotate camera by whatever ammout
	transform->GetTransform()->q = PxQuat(PxIdentity);
	transform->GetTransform()->q *= PxQuat(rotations.x, PxVec3(0, 1, 0));
	transform->GetTransform()->q *= PxQuat(rotations.y, PxVec3(1, 0, 0));

	//Translate camera in orientation corrected movement direction
	transform->GetTransform()->p += transform->GetTransform()->rotate(MoveDirection);


	// Convert physx::PxMat44 to glm::mat4 and send updated transform to camera component
	// (I would like to change this in the fututre)
	camera->m_Transform = glm::make_mat4(transform->GetGlobalTransformMatrix().front());
	return;
}

void DebugCameraControlSystem::PostUpdate(double deltaTime)
{
	//zero out rotation for next update
	rotation = PxQuat(PxIDENTITY::PxIdentity);

	//zero out move direction for next update
	MoveDirection *= 0;


}

void DebugCameraControlSystem::KeyDown(unsigned char key)
{
	switch (key)
	{
	case 'W':
		MoveDirection.z -= 0.5f;
		break;
	case 'A':
		MoveDirection.x -= 0.5f;
		break;
	case 'S':
		MoveDirection.z += 0.5f;
		break;
	case'D':
		MoveDirection.x += 0.5f;
		break;
	case 'E':
		MoveDirection.y += 0.5f;
		break;
	case 'Q':
		MoveDirection.y -= 0.5f;
		break;
	}
}

void DebugCameraControlSystem::KeyUp(unsigned char key)
{

}

// If the Mouse is moved, Rotate the Camera's View Matrix 
// and change the foreward direction used for movement
// 
void DebugCameraControlSystem::MouseMoved(float xVelocity, float yVelocity)
{
	rotations.x += xVelocity;
	rotations.y += yVelocity;

}