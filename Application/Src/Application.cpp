#include "Application.h"
#include "RenderSystem.h"
#include <glm/gtc/matrix_transform.hpp>

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
	m_Physics.Initalize(PSettings);

	currentRot = glm::vec2(0);
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
	for each (auto callback in InputCallbackList)
	{
		callback->KeyDown(*SDL_GetKeyName(key.keysym.sym));
	}
}

void Application::MouseMoved(SDL_MouseMotionEvent motion)
{
	currentRot.y += ((float)motion.yrel) * -0.01f;
	currentRot.x += ((float)motion.xrel) * -0.01f;

	m_camera.m_Transform = glm::fmat4();

	m_camera.m_Transform = glm::translate(m_camera.m_Transform, glm::vec3(10, 50, 70));

	m_camera.m_Transform = glm::rotate(m_camera.m_Transform, currentRot.x, glm::vec3(0, 1, 0));
	m_camera.m_Transform = glm::rotate(m_camera.m_Transform, currentRot.y, glm::vec3(1, 0, 0));




}

void Application::Unload()
{
	m_Physics.Unload();
}