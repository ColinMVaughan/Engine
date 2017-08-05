#ifndef APPLICATION_H
#define APPLICATION_H

#include<ECS.h>
#include <Renderer.h>
#include <BaseSystem.h>

//------------------------------------------------
//
//
//-----------------------------------------------

class Application
{
public:
	Application();
	~Application();

	void Initalize();
	void Update();

protected:

	void PreInitalize();
	virtual void DoInitalize() = 0;
	void PostInitalize();

	void PreUpdate();
	virtual void DoUpdate() = 0;
	void PostUpdate();


	void RegisterKeyboardCallback();
	void KeyUp(unsigned char key, int x, int y);
	void KeyDown(unsigned char key, int x, int y);


protected:

	ComponentManager* m_ComponentManager;
	SystemManager* m_SystemManager;
	ECS* m_ECS;

	Renderer* m_Renderer;
	BaseSystem* CallbackFunction;
};

#endif