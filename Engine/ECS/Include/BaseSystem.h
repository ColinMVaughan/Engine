#ifndef BASE_SYSTEM_H
#define BASE_SYSTEM_H
//Colin Vaughan		June 5th, 2017

#include "ComponentManager.h"
#include "EventManager.h"
#include <iostream>

namespace ECS
{
	class Entity;

	template<typename T>
	class AddComponentEvent : public IEvent
	{
	public:
		AddComponentEvent() = default;
		AddComponentEvent(Entity* a_entity) : m_Entity(a_entity){}

		Entity* m_Entity;
	};


	//-----------------------------------------
	//				BASE SYSTEM
	// Derrive your system from this class to be able to submit it to the system manager.
	// When derriving From base system, set component requirements by setting them as
	// template arguments for BaseSystem.
	//-----------------------------------------
	class BaseSystem
	{
	public:
		virtual void Initalize() {};

		// PreUpdate:  runs one once per frame
		// Update:     runs once per applicable entity
		// PostUpdate: runs once per frame

		//virtual void PreStart() {}
		//virtual void Start(Entity& entity) = 0;
		//virtual void PostStart() {}

		virtual void PreUpdate(double deltaTime) {}
		virtual void Update(double deltaTime, Entity& entity) = 0;
		virtual void PostUpdate(double deltaTime) {}

		//virtual void PreStop() {}
		//virtual void Stop(Entity& entity) = 0;
		//virtual void PostStop(){}

		virtual void UnInitalize() {};

		// returns true if the entity is registered with all the required components.
		virtual bool HasComponents(unsigned int entityID)
		{
			std::cout << "\nThis function (BaseSystem::HasComponent) should never be called.";
			return false;
		}

		virtual void KeyUp(unsigned char key) {}
		virtual void KeyDown(unsigned char key) {}
		virtual void MouseMoved(float xVelocity, float yVelicity) {}
	};


	//---------------------------------------------------------------------
	//					SYSTEM
	//
	// This is the class that you should derrive your custom system from.
	//
	// When derriving from this class, include the required components 
	// as system's template arguments. This ensures that only entities with
	// all required componets are updated within the system.
	//----------------------------------------------------------------------
	template<typename C, typename... RequiredComponents>
	class System : public BaseSystem
	{
	public:

		System(ComponentManager* a_cmanager, EventManager& a_eManager) : m_CManager(a_cmanager)
		{
			RegisterEventCallback_AddComponent<C, RequiredComponents...>(a_eManager);
		}
		virtual void Initalize() {};

		// PreUpdate:  runs one once per frame
		// Update:     runs once per applicable entity
		// PostUpdate: runs once per frame

		//virtual void PreStart() {}
		//virtual void Start(Entity& entity) = 0;
		//virtual void PostStart() {}

		virtual void PreUpdate(double deltaTime) {};
		virtual void Update(double deltaTime, Entity& entity) {};
		virtual void PostUpdate(double deltaTime) {};

		//virtual void PreStop() {}
		//virtual void Stop(Entity& entity) = 0;
		//virtual void PostStop() {}

		virtual void UnInitalize() {};
		virtual void ComponentAdded(Entity& entity) {}

		// Returns true if the entity is registered with all the required components.
		// Marked final to prevent definition by derrived classes.
		virtual bool HasComponents(unsigned int entityID) final
		{
			return m_CManager->HasComponents<C, RequiredComponents...>(entityID);
		}

		ComponentManager* m_CManager;




		template<typename T>
		void ComponentAddedCallback(AddComponentEvent<T>& a_event)
		{
			ComponentAdded(*a_event.m_Entity);
		}

	private:

		template<typename First>
		void RegisterEventCallback_AddComponent(EventManager& eManager)
		{
			//REGISTER_EVENT_LISTNER(AddComponentEvent<First>, System::ComponentAddedCallback<First>, eManager)
			std:std::function<void(AddComponentEvent<First>&)> f2 = [this](AddComponentEvent<First>& eve) {ComponentAddedCallback<First>(eve); };
			eManager.AddListner<AddComponentEvent<First>>(f2);
			return;
		}

		template<typename First, typename Second, typename ...Components>
		void RegisterEventCallback_AddComponent(EventManager& eManager)
		{
			//REGISTER_EVENT_LISTNER(AddComponentEvent<First>, System::ComponentAddedCallback<First>, eManager)
			std:std::function<void(AddComponentEvent<First>&)> f2 = [this](AddComponentEvent<First>& eve) {ComponentAddedCallback<First>(eve); };
			eManager.AddListner<AddComponentEvent<First>>(f2);

			RegisterEventCallback_AddComponent<Second, Components...>(eManager);
			return;
		}
	};


	//------------------------------------------------------
	//					EXAMPLEDERRIVEDSYSTEM
	//
	// This is an example of a basic custom system.
	// Notice that the Update function must be included, and the tamplate arguments
	// represent the requred components.
	//-----------------------------------------------------
	class ExampleDerrivedSystem : public System<int, bool>
	{
		void Update(double deltaTime, Entity& entity) override
		{
			return;
		}
	};

}

#endif