#ifndef PLAYER_CONTROL_H
#define PLAYER_CONTROL_H

#include<BaseSystem.h>
#include <PhysicsSystem.h>

//	**This file will be renamed to PlayerControlSystem**

// PlayerControl is used to take input and control the player's movement.
// Note: Each Playercontrol component will be moved in the same direction defined by the 
//       System.

class PlayerControl
{
public:
	void Initalize(PxController* controller)
	{
		m_Controller = controller;
	}

	void Move(PxVec3 moveDirection, double deltaTime)
	{
		m_Controller->move(moveDirection, 0.0001f, deltaTime, filters);
	}

private:
	PxController* m_Controller;
	PxControllerFilters filters;
};

class PlayerControlSystem : public System<PlayerControl>
{
public:
	PlayerControlSystem(ComponentManager* a_cmanager)
		:System(a_cmanager) {}

	void Update(unsigned int entity) override
	{
		m_CManager->GetComponent<PlayerControl>(entity)->Move(MoveDirection, 0.016);
		return;
	}

	void KeyDown(int key)
	{
		switch (key)
		{
		case 'w':
			break;
		case 'a':
			break;
		case 's':
			break;
		case'd':
			break;

		}
	}
	void KeyUp(int key)
	{
		switch (key)
		{
		case 'w':
			break;
		case 'a':
			break;
		case 's':
			break;
		case'd':
			break;

		}
	}


private:
	PxVec3 MoveDirection;
};

#endif