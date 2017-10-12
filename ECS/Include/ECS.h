#ifndef ECS_H
#define ECS_H



#include <memory>
#include "SystemManager.h"
#include "ComponentManager.h"


namespace ECS
{

	//-------------------------------------------------
	//					ECS				Colin Vaughan
	//
	// ECS class is used to create entities and connect
	// the system and component managers.
	//--------------------------------------------------
	struct Entity
	{
	public:
		Entity(unsigned int a_id) :m_ID(a_id) {}
		unsigned int GetID() { return m_ID; }

	private:
		unsigned int m_ID;
	};


	class ECS
	{

	public:
		ECS(SystemManager* a_systemMgr, ComponentManager* a_compMgr)
			: m_SystemManager(a_systemMgr), m_ComponentManager(a_compMgr) {}

		Entity CreateEntity();
		void DestroyEntity(Entity a_entity);

		template<typename T>
		T* AddComponent(Entity a_entity);

		template<typename T>
		T* AddSystem();

		template<typename T>
		T* GetComponent(Entity a_entity);

		void UpdateSystems(double deltaTime);

	private:
		SystemManager*    m_SystemManager;
		ComponentManager* m_ComponentManager;

		unsigned int EntityCounter = 0;
		std::vector<unsigned int> m_EntityList;
	};




	//					IMPLEMENTATION
	//------------------------------------------------------------------------------

	//TODO: Optimize getting component after adding. currenty pretty Inelegant
	template <typename T>
	T* ECS::AddComponent(Entity a_entity)
	{
		m_ComponentManager->AddComponent<T>(a_entity.GetID());
		return m_ComponentManager->GetComponent<T>(a_entity.GetID());
	}

	template <typename T>
	T* ECS::AddSystem()
	{
		return m_SystemManager->AddSystem<T>();
	}

	template <typename T>
	T* ECS::GetComponent(Entity a_entity)
	{
		return m_ComponentManager->GetComponent<T>(a_entity.GetID());
	}


	inline Entity ECS::CreateEntity()
	{
		EntityCounter++;
		m_EntityList.push_back(EntityCounter);
		return Entity(EntityCounter);
	}

	inline void ECS::UpdateSystems(double deltaTime)
	{
		m_SystemManager->UpdateSystems(deltaTime, m_EntityList.data(), m_EntityList.size());
	}

	////Destroys all components associated with the entity and remove it from the list
	//inline void ECS::DestroyEntity(Entity a_entity)
	//{
	//}
	//
	//template <typename T>
	//std::shared_ptr<T> ECS::AddComponent(Entity a_entity)
	//{
	//	return 0;
	//}
	//
	//template <typename T>
	//std::shared_ptr<T> ECS::AddSystem()
	//{
	//	return m_SystemManager->AddSystem<T>();
	//}

}

#endif //ECS_H