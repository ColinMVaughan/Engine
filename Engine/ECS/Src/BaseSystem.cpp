#include <BaseSystem.h>
#include <ECS.h>


void ECS::BaseSystem::RegisterEntity(Entity& a_Entity)
{
	//check that entity has required components
	if (!HasComponents(a_Entity.GetID()))
		return;

	//check that entity is not already registered
	for each(Entity registeredEnt in m_RegisteredEntities)
	{
		if (a_Entity.GetID() == registeredEnt.GetID())
			return;
	}

	//add entity to vector and call user defined registration funciton
	m_RegisteredEntities.push_back(a_Entity);
	EntityRegistered(a_Entity);
}


void ECS::BaseSystem::UpdateSystem(double deltaTime)
{
	PreUpdate(deltaTime);
	for each(Entity entity in m_RegisteredEntities)
	{
		Update(deltaTime, entity);
	}
	PostUpdate(deltaTime);
}

void ECS::BaseSystem::StartSystem()
{
	PreStart();
	for each(Entity entity in m_RegisteredEntities)
	{
		Start(entity);
	}
	PostStart();
}

void ECS::BaseSystem::StopSystem()
{
	PreStop();
	for each(Entity entity in m_RegisteredEntities)
	{
		Stop(entity);
	}
	PostStop();
}
