#include "PlayerControlSystem.h"

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

PlayerControlSystem::PlayerControlSystem(ComponentManager* a_cmanager)
	:System(a_cmanager)
{
	XForce *= 0;
	YForce *= 0;
	ZForce *= 0;

	YForce.y = -0.0981;
}

void PlayerControlSystem::PreUpdate(double deltaTime)
{
	MoveDirection *= 0.0f;
	MoveDirection = XForce + YForce + ZForce;
}

void PlayerControlSystem::Update(double deltaTime, unsigned int entity)
{
	m_CManager->GetComponent<PlayerControl>(entity)->Move(MoveDirection, deltaTime);
	return;
}

void PlayerControlSystem::KeyDown(unsigned char key)
{
	switch (key)
	{
	case 'W':
		XForce.z = 0.3f;
		break;
	case 'A':
		ZForce.x = -0.3f;
		break;
	case 'S':
		XForce.z = -0.3f;
		break;
	case'D':
		ZForce.x = 0.3f;
		break;

	}
}

void PlayerControlSystem::KeyUp(unsigned char key)
{
	switch (key)
	{
	case 'W':
		XForce.z = 0.0f;
		break;
	case 'A':
		ZForce.x = 0.0f;
		break;
	case 'S':
		XForce.z = 0.0f;
		break;
	case'D':
		ZForce.x = 0.0f;
		break;

	}
}