#ifndef COMPONENT_POOL_H
#define COMPONENT_POOL_H

#include <map>

class BaseComponentPool
{
public:
	virtual bool HasComponent(unsigned int entity) = 0;
};



template <typename T>
class ComponentPool : public BaseComponentPool
{
public:
	ComponentPool();
	~ComponentPool();
	void AddComponent(unsigned int entityID);
	T* GetComponent(unsigned int entityID);
	bool HasComponent(unsigned int entity) override;

private:

	std::map<unsigned int, T> Components;
	T* m_Pool;
};

//-----------------------------------------------------------------
//						IMPLEMENTATION
//-----------------------------------------------------------------

template <typename T>
ComponentPool<T>::ComponentPool()
{
	m_Pool = new T[10];
}

template <typename T>
ComponentPool<T>::~ComponentPool()
{
	delete m_Pool;
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
	return &Components.at(entityID);
}

template<typename T>
inline bool ComponentPool<T>::HasComponent(unsigned int entity)
{
	return (Components.find(entity) != Components.end()) ? true : false;
}

#endif