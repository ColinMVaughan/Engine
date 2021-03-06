#ifndef COMPONENT_POOL_H
#define COMPONENT_POOL_H

#include <map>

namespace ECS
{

	//-------------------------------------------------
	//				Component Pool
	//
	//	Component pool holds an array of componetns that 
	//	can be accessed with the corrisponding entity.
	//-------------------------------------------------


	class BaseComponentPool
	{
	public:
		virtual ~BaseComponentPool() {}

		virtual bool HasComponent(unsigned int entity) = 0;
		virtual void RemoveComponent(unsigned int entity) = 0;
	};



	template <typename T>
	class ComponentPool : public BaseComponentPool
	{
	public:
		ComponentPool();
		~ComponentPool();
		void AddComponent(unsigned int entityID);
		T* GetComponent(unsigned int entityID);
		void RemoveComponent(unsigned int entityID) override;
		bool HasComponent(unsigned int entity) override;

	private:

		std::map<unsigned int, T> Components;
	};

	//-----------------------------------------------------------------
	//						IMPLEMENTATION
	//-----------------------------------------------------------------

	template <typename T>
	ComponentPool<T>::ComponentPool()
	{
	}

	template <typename T>
	ComponentPool<T>::~ComponentPool()
	{
		Components.clear();
	}

	template<typename T>
	inline void ComponentPool<T>::AddComponent(unsigned int entityID)
	{
		T comp;
		Components.insert(std::make_pair(entityID, comp));
	}

	template<typename T>
	inline T * ComponentPool<T>::GetComponent(unsigned int entityID)
	{
		std::map<unsigned, T>::iterator it = Components.find(entityID);
		return (it != Components.end()) ? &it->second : nullptr;
	}

	template<typename T>
	inline void ComponentPool<T>::RemoveComponent(unsigned int entityID)
	{
		Components.erase(entityID);
	}

	template<typename T>
	inline bool ComponentPool<T>::HasComponent(unsigned int entity)
	{
		return (Components.find(entity) != Components.end()) ? true : false;
	}

}
#endif