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

	void SystemManager::UpdateUserSystems(double deltaTime)
	{
		for (unsigned int i = 0; i < UserSystemList.size(); ++i)
		{
			UserSystemList[i]->UpdateSystem(deltaTime);
		}
	}

	void SystemManager::RegisterEntity(Entity& entity)
	{
		for each(BaseSystem* system in SystemList)
		{
			system->RegisterEntity(entity);
		}
		for each(BaseSystem* system in UserSystemList)
		{
			system->RegisterEntity(entity);
		}
		for each(BaseSystem* system in CoreSystemList)
		{
			system->RegisterEntity(entity);
		}
	}

	void SystemManager::RemoveUserSystems()
	{
		for each (BaseSystem* system in UserSystemList)
		{
			delete system;
		}

		UserSystemList.clear();
	}

	void SystemManager::ClearAllRegisteredEntities()
	{
		for each(BaseSystem* sys in SystemList)
		{
			sys->ClearRegisteredEntities();
		}
		for each(BaseSystem* sys in UserSystemList)
		{
			sys->ClearRegisteredEntities();
		}
		for each(BaseSystem* sys in CoreSystemList)
		{
			sys->ClearRegisteredEntities();
		}
	}
	void SystemManager::StartSystems()
	{
		for each(BaseSystem* system in SystemList)
		{
			system->StartSystem();
		}
		for each(BaseSystem* system in UserSystemList)
		{
			system->StartSystem();
		}
		for each(BaseSystem* system in CoreSystemList)
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
		for each(BaseSystem* system in UserSystemList)
		{
			system->StopSystem();
		}
		for each(BaseSystem* system in CoreSystemList)
		{
			system->StopSystem();
		}

	}

	void SystemManager::UpdateGizmos(Entity& entity)
	{
		for each(BaseSystem* system in SystemList)
		{
			if (system->IsEntityRegistered(entity))
				system->DrawGizmo(entity);
		}
		for each(BaseSystem* system in UserSystemList)
		{
			if (system->IsEntityRegistered(entity))
				system->DrawGizmo(entity);
		}
		for each(BaseSystem* system in CoreSystemList)
		{
			if (system->IsEntityRegistered(entity))
				system->DrawGizmo(entity);
		}
	}
}
