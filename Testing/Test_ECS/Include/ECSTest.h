#ifndef ECS_TEST_H
#define ECS_TEST_H

#include <gtest/gtest.h>
#include "ECS.h"
#include "Fixtures.h"


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

}

#endif