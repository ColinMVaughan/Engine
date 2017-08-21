#ifndef PLAYER_CONTROL_H
#define PLAYER_CONTROL_H

#include<BaseSystem.h>
#include <PhysicsSystem.h>

//	**This file will be renamed to PlayerControlSystem**

// PlayerControl is used to take input and control the player's movement.

class PlayerControl
{
public:
	void Initalize(PxCapsuleControllerDesc controlDescription)
	{
	
	}

	PxController* m_Controller;
};

class PlayerControlSystem : public System<PlayerControl>
{
public:
	PlayerControlSystem(ComponentManager* a_cmanager)
		:System(a_cmanager) {}

	void Update(unsigned int entity) override;

	void KeyDown(int key);
	void KeyUp(int key);


private:
	PxVec3 MoveDirection;
};

#endif