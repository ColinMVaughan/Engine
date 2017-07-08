#ifndef ECS_TEST_H
#define ECS_TEST_H

#include <gtest/gtest.h>
#include "ECS.h"


TEST(ECSTest, CreateEntity)
{
	//ComponentManager* CompMgr = new ComponentManager;
	//SystemManager* SysMgr     = new SystemManager(CompMgr);
	//
	//ECS Master(SysMgr, CompMgr);
	//Entity entity1 = Master.CreateEntity();
	//Entity entity2 = Master.CreateEntity();
	//
	//EXPECT_TRUE(entity2.GetID() != 0);
}


TEST(EcsTest, DestroyEntity)
{
	//ComponentManager* CompMgr = new ComponentManager;
	//SystemManager* SysMgr = new SystemManager(CompMgr);
	//
	//ECS Master(SysMgr, CompMgr);
	//
	//Entity entity1 = Master.CreateEntity();
	//Entity entity2 = Master.CreateEntity();
	//
	//Master.AddComponent<int>(entity1);
	//Master.AddComponent<bool>(entity1);
	//
	//
	//Master.DestroyEntity(entity1);
	//
	//
	//EXPECT_TRUE(false);
}

#endif