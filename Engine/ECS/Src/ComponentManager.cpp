#include "ComponentManager.h"
namespace ECS
{

	ComponentManager::ComponentManager()
	{
	}

	//Deleting raw pointers here, remove when moving to smart pointers
	ComponentManager::~ComponentManager()
	{
		for (auto it = PoolMap.begin(); it != PoolMap.end(); it++)
		{
			delete it->second;
		}
	}
	

	void ComponentManager::RemoveAllComponents(unsigned int entity)
	{
		for (auto it = PoolMap.begin(); it != PoolMap.end(); it++)
		{
			if (it->second->HasComponent(entity))
			{
				it->second->RemoveComponent(entity);
			}
		}
	}
}
