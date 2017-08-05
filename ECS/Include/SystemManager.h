#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

//--------------------------------------------------------------
//					Colin Vaughan 
//					June 5th, 2017
//
// The System Manager is used to Contain and update any added Systems.
// Every cycle, each system is looped through and their update nethod called,
// while passing only entities that have the required components.
//--------------------------------------------------------------


#include "ComponentManager.h"
#include "BaseSystem.h"

class SystemManager
{
public:
	SystemManager(ComponentManager* cManager): m_ComponentManager(cManager) {}

	template<typename T>
	T* AddSystem()
	{
		//static_assert(std::is_base_of<BaseSystem, T>::value, "");
		T* returnPtr = new T(m_ComponentManager);

		BaseSystem* tempPtr = returnPtr;
		SystemList.push_back(tempPtr);

		return returnPtr;
	}

	//-------------------------------
	// Loop through each system and update its corrisponding entities.
	//We need to figure out how to efficiently register systems with componets
	//------------------------------
	void UpdateSystems(double deltaTime, unsigned int entityList[], size_t entityCount)
	{
		for (int i = 0; i < SystemList.size(); ++i)
		{
			SystemList[i]->PreUpdate();

			for (unsigned int count = 0; count < entityCount; ++count)
			{
				if(SystemList[i]->HasComponents(entityList[count]))
					SystemList[i]->Update(entityList[count]);
			}

			SystemList[i]->PostUpdate();
		}
	}

	
private:


	ComponentManager* m_ComponentManager;
	std::vector<BaseSystem*> SystemList;
};

#endif