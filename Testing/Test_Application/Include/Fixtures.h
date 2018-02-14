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

	SomeAsset() = default;

	SomeAsset(std::string a_filePath)
	{
		m_FilePath = a_filePath;
		m_Index = 1;
	}

	void UnloadAsset()
	{
		m_FilePath = "";
		m_Index = 0;
	}

public:
	int m_Index = 0;
	std::string m_FilePath;
};
