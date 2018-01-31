#ifndef COMPONENT_MANAGER_TEST_H
#define COMPONENT_MANAGER_TEST_H


#include <gtest\gtest.h>
#include <ComponentManager.h>


//-----------------------------------------------
// Checks if the entity has the required components, returns true if it does.
// Can take an arbitrary number of template arguments (required components)
//----------------------------------------------
TEST(ComponentManagerTest, HasComponent)
{
	//----------------------------------------------
	{
		ECS::ComponentManager CManager;
		CManager.AddComponent<int>(0);
		CManager.AddComponent<bool>(0);
		
		bool result = CManager.HasComponents<int, bool>(0);
		EXPECT_TRUE(result);
	}
	//------------------------------------------
	{
		ECS::ComponentManager CManager;
		CManager.AddComponent<int>(0);

		bool Result = CManager.HasComponents<int, bool>(0);
		EXPECT_FALSE(Result);
	}

}


//---------------------------------------------
// Tests Adding components to the manager. The manager determines the correct pool to add the component to,
// and adds it if the pool exists. If not it creates the pool.
//
// Returns pointer to created component.
//--------------------------------------------
TEST(ComponentManagerTest, AddComponent)
{
	{
		ECS::ComponentManager CManager;
		CManager.AddComponent<bool>(0);

		bool* result = CManager.GetComponent<bool>(0);
		*result = true;

		EXPECT_TRUE(*result);
	}

	{
		ECS::ComponentManager CManager;
		CManager.AddComponent<bool>(0);

		bool* result = CManager.GetComponent<bool>(0);
		*result = false;

		EXPECT_FALSE(*result);
	}
}

TEST(ComponentManagerTest, RemoveComponent)
{
	ECS::ComponentManager CManager;

	CManager.AddComponent<bool>(0);
	EXPECT_TRUE(CManager.HasComponents<bool>(0));

	CManager.RemoveComponent<bool>(0);
	EXPECT_FALSE(CManager.HasComponents<bool>(0));
}

TEST(ComponentManagerTest, GetComponent)
{
	ECS::ComponentManager cManager;
	cManager.AddComponent<int>(0);

	{
		int* val = cManager.GetComponent<int>(0);
		*val = 1234;
	}

	int* newVal = cManager.GetComponent<int>(0);

	EXPECT_EQ(*newVal, 1234);
}

#endif