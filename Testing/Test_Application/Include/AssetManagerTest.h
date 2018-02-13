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
	AssetManager manager;
	manager.AddResourceType<SomeAsset>("SomeAsset");
	manager.AddResource("SomeAsset", "MyFilepath");

	AssetRequestEvent request;
	request.m_AssetTypeName = "SomeAsset";
	request.m_AssetName = "name";

	manager.HandleAssetRequestEvent(&request);
	EXPECT_EQ(request.m_AssetName, "Found Asset");
}

TEST(AssetManagerTest, RemoveAsset)
{

}
#endif