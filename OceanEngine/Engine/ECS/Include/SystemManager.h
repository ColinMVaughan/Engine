#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <vector>

#include "ComponentManager.h"
#include "BaseSystem.h"
#include "EventManager.h"

namespace ECS
{
	class Entity;
	//--------------------------------------------------------------
	//					Colin Vaughan 
	//					June 5th, 2017
	//
	// The System Manager is used to Contain and update any added Systems.
	// Every cycle, each system is looped through and their update nethod called,
	// while passing only entities that have the required components.
	//--------------------------------------------------------------

	class SystemManager
	{
	public:
		SystemManager(ComponentManager* cManager, EventManager* eManager) : m_ComponentManager(cManager), m_EventManager(eManager) {}

		template<typename T>
		T* AddSystem()
		{
			//static_assert(std::is_base_of<BaseSystem, T>::value, "");
			T* returnPtr = new T(m_ComponentManager, *m_EventManager);

			BaseSystem* tempPtr = returnPtr;
			SystemList.push_back(tempPtr);

			return returnPtr;
		}

		template<typename T>
		T* AddCoreSystem()
		{
			T* returnPtr = new T(m_ComponentManager, *m_EventManager);
			BaseSystem* tempPtr = returnPtr;
			
			CoreSystemList.push_back(tempPtr);
			return returnPtr;
		}

		template<typename T>
		T* AddUserSystem()
		{
			T* returnPtr = new T(m_ComponentManager, *m_EventManager);
			BaseSystem* tempPtr = returnPtr;

			UserSystemList.push_back(tempPtr);

			return returnPtr;
		}

		//-------------------------------
		// Loop through each system and update its corrisponding entities.
		//We need to figure out how to efficiently register systems with componets
		//------------------------------
		void UpdateSystems(double deltaTime, Entity entityList[], size_t entityCount);
		void UpdateCoreSystems(double deltaTime);
		void UpdateUserSystems(double deltaTime);

		void RegisterEntity(Entity& entity);

		void RemoveUserSystems();
		void ClearAllRegisteredEntities();

		void StartSystems();
		void StopSystems();

		void UpdateGizmos(Entity& entity);

		ComponentManager* m_ComponentManager;
		EventManager* m_EventManager;
	private:

		std::vector<BaseSystem*> SystemList;
		std::vector<BaseSystem*> CoreSystemList;
		std::vector<BaseSystem*> UserSystemList;
	};

}
#endif