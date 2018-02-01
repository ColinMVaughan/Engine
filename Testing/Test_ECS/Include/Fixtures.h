#pragma once
#include <gtest\gtest.h>
#include <ECS.h>
#include <ComponentReflection.h>

//--------------------------------------------------------------------
//						Dummy Components
//-------------------------------------------------------------------

class TestComp1
{
public:
	COMPONENT_SERIALIZE(value)

public:
	int value = 0;
};
COMPONENT_REGISTER(TestComp1, "TestComp1")

class TestComp2
{
public:
	COMPONENT_SERIALIZE(value2)
public:
	int value2 = 0;
};
COMPONENT_REGISTER(TestComp2, "TestComp2")

//--------------------------------------------------------------------
//						Dummy Systems
//-------------------------------------------------------------------




//---------------------------------------------------------------------
//							FIXTURES
//--------------------------------------------------------------------

class SceneTest : public ::testing::Test
{
public:
	void SetUp()
	{
		//Allocate and set up Scene and related managers;
		m_EveManager = new EventManager;
		m_CompManager = new ECS::ComponentManager;
		m_SysManager = new ECS::SystemManager(m_CompManager, m_EveManager);
		m_Scene = new ECS::Scene(m_SysManager, m_CompManager);
	}

	void TearDown()
	{
		//Destroy all ECS managers
		delete m_Scene;
		delete m_SysManager;
		delete m_CompManager;
		delete m_EveManager;
	}

public:
	ECS::Scene* m_Scene;
	ECS::SystemManager* m_SysManager;
	ECS::ComponentManager* m_CompManager;
	EventManager* m_EveManager;
};