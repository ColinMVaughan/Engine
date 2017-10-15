#include "Application.h"
#include "RenderSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

Application::Application()
{
	m_ComponentManager = new ECS::ComponentManager;
	m_SystemManager = new ECS::SystemManager(m_ComponentManager);
	m_ECS = new ECS::ECS(m_SystemManager, m_ComponentManager);
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
	m_Physics.Initalize(PSettings);

}

void Application::PostInitalize()
{
	m_SystemManager->AddSystem<RenderSystem>()->SetRenderer(m_Renderer);
	CameraPos.GetTransform()->p = PxVec3(10.0f,50.0f,70.0f);
	//CameraPos.GetTransform()->q *= PxQuat(0.5, PxVec3(1, 0, 0));
	std::cout << "Block";
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
		case SDL_MOUSEMOTION:
			MouseMoved(InputEvent.motion);
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
	m_Physics.Unload();
}