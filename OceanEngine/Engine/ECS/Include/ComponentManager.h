#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include "ComponentPool.h"
#include <map>
#include <vector>
#include <utility>

namespace ECS
{
	//------------------------------------------
	// Holds a list of componet pools & 
	// accesses components based on entity value
	//--------------------------------------------

	class ComponentManager
	{
	public:

		ComponentManager();
		~ComponentManager();


		//Adds a componet to its corrisponding pool. 
		//Component must be derrived from BaseComponent
		template<typename T>
		void AddComponent(unsigned int entity);

		template<typename T>
		T* GetComponent(unsigned int entity);

		template<typename T>
		void RemoveComponent(unsigned int entity);

		void RemoveAllComponents(unsigned int entity);

		void ClearAllComponents();

		//--------------
		template<typename First>
		bool HasComponents(unsigned int entity);


		template<typename First, typename Second, typename... Components>
		bool HasComponents(unsigned int entity);
		//--------------

	private:
		std::map<size_t, BaseComponentPool*> PoolMap;
	};

	//---------------------------------------------------
	//				IMPLEMENTATION
	//---------------------------------------------------


	template<typename T>
	void ComponentManager::AddComponent(unsigned int entity)
	{
		//if the component pool does not exitst, add a new one.
		auto iterator = PoolMap.find(typeid(T).hash_code());
		if (iterator == PoolMap.end())
		{
			BaseComponentPool* temp = new ComponentPool<T>();
			PoolMap.insert(std::map<size_t, BaseComponentPool*>::value_type(typeid(T).hash_code(), temp));
		}

		//Add componet to the correct pool
		BaseComponentPool* temp = PoolMap.at(typeid(T).hash_code());
		ComponentPool<T>* pool = static_cast<ComponentPool<T>*>(temp);
		pool->AddComponent(entity);
	}


	template<typename T>
	T* ComponentManager::GetComponent(unsigned int entity)
	{
		std::map<size_t,BaseComponentPool*>::iterator temp = PoolMap.find(typeid(T).hash_code());

		if (temp == PoolMap.end())
			return nullptr;

		ComponentPool<T>* pool = static_cast<ComponentPool<T>*>(temp->second);
		return pool->GetComponent(entity);
	}


	template<typename T>
	inline void ComponentManager::RemoveComponent(unsigned int entity)
	{
		BaseComponentPool* temp = PoolMap.at(typeid(T).hash_code());
		ComponentPool<T>* pool = static_cast<ComponentPool<T>*>(temp);
		pool->RemoveComponent(entity);
	}


	//----------------------------------------------------------
	// NOTE TO FUTURE ME: Trying to access a component pool that does not exist is going to throw an excepton you dolt!
	//---------------------------------------------------------
	template<typename First>
	bool ComponentManager::HasComponents(unsigned int entity)
	{
		if (PoolMap.count(typeid(First).hash_code()) > 0)
		{
			BaseComponentPool* temp = PoolMap.at(typeid(First).hash_code());
			if (temp->HasComponent(entity))
				return true;
		}

		return false;
	}


	template<typename First, typename Second, typename... Components>
	bool ComponentManager::HasComponents(unsigned int entity)
	{
		if (PoolMap.count(typeid(First).hash_code()) > 0)
		{
			BaseComponentPool* temp = PoolMap.at(typeid(First).hash_code());
			if (temp->HasComponent(entity) && HasComponents<Second, Components...>(entity))
				return true;
		}

		return false;
	}
}

#endif