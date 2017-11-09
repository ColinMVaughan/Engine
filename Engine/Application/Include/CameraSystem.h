#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include <ECS.h>
#include <PhysicsSystem.h>
#include <Renderer.h>

#include <glm\mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

//-----------------------------------------------------------------------------
//					CameraSystem
//
//	CameraSystem sends the entitie's transform matrix to the Camera component
//	once per frame.
//	
//	Required Components: Camera, Transform
//-----------------------------------------------------------------------------

class CameraSystem : public ECS::System<Camera, Transform>
{
	CameraSystem(ECS::ComponentManager* a_CompManager)
		:System(a_CompManager) {}

	// This Updates the Camera Component's transform menmber to the Entities global transform.
	// This Will most likley be refactored later.
	void Update(double deltaTime, ECS::Entity& entity) override
	{
		//Get Required Components
		auto camera		= entity.GetComponent<Camera>();
		auto transform  = entity.GetComponent<Transform>();

		//Set the Camera's Transform Component to the Transform of the Entity
		camera->m_Transform = glm::make_mat4(transform->GetGlobalTransformMatrix().front());
	}
};


//---------------------------------------------------------------------------------------
//						Debug Camera Control
//
//	Debug Camera Control is used for controlling the camera without being connected to the player
//	or gravity & collisions.
//
//	Required Components: DebugControl, Transform, Camera
//---------------------------------------------------------------------------------------

struct DebugControl
{

};

class DebugCameraControlSystem : public ECS::System<DebugControl, Transform, Camera>
{
public:

	DebugCameraControlSystem(ECS::ComponentManager* a_cmanager);

	void PreUpdate(double deltaTime) override;
	void Update(double deltaTime, ECS::Entity& entity) override;
	void PostUpdate(double deltaTime) override;

	void KeyDown(unsigned char key) override;
	void KeyUp(unsigned char key) override;

	void MouseMoved(float xVelocity, float yVelocity);

private:
	PxVec3 MovementDirection;

	float speed = 0.3f;
	PxVec3 MoveDirection;
	PxQuat rotation;

	PxVec2 rotations;
};

#endif