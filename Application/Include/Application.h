#ifndef APPLICATION_H
#define APPLICATION_H

#include<ECS.h>
#include <Renderer.h>
#include <BaseSystem.h>
#include <PhysicsSystem.h>

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

	void KeyUp(unsigned char key, int x, int y);
	void KeyDown(unsigned char key, int x, int y);

protected:
	void PreInitalize();
	virtual void DoInitalize() = 0;
	void PostInitalize();

	void PreUpdate(double deltaTime);
	virtual void DoUpdate(double deltaTime) = 0;
	void PostUpdate(double deltaTime);

	void RegisterKeyboardCallback(BaseSystem* system);


protected:

	ComponentManager* m_ComponentManager;
	SystemManager* m_SystemManager;
	ECS* m_ECS;

	Renderer* m_Renderer;
	PhysicsSystem m_Physics;

	Timer* m_Timer;
	std::vector<BaseSystem*> InputCallbackList;

	SDL_Window* m_Window;
};

#endif