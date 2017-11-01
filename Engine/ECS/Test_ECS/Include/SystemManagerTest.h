#ifndef SYS_MANAGER_TEST_H
#define SYS_MANAGER_TEST_H

#include <gtest/gtest.h>
#include "SystemManager.h"


class TestSystem : public System<bool, int>
{
public:
	TestSystem(ComponentManager* a_cmanager) 
	: System(a_cmanager){}

	void Update(unsigned int entity) override
	{
		bool* updated = m_CManager->GetComponent<bool>(entity);
		*updated = true;
		
		HasUpdated = true;
	}

	bool HasUpdated = false;
};


class TestSystem2 : public System<bool>
{
public:
	TestSystem2(ComponentManager* a_cmanager)
		: System(a_cmanager) {}

	void Update(unsigned int entity) override
	{
		//bool* updated = m_CManager->GetComponent<bool>(entity);
		//*updated = true;

		HasUpdated = true;
	}

	bool HasUpdated = false;
};

//---------------------------------------------------
//					TESTS
//---------------------------------------------------

TEST(SystemManagerTest, AddSystem)
{
	ComponentManager CManager;
	SystemManager SysManager(&CManager);

	TestSystem* testSys = SysManager.AddSystem<TestSystem>();

	EXPECT_TRUE(testSys != nullptr);
}

TEST(SystemManagerTest, DoesUpdateSystems)
{
	ComponentManager CManager;
	SystemManager SysManager(&CManager);

	TestSystem* testSys = SysManager.AddSystem<TestSystem>();
	CManager.AddComponent<bool>(0);
	CManager.AddComponent<int>(0);


	//SysManager.UpdateSystems(0.0, 0);


	EXPECT_TRUE(testSys->HasUpdated);
	EXPECT_TRUE(*CManager.GetComponent<bool>(0));
}

TEST(SystemManagerTest, UpdatesCorrectEntities)
{
	ComponentManager CManager;
	SystemManager SysManager(&CManager);

	SysManager.AddSystem<TestSystem>();

	//SysManager.UpdateSystems(0.0, 0);
	//SysManager.UpdateSystems(0.0, 1);
	//SysManager.UpdateSystems(0.0, 2);


	CManager.AddComponent<bool>(0);
	CManager.AddComponent<bool>(1);
	CManager.AddComponent<int>(1);
	CManager.AddComponent<bool>(2);

	EXPECT_TRUE(*CManager.GetComponent<bool>(1));
	EXPECT_FALSE(*CManager.GetComponent<bool>(0));
	EXPECT_FALSE(*CManager.GetComponent<bool>(2));
}

#endif