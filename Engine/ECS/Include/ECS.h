#ifndef ECS_H
#define ECS_H

#include <memory>
#include <vector>
#include "SystemManager.h"
#include "ComponentManager.h"
#include <cereal\cereal.hpp>
#include <cereal\archives\binary.hpp>

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
		Entity(unsigned int a_id, std::string a_name, Scene* a_scene) :m_ID(a_id), m_Name(a_name), m_Scene(a_scene) {}

		unsigned int GetID() { return m_ID; }

		template<typename T>
		T* GetComponent();

		template<typename T>
		T* AddComponent();

		template<typename T>
		void RemoveComponent();

		template<typename EventType>
		void DispatchEvent(EventType& a_event);

		unsigned int GetNumComponents();

		void SetName(std::string a_name);
		std::string GetName();

		void DestroyEntity();
		void DisableEntity();
		void EnableEntity();

	private:
		std::string m_Name;
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
		Scene(SystemManager* a_systemMgr, ComponentManager* a_compMgr, EventManager* a_eveMgr)
			: m_SystemManager(a_systemMgr), m_ComponentManager(a_compMgr), m_EventManager(a_eveMgr) {}

		Scene()
		{
			m_EventManager = new EventManager;
			m_ComponentManager = new ComponentManager;
			m_SystemManager = new SystemManager(m_ComponentManager, m_EventManager);

		}

		Entity& CreateEntity();
		Entity& CreateEntity(std::string a_name);
		void DestroyEntity(Entity& a_entity);

		template<typename T>
		T* AddComponent(Entity a_entity);

		template<typename T>
		T* AddSystem();

		template<typename T>
		T* GetComponent(Entity a_entity);

		template<typename T>
		bool HasComponent(Entity a_entity);

		template<typename T>
		bool SerializeComponent(Entity a_entity);

		template <typename T>
		bool UnserializeComponent(Entity a_entity);

		template<typename EventType>
		void DispatchEvent(EventType& a_event);

		bool SaveScene(std::string filePath);
		bool LoadScene(std::string filePath);

		void UpdateSystems(double deltaTime);
		unsigned int GetNumEntities();
		Entity& GetEntity(unsigned int index);
		EventManager& GetEventManager();

		std::vector<std::string> m_EntityNames;
		cereal::BinaryOutputArchive* m_BinaryOutput;
		cereal::BinaryInputArchive* m_BinaryInput;

	private:
		SystemManager*    m_SystemManager;
		ComponentManager* m_ComponentManager;
		EventManager*	  m_EventManager;

		unsigned int EntityCounter = 0;
		std::vector<Entity> m_EntityList;
		std::string m_SceneName;

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

	template<typename T>
	inline bool Scene::SerializeComponent(Entity a_entity)
	{
		//Get Component
		T* component = GetComponent<T>(a_entity);

		//if the component exists, and the binary output exists
		if (component && m_BinaryOutput)
		{
			//m_BinaryOutput(component);
			m_BinaryOutput->operator()(*component);
			return true;
		}

		return false;
	}

	template<typename T>
	inline bool Scene::UnserializeComponent(Entity a_entity)
	{
		//Get Component
		T* component = GetComponent<T>(a_entity);

		//if the component exists, and the binary output exists
		if (component && m_BinaryInput)
		{
			//m_BinaryInput(component);
			m_BinaryInput->operator()(*component);
			return true;
		}

		return false;
	}

	template<typename EventType>
	inline void Scene::DispatchEvent(EventType & a_event)
	{
		m_EventManager->DispatchEvent<EventType>(a_event);
	}


	inline Entity& Scene::CreateEntity()
	{
		EntityCounter++;
		m_EntityList.push_back(Entity(EntityCounter,"Entity " + std::to_string(EntityCounter), this));
		return m_EntityList.back();
	}

	inline Entity& Scene::CreateEntity(std::string a_name)
	{
		EntityCounter++;
		m_EntityList.push_back(Entity(EntityCounter, a_name, this));
		return m_EntityList.back();
	}

	inline void Scene::DestroyEntity(Entity& a_entity)
	{
		//Free all components associated with the Entity
		m_ComponentManager->RemoveAllComponents(a_entity.GetID());

		//Remove Entity from EntityList

		//Set Entity's ID to zero and scene pointer to null.
		//this denotes that the entity is no longer available.
		a_entity = Entity(0,"", nullptr);
	}

	inline void Scene::UpdateSystems(double deltaTime)
	{
		m_SystemManager->UpdateSystems(deltaTime, m_EntityList.data(), m_EntityList.size());
	}

	inline unsigned int Scene::GetNumEntities()
	{
		return m_EntityList.size();
	}

	inline Entity& Scene::GetEntity(unsigned int index)
	{
		return m_EntityList[index];
	}


//-----------------------------------------------------------
//				ENTITY IMPLEMENTATION
//-----------------------------------------------------------
	template<typename T>
	T* Entity::AddComponent()
	{
		return m_Scene->AddComponent<T>(*this);
	}

	template<typename T>
	inline void Entity::RemoveComponent()
	{
	}

	template<typename EventType>
	inline void Entity::DispatchEvent(EventType & a_event)
	{
		m_Scene->DispatchEvent<EventType>(a_event);
	}

	template<typename T>
	T* Entity::GetComponent()
	{
		return m_Scene->GetComponent<T>(*this);
	}

	inline void Entity::SetName(std::string a_name)
	{
		m_Name = a_name;
	}
	inline std::string Entity::GetName()
	{
		return m_Name;
	}
}
#endif //ECS_H