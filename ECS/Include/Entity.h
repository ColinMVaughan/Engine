#ifndef ENTITY_H
#define ENTITY_H

class Scene;

namespace ECS
{
	class Entity
	{
	public:
		Entity(unsigned int a_id) :m_ID(a_id) {}

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


#endif