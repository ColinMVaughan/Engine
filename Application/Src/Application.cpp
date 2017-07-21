#include "Application.h"
#include "RenderSystem.h"

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
	//DoInitalize();
	PostInitalize();
}


void Application::Update()
{
	PreUpdate();
	//DoUpdate();
	PostUpdate();
}

void Application::PreInitalize()
{
	Camera cam;
	m_Renderer = new Renderer(800, 800, &cam);

}

void Application::PostInitalize()
{
	m_SystemManager->AddSystem<RenderSystem>()->SetRenderer(m_Renderer);
}

void Application::PreUpdate()
{

}

void Application::PostUpdate()
{
	m_ECS->UpdateSystems();
}
