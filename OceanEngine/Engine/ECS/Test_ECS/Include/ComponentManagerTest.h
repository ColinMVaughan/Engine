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
		ComponentManager CManager;
		CManager.AddComponent<int>(0);
		CManager.AddComponent<bool>(0);
		
		bool result = CManager.HasComponents<int, bool>(0);
		EXPECT_TRUE(result);
	}
	//------------------------------------------
	{
		ComponentManager CManager;
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
		ComponentManager CManager;
		CManager.AddComponent<bool>(0);

		bool* result = CManager.GetComponent<bool>(0);
		*result = true;

		EXPECT_TRUE(*result);
	}

	{
		ComponentManager CManager;
		CManager.AddComponent<bool>(0);

		bool* result = CManager.GetComponent<bool>(0);
		*result = false;

		EXPECT_FALSE(*result);
	}
}

TEST(ComponentManagerTest, RemoveComponent)
{
	EXPECT_TRUE(false);
}

TEST(ComponentManagerTest, GetComponent)
{
	EXPECT_TRUE(false);
}

#endif