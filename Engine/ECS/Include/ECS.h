#ifndef ECS_H
#define ECS_H

#include <memory>
#include <vector>
#include "SystemManager.h"
#include "ComponentManager.h"

namespace ECS
{
	class Scene;
//---------------------------------------------------------
//						ENTITY
//	Entities act as the Game Objects within a scene.
//	Entities must allways be created and destroyed by a Scene
//	An Entity that is not created with a scene is considered
//	invalid and will produce undefined behaviour.
//
//----------------------------------------------------------
	class Entity
	{
	public:
		//Entity requires a unique ID and a pointer to the scene it belongs to
		Entity(unsigned int a_id, Scene* a_scene) :m_ID(a_id), m_Scene(a_scene) {}

		unsigned int GetID() { return m_ID; }

		template<typename T>
		T* GetComponent();

		template<typename T>
		T* AddComponent();

		template<typename T>
		void RemoveComponent();

		void DestroyEntity();
		void DisableEntity();
		void EnableEntity();

	private:
		unsigned int m_ID;
		Scene* m_Scene;
	};

//-------------------------------------------------
//					Scene			Colin Vaughan
//
// Scene class is used to create entities and connect
// the system and component managers.
// All entities created are tied to it's scene.
//--------------------------------------------------
	class Scene
	{

	public:
		Scene(SystemManager* a_systemMgr, ComponentManager* a_compMgr)
			: m_SystemManager(a_systemMgr), m_ComponentManager(a_compMgr) {}

		Entity CreateEntity();
		void DestroyEntity(Entity a_entity);

		template<typename T>
		T* AddComponent(Entity a_entity);

		template<typename T>
		T* AddSystem();

		template<typename T>
		T* GetComponent(Entity a_entity);

		template<typename T>
		bool HasComponent(Entity a_entity);

		void UpdateSystems(double deltaTime);
		unsigned int GetNumEntities();
		Entity GetEntity(unsigned int index);

		std::vector<std::string> m_EntityNames;
	private:
		SystemManager*    m_SystemManager;
		ComponentManager* m_ComponentManager;

		unsigned int EntityCounter = 0;
		std::vector<Entity> m_EntityList;
	};






//-------------------------------------------------------------------------------
//				SCENE IMPLEMENTATION
//------------------------------------------------------------------------------

	//TODO: Optimize getting component after adding. currenty pretty Inelegant
	template <typename T>
	T* Scene::AddComponent(Entity a_entity)
	{
		m_ComponentManager->AddComponent<T>(a_entity.GetID());
		return m_ComponentManager->GetComponent<T>(a_entity.GetID());
	}

	template <typename T>
	T* Scene::AddSystem()
	{
		return m_SystemManager->AddSystem<T>();
	}

	template <typename T>
	T* Scene::GetComponent(Entity a_entity)
	{
		return m_ComponentManager->GetComponent<T>(a_entity.GetID());
	}

	template<typename T>
	inline bool Scene::HasComponent(Entity a_entity)
	{
		return m_ComponentManager->HasComponents<T>(a_entity.GetID());
	}


	inline Entity Scene::CreateEntity()
	{
		EntityCounter++;
		m_EntityList.push_back(Entity(EntityCounter, this));
		return Entity(EntityCounter, this);
	}

	inline void Scene::UpdateSystems(double deltaTime)
	{
		m_SystemManager->UpdateSystems(deltaTime, m_EntityList.data(), m_EntityList.size());
	}

	inline unsigned int Scene::GetNumEntities()
	{
		return m_EntityList.size();
	}
	inline Entity Scene::GetEntity(unsigned int index)
	{
		return m_EntityList[index];
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



//-----------------------------------------------------------
//				ENTITY IMPLEMENTATION
//-----------------------------------------------------------
	template<typename T>
	T* Entity::AddComponent()
	{
		return m_Scene->AddComponent<T>(*this);
	}

	template<typename T>
	T* Entity::GetComponent()
	{
		return m_Scene->GetComponent<T>(*this);
	}


}
#endif //ECS_H