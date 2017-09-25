#ifndef BASE_SYSTEM_H
#define BASE_SYSTEM_H
//Colin Vaughan		June 5th, 2017

#include "ComponentManager.h"
#include <iostream>
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

	virtual void PreUpdate(double deltaTime) {};
	virtual void Update(double deltaTime, unsigned int entity) = 0;
	virtual void PostUpdate(double deltaTime) {};

	virtual void UnInitalize() {};

	// returns true if the entity is registered with all the required components.
	virtual bool HasComponents(unsigned int entity)
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

	System(ComponentManager* a_cmanager): m_CManager(a_cmanager){}
	virtual void Initalize() {};

	// PreUpdate:  runs one once per frame
	// Update:     runs once per applicable entity
	// PostUpdate: runs once per frame

	virtual void PreUpdate(double deltaTime) {};
	virtual void Update(double deltaTime, unsigned int entity) {};
	virtual void PostUpdate(double deltaTime) {};

	virtual void UnInitalize() {};


	// Returns true if the entity is registered with all the required components.
	// Marked final to prevent definition by derrived classes.
	virtual bool HasComponents(unsigned int entity) final
	{
		return m_CManager->HasComponents<C, RequiredComponents...>(entity);
	}


	ComponentManager* m_CManager;
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
	void Update(double deltaTime, unsigned int entity) override
	{
		return;
	}
};

#endif