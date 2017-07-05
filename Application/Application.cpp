#include "Application.h"
#include "Renderer/RenderSystem.h"

Application::Application()
{
	m_ComponentManager = new ComponentManager;
	m_SystemManager = new SystemManager(m_ComponentManager);
	m_ECS = new ECS(m_SystemManager, m_ComponentManager);
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
	Initalize();
	PostInitalize();
}


void Application::Update()
{
	PreUpdate();
	DoUpdate();
	PostUpdate();
}

void Application::PreInitalize()
{

}

void Application::PostInitalize()
{
	m_SystemManager->AddSystem<RenderSystem>();
}

void Application::PreUpdate()
{
}

void Application::PostUpdate()
{
	
}
