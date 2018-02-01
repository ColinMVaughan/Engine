#ifndef ECS_TEST_H
#define ECS_TEST_H

#include <gtest/gtest.h>
#include "ECS.h"
#include "Fixtures.h"
#include <string>

TEST_F(SceneTest, CreateEntity)
{
	EXPECT_EQ(m_Scene->GetNumEntities(), 0);
	m_Scene->CreateEntity();
	m_Scene->CreateEntity();
	m_Scene->CreateEntity();

	EXPECT_EQ(m_Scene->GetNumEntities(), 3);
	auto entity = m_Scene->GetEntity(2);
	
	EXPECT_GT(entity.GetID(), 0);
}


TEST_F(SceneTest, DestroyEntity)
{
	m_Scene->CreateEntity();
	m_Scene->CreateEntity();
	m_Scene->CreateEntity();
	EXPECT_EQ(m_Scene->GetNumEntities(), 3);

	auto entity = m_Scene->GetEntity(2);
	//m_Scene->DestroyEntity(entity);
	EXPECT_EQ(m_Scene->GetNumEntities(), 2);

}

TEST_F(SceneTest, AddComponent)
{
	auto Entity = m_Scene->CreateEntity();
	m_Scene->AddComponent<std::string>(Entity);
	m_Scene->AddComponent<int>(Entity);

	EXPECT_TRUE(m_Scene->HasComponent<std::string>(Entity));
	EXPECT_TRUE(m_Scene->HasComponent<int>(Entity));
	EXPECT_FALSE(m_Scene->HasComponent<float>(Entity));

}

//Test the saving and loading of components into the scene.
TEST_F(SceneTest, SaveAndLoadComp)
{
	auto ent1 = m_Scene->CreateEntity();
	auto ent2 = m_Scene->CreateEntity();
	auto ent3 = m_Scene->CreateEntity();

	m_Scene->AddComponent<TestComp1>(ent1);
	m_Scene->AddComponent<TestComp2>(ent2);

	m_Scene->AddComponent<TestComp1>(ent3);
	m_Scene->AddComponent<TestComp2>(ent3);

	m_Scene->SaveScene("./TestingFiles/TestScene.Scene");

	{
		EventManager eMan;
		ECS::ComponentManager cMan;
		ECS::SystemManager sMan(&cMan, &eMan);
		ECS::Scene loadedScene(&sMan, &cMan);

		loadedScene.LoadScene("./TestingFiles/TestScene.Scene");

		auto loadedEnt1 = loadedScene.GetEntity(0);
		auto loadedEnt2 = loadedScene.GetEntity(1);
		auto loadedEnt3 = loadedScene.GetEntity(2);


		EXPECT_TRUE(loadedScene.HasComponent<TestComp1>(loadedEnt1));
		EXPECT_TRUE(loadedScene.HasComponent<TestComp2>(loadedEnt2));

		EXPECT_TRUE(loadedScene.HasComponent<TestComp1>(loadedEnt3));
		EXPECT_TRUE(loadedScene.HasComponent<TestComp2>(loadedEnt3));
	}
}

#endif