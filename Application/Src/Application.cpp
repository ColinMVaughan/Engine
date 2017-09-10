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

	m_Timer->tick();
	double deltaTime = m_Timer->getElapsedTimeSeconds();

	PreUpdate(deltaTime);
	DoUpdate(deltaTime);
	PostUpdate(deltaTime);
}

void Application::PreInitalize()
{
	Camera cam;
	m_Renderer = new Renderer(720, 1280, m_Window, &cam);

	PhysicsSettings PSettings;
	//PSettings.GpuRigidbodies = true;
	//PSettings.OpenGLContext = SDL_GL_GetCurrentContext();
	m_Physics.Initalize(PSettings);
}

void Application::PostInitalize()
{
	m_SystemManager->AddSystem<RenderSystem>()->SetRenderer(m_Renderer);
}

void Application::PreUpdate(double deltaTime)
{
	while (SDL_PollEvent(&InputEvent))
	{
		switch (InputEvent.type)
		{
		case SDL_KEYDOWN:
			KeyDown(InputEvent.key);
			break;
		case SDL_KEYUP:
			KeyUp(InputEvent.key);
			break;
		case SDL_QUIT:
			Running = false;
			break;
		}
	}
	m_Physics.StepPhysics(deltaTime);
}

void Application::PostUpdate(double deltaTime)
{

	m_ECS->UpdateSystems(deltaTime);
}

void Application::RegisterKeyboardCallback(BaseSystem* system)
{
	InputCallbackList.push_back(system);
}

void Application::KeyUp(SDL_KeyboardEvent key)
{
	for each (auto callback in InputCallbackList)
	{
		callback->KeyUp(*SDL_GetKeyName(key.keysym.sym));
	}
}

void Application::KeyDown(SDL_KeyboardEvent key)
{
	std::cout <<*SDL_GetKeyName(key.keysym.sym);
	for each (auto callback in InputCallbackList)
	{
		callback->KeyDown(*SDL_GetKeyName(key.keysym.sym));
	}
}

void Application::Unload()
{
	m_Physics.Unload();
}