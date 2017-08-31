#include "Application.h"
#include "RenderSystem.h"

Application::Application()
{
	m_ComponentManager = new ComponentManager;
	m_SystemManager = new SystemManager(m_ComponentManager);
	m_ECS = new ECS(m_SystemManager, m_ComponentManager);
	m_Timer = new Timer();
}

Application::~Application()
{
	delete m_ECS;
	delete m_SystemManager;
	delete m_ComponentManager;
}

void Application::Initalize()
{
	PreInitalize();
	DoInitalize();
	PostInitalize();
}


void Application::Update()
{
	double deltaTime = m_Timer->tick();

	PreUpdate(deltaTime);
	DoUpdate(deltaTime);
	PostUpdate(deltaTime);
}

void Application::PreInitalize()
{
	Camera cam;
	m_Renderer = new Renderer(720, 1280, m_Window, &cam);
	m_Physics.Initalize();
}

void Application::PostInitalize()
{
	m_SystemManager->AddSystem<RenderSystem>()->SetRenderer(m_Renderer);
}

void Application::PreUpdate(double deltaTime)
{
	
}

void Application::PostUpdate(double deltaTime)
{
	m_Physics.StepPhysics();
	m_ECS->UpdateSystems(deltaTime);
}

void Application::RegisterKeyboardCallback(BaseSystem* system)
{
	InputCallbackList.push_back(system);
}

void Application::KeyUp(unsigned char key, int x, int y)
{
	for each (auto callback in InputCallbackList)
	{
		callback->KeyUp(key);
	}
}

void Application::KeyDown(unsigned char key, int x, int y)
{
	for each (auto callback in InputCallbackList)
	{
		callback->KeyDown(key);
	}
}

void Application::Unload()
{
	m_Physics.Unload();
}