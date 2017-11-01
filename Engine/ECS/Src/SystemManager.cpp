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
			SystemList[i]->PreUpdate(deltaTime);

			for (unsigned int count = 0; count < entityCount; ++count)
			{
				if (SystemList[i]->HasComponents(entityList[count].GetID()))
					SystemList[i]->Update(deltaTime, entityList[count]);
			}

			SystemList[i]->PostUpdate(deltaTime);
		}
	}
}
