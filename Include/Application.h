#pragma once
#include<ECS/ECS.h>

class Application
{
public:
	Application();
	~Application();

	void Initalize();
	void Update();

private:

	void PreInitalize();
	virtual void DoInitalize() = 0;
	void PostInitalize();

	void PreUpdate();
	virtual void DoUpdate() = 0;
	void PostUpdate();


private:

	ComponentManager* m_ComponentManager;
	SystemManager* m_SystemManager;
	ECS* m_ECS;
};

