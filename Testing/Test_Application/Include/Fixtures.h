#pragma once
#include <gtest\gtest.h>
#include <AssetManager.h>
#include <string>

//-----------------------------------------------
//				TEST ASSETS
//----------------------------------------------
class SomeAsset
{
public:
	void LoadAsset(std::string filePath)
	{
		m_FilePath = filePath;
		Index = 1;
	}

	void UnloadAsset()
	{
		m_FilePath = "";
		Index = 0;
	}

public:
	int Index = 0;
	std::string m_FilePath;
};


//-----------------------------------------------
//				FIXTURES
//-----------------------------------------------

//class AssetManagerTest : public ::testing::Test
//{
//public:
//	AssetManagerTest()
//	{
//		Manager.AddResourceType<SomeAsset>("SomeAsset");
//	}
//
//	~AssetManagerTest()
//	{
//
//	}
//
//public:
//	AssetManager m_Manager;
//};