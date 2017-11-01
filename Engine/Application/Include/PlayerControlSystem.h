#ifndef PLAYER_CONTROL_H
#define PLAYER_CONTROL_H

#include<BaseSystem.h>
#include <PhysicsSystem.h>
#include <Renderer.h>

//----------------------------------------------------------------------------------------
// PlayerControl is used to take input and control the player's movement.
// Note: Each Playercontrol component will be moved in the same direction defined by the 
//       System.
//----------------------------------------------------------------------------------------

class PlayerControl
{
public:

	void Initalize(PxController* controller);
	void Move(PxVec3 moveDirection, double deltaTime);
	PxRigidDynamic* GetActor();

private:
	PxController* m_Controller;
	PxControllerFilters filters;
	PxQuat Forward;
};

//------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------

class FPSControlSystem : public ECS::System<Transform, Camera, PlayerControl>
{
public:

	FPSControlSystem(ECS::ComponentManager* a_cmanager);

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