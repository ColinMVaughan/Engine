#pragma once
#include <gtest\gtest.h>
#include <ECS.h>

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