#pragma once
#include <string>

class PrefabEntity
{
public:
	PrefabEntity(std::string a_Filepath) :m_Filepath(a_Filepath){}
	PrefabEntity() {}
	std::string m_Filepath;
};
