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

	PxRigidDynamic* GetActor()
	{
		return m_Controller->getActor();
	}

private:
	PxController* m_Controller;
	PxControllerFilters filters;
};




class PlayerControlSystem : public System<PlayerControl>
{
public:
	PlayerControlSystem(ComponentManager* a_cmanager)
		:System(a_cmanager) 
	{
		XForce *= 0;
		YForce *= 0;
		ZForce *= 0;

		YForce.y = -0.0981;
	}


	void PreUpdate(double deltaTime) override
	{
		MoveDirection *= 0.0f;
		MoveDirection = XForce + YForce + ZForce;
	}

	void Update(double deltaTime, unsigned int entity) override
	{
		m_CManager->GetComponent<PlayerControl>(entity)->Move(MoveDirection, deltaTime);
		return;
	}

	void KeyDown(unsigned char key) override
	{
		switch (key)
		{
		case 'w':
			XForce.x = 0.3f;
			break;
		case 'a':
			ZForce.z = -0.3f;
			break;
		case 's':
			XForce.x = -0.3f;
			break;
		case'd':
			ZForce.z = 0.3f;
			break;

		}
	}
	void KeyUp(unsigned char key) override
	{
		switch (key)
		{
		case 'w':
			XForce.x = 0.0f;
			break;
		case 'a':
			ZForce.z = 0.0f;
			break;
		case 's':
			XForce.x = 0.0f;
			break;
		case'd':
			ZForce.z = 0.0f;
			break;

		}
	}


private:
	PxVec3 XForce;
	PxVec3 YForce;
	PxVec3 ZForce;
	
	PxVec3 MoveDirection;
};

#endif