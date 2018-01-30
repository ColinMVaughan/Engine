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


#endif