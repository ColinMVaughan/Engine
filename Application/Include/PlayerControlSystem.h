#ifndef PLAYER_CONTROL_H
#define PLAYER_CONTROL_H

#include<BaseSystem.h>
#include <PhysicsSystem.h>


// PlayerControl is used to take input and control the player's movement.
// Note: Each Playercontrol component will be moved in the same direction defined by the 
//       System.

class PlayerControl
{
public:

	void Initalize(PxController* controller);
	void Move(PxVec3 moveDirection, double deltaTime);
	PxRigidDynamic* GetActor();

private:
	PxController* m_Controller;
	PxControllerFilters filters;
};

//------------------------------------------------------------------------------------------


class PlayerControlSystem : public System<PlayerControl>
{
public:

	PlayerControlSystem(ComponentManager* a_cmanager);

	void PreUpdate(double deltaTime) override;
	void Update(double deltaTime, unsigned int entity) override;

	void KeyDown(unsigned char key) override;
	void KeyUp(unsigned char key) override;


private:
	PxVec3 XForce;
	PxVec3 YForce;
	PxVec3 ZForce;
	
	PxVec3 MoveDirection;
};

#endif