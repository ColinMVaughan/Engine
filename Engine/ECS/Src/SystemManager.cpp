#include "SystemManager.h"
#include "ECS.h"

namespace ECS
{
	//-------------------------------
	// Loop through each system and update its corrisponding entities.
	//We need to figure out how to efficiently register systems with componets
	//------------------------------
	void SystemManager::UpdateSystems(double deltaTime, Entity entityList[], size_t entityCount)
	{
		for (unsigned int i = 0; i < SystemList.size(); ++i)
		{
			SystemList[i]->UpdateSystem(deltaTime);
		}
	}

	void SystemManager::UpdateCoreSystems(double deltaTime)
	{
		for (unsigned int i = 0; i < CoreSystemList.size(); ++i)
		{
			CoreSystemList[i]->UpdateSystem(deltaTime);
		}
	}

	void SystemManager::RegisterEntity(Entity& entity)
	{
		for each(BaseSystem* system in SystemList)
		{
			system->RegisterEntity(entity);
		}
	}
	void SystemManager::StartSystems()
	{
		for each(BaseSystem* system in SystemList)
		{
			system->StartSystem();
		}
	}
	void SystemManager::StopSystems()
	{
		for each(BaseSystem* system in SystemList)
		{
			system->StopSystem();
		}
	}
}
