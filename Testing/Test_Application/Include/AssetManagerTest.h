#ifndef _ASSET_MANAGER_TESTS_H_
#define _ASSET_MANAGER_TESTS_H_

#include <gtest\gtest.h>
#include <AssetManager.h>
#include "Fixtures.h"


//Tests that Resources can be added to the AssetManager
TEST(AssetManagerTest, AddResourceType)
{
	AssetManager manager;
	manager.AddResourceType<SomeAsset>("SomeAsset");
}

//Check that existing assets can be Retrieved and copied
TEST(AssetManagerTest, RetrieveAsset)
{

}

TEST(AssetManagerTest, RemoveAsset)
{

}
#endif