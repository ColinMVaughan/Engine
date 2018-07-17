#include "CameraSystem.h"

DebugCameraControlSystem::DebugCameraControlSystem(ECS::ComponentManager* a_cmanager, EventManager& a_eManager)
	:System(a_cmanager, a_eManager)
{
	MoveDirection = PxVec3(0);
	rotations = PxVec2(0);
}

void DebugCameraControlSystem::PreUpdate(double deltaTime)
{

}


void DebugCameraControlSystem::Update(double deltaTime, ECS::Entity& entity)
{
	//Get Transform and camera components of entity
	auto transform = entity.GetComponent<Transform>();
	//auto camera = entity.GetComponent<Camera>();

	//Rotate camera by whatever ammout
	//transform->GetTransform()->q = PxQuat(PxIdentity);
	//transform->GetTransform()->q *= PxQuat(rotations.x, PxVec3(0, 1, 0));
	//transform->GetTransform()->q *= PxQuat(rotations.y, PxVec3(1, 0, 0));

	//PxVec3 zoom(0, 0, Zoom);

	//zoom = transform->GetTransform()->q.rotate(zoom);

	//Translate camera in orientation corrected movement direction
	//transform->GetTransform()->p = zoom;//+= transform->GetTransform()->rotate(MoveDirection) * deltaTime;


	//---------------------------------------------------
	PxMat44 Zoom;
	PxMat44 Rot;
	PxMat44 Translate;

	PxQuat quatRot = PxQuat(rotations.x, PxVec3(0, 1, 0)) * PxQuat(rotations.y, PxVec3(1, 0, 0));


	Zoom.setPosition(PxVec3(0.0f, 0.0f, MyZoom));
	Rot = PxMat44(quatRot);
	Translate.setPosition(MoveDirection);

	PxMat44 finalMat = Rot * Zoom;

	//*transform->GetTransform() = PxTransform(Translate);
	transform->GetTransform()->p = finalMat.transform(MoveDirection);
	transform->GetTransform()->q = quatRot;

	//--------------------------------------------------


	// Convert physx::PxMat44 to glm::mat4 and send updated transform to camera component
	// (I would like to change this in the fututre)
	//camera->m_Transform = glm::make_mat4(transform->GetGlobalTransformMatrix().front());
	return;
}

void DebugCameraControlSystem::PostUpdate(double deltaTime)
{
	//zero out rotation for next update
	//rotation = PxQuat(PxIDENTITY::PxIdentity);

	//zero out move direction for next update
	//MoveDirection *= 0;


}


//Checks that the relivent modifier key has been pressed.
void DebugCameraControlSystem::KeyDown(unsigned char key)
{
	float Speed = 0.5f;

	switch (key)
	{
	case 'W':
		MoveDirection.z -= Speed;
		break;
	case 'A':
		MoveDirection.x -= Speed;
		break;
	case 'S':
		MoveDirection.z += Speed;
		break;
	case'D':
		MoveDirection.x += Speed;
		break;
	case 'E':
		MoveDirection.y += Speed;
		break;
	case 'Q':
		MoveDirection.y -= Speed;
		break;


	case 'Z':
		MyZoom -= Speed;
		break;
	case 'X':
		MyZoom += Speed;
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