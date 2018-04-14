#ifndef APPLICATION_H
#define APPLICATION_H

#include <Entity.h>
#include <ECS.h>
#include <Renderer.h>
#include <BaseSystem.h>
#include <PhysicsSystem.h>
#include <EventManager.h>
#include "AssetManager.h"

//------------------------------------------------
//				Application
//
//	Application class is the entry point for the engine.
//	Users can inheret from this class and deifine cusom initalization and update methods.
//	All other methods ensure proper initalization and updatre order of key engine systems.
//-----------------------------------------------

class Application
{
public:
	Application();
	~Application();

	void Initalize();
	void Update();
	void Unload();

	void KeyUp(SDL_KeyboardEvent key);
	void KeyDown(SDL_KeyboardEvent key);
	void MouseMoved(SDL_MouseMotionEvent motion);

protected:
	void PreInitalize();
	virtual void DoInitalize() = 0;
	void PostInitalize();

	virtual void PreUpdate(double deltaTime);
	virtual void DoUpdate(double deltaTime) = 0;
	virtual void PostUpdate(double deltaTime);

	void RegisterKeyboardCallback(ECS::BaseSystem* system);


public:
	bool Running = true;
	SDL_Window* m_Window;
protected:

	EventManager * m_EventManager;
	ECS::ComponentManager* m_ComponentManager;
	ECS::SystemManager* m_SystemManager;
	ECS::Scene* m_Scene;
	AssetManager* m_AssetManager;

	Renderer* m_Renderer;
	PhysXWrapper m_Physics;

	Timer* m_Timer;
	std::vector<ECS::BaseSystem*> InputCallbackList;
	SDL_Event InputEvent;
	bool NewEvent = false;
};

#endif