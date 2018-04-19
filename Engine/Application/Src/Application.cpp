#include "Application.h"
#include "RenderSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

Application::Application()
{
	m_EventManager = new EventManager;
	m_ComponentManager = new ECS::ComponentManager;
	m_SystemManager = new ECS::SystemManager(m_ComponentManager, m_EventManager);
	m_Scene = new ECS::Scene(m_SystemManager, m_ComponentManager,m_EventManager);
	m_Timer = new Timer();
	m_AssetManager = new AssetManager("./Assets/", *m_EventManager);
}

Application::~Application()
{
	delete m_Scene;
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
	
	Camera cam; // renderer requires some default camera so we send it a dummy

	//Get the resolution of the screen
	int width, height;
	SDL_GetWindowSize(m_Window, &width, &height);

	m_Renderer = new Renderer(height, width, m_Window, &cam);
	m_Renderer->Initalize();


	PhysicsSettings PSettings;
	m_Physics.Initalize(PSettings);

}

void Application::PostInitalize()
{
	m_SystemManager->AddSystem<PhysicsSystem>()->m_PhysX = &m_Physics;
	m_SystemManager->AddSystem<CollisionSystem>()->m_Physx = &m_Physics;

	m_SystemManager->AddCoreSystem<PointLightSystem>()->SetRenderer(m_Renderer);
	//m_SystemManager->AddCoreSystem<PhysicsVisualizationSystem>()->Initalize(m_Renderer->DebugBuffer, *m_Renderer->m_Camera, m_Physics);
	m_SystemManager->AddCoreSystem<RenderSystem>()->SetRenderer(m_Renderer);
}

void Application::PreUpdate(double deltaTime)
{
	NewEvent = false;
	while (SDL_PollEvent(&InputEvent))
	{
		NewEvent = true;
		switch (InputEvent.type)
		{
		case SDL_KEYDOWN:
			KeyDown(InputEvent.key);
			break;
		case SDL_KEYUP:
			KeyUp(InputEvent.key);
			break;
		case SDL_MOUSEMOTION:
			MouseMoved(InputEvent.motion);
			break;
		case SDL_QUIT:
			Running = false;
			break;
		}
	}
	//m_Physics.StepPhysics(deltaTime);
}

void Application::PostUpdate(double deltaTime)
{
	m_Scene->UpdateSystems(deltaTime);
}

void Application::RegisterKeyboardCallback(ECS::BaseSystem* system)
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
	if (key.keysym.sym == SDLK_ESCAPE)
		Running = false;

	for each (auto callback in InputCallbackList)
	{
		callback->KeyDown(*SDL_GetKeyName(key.keysym.sym));
	}
}

//Call the callback for each registered system
void Application::MouseMoved(SDL_MouseMotionEvent motion)
{
	for each (auto callback in InputCallbackList)
	{
		callback->MouseMoved((float)motion.xrel * -0.01f, (float)motion.yrel *-0.01f);
	}
}

void Application::Unload()
{
	//m_Physics.Unload();
}