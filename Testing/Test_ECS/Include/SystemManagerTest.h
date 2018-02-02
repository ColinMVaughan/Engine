#ifndef SYS_MANAGER_TEST_H
#define SYS_MANAGER_TEST_H

#include <gtest/gtest.h>
#include "SystemManager.h"


class TestSystem : public ECS::System<bool, int>
{
public:
	TestSystem(ECS::ComponentManager* a_cmanager, EventManager& a_emanager) 
	: System(a_cmanager, a_emanager){}

	void Update(double deltaTime, ECS::Entity& entity) override
	{
	}

};


class TestSystem2 : public ECS::System<bool>
{
public:
	TestSystem2(ECS::ComponentManager* a_cmanager, EventManager& a_emanager)
		: System(a_cmanager, a_emanager) {}

	void Update(double deltaTime, ECS::Entity& entity) override
	{
	}

};

//---------------------------------------------------
//					TESTS
//---------------------------------------------------

TEST(SystemManagerTest, AddSystem)
{

}

TEST(SystemManagerTest, DoesUpdateSystems)
{

}

TEST(SystemManagerTest, UpdatesCorrectEntities)
{

}

#endif