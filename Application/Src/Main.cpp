//--------------------------------------------------------------------------------------------------
//									DEMO APP
//								  Colin Vaughan 
//
//
//	Demo App includes selection of different lighting and material scenareos with a
//	single point light. The demo showcases the ECS pattern design and a component / memory manager
//	The renderer is wrapped in a dedicated system class that fully integrates into the system
//	and component managers.
//---------------------------------------------------------------------------------------------------

#include <GL/glew.h>
#include <SDL\SDL.h>

#include "Application.h"
#include <glm/gtc/matrix_transform.hpp>
#include <PhysicsSystem.h>
#include <RenderSystem.h>
#include "PlayerControlSystem.h"
#include "Voxels.h"

class Demo : public Application
{
	void DoInitalize() override
	{
		//---------------------------------------------------
		//  Rendering Setup
		//-------------------------------------------------
		m_Scene->AddSystem<PointLightSystem>();
		auto vds = m_Scene->AddSystem<VoxelDestructionSystem>();
		vds->Initalize(&m_Physics);


		RegisterKeyboardCallback(m_Scene->AddSystem<FPSControlSystem>());
		RegisterKeyboardCallback(vds);


		m_Renderer->Initalize();
		m_Renderer->InitalizePBREnvironmentMaps("./Assets/Textures/Footprint_Court_2k.hdr");

		//---------------------------------------------------------------
		//	Camera Setup
		//--------------------------------------------------------------

		auto cameraEntity = m_Scene->CreateEntity();
		auto camera = m_Scene->AddComponent<Camera>(cameraEntity);
		m_Scene->AddComponent<Transform>(cameraEntity);

		m_Renderer->SetCamera(camera);
		camera->m_Projection = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);

		//--------------------------------------------------------------------------------------------
		//Player Setup
		//--------------------------------------------------------------------------------------------
		//Create entity
		ECS::Entity Player = m_Scene->CreateEntity();

		//Add Mesh and Material
		auto characterMesh = m_Scene->AddComponent<Mesh>(Player);
		characterMesh->LoadFromFile("./Assets/Models/Cube.obj");

		auto m_Test = m_Scene->AddComponent<VoxelContainer>(Player);
		m_Test->ReadQubicBinaryFile("./Assets/Voxels/character.qb", characterMesh);
		vds->AddTestCase(m_Test);


		m_Scene->AddComponent<Material>(Player)->SetTexturesFromFolder("./Assets/Textures/Gold");

		//create shape and physics material
		PxMaterial* myMat = m_Physics.GetPhysics()->createMaterial(0.5, 0.5, 0.5);
		PxShape* shape = m_Physics.GetPhysics()->createShape(PxSphereGeometry(3.0f), *myMat, true);

		//Add PlayerController
		PxCapsuleControllerDesc description;
		description.position = PxExtendedVec3(0, 10.0f, 0);
		description.height = 0.001f;
		description.radius = 3.0f;
		description.material = myMat;
		description.upDirection = PxVec3(1, 0, 0);
		
		auto pc = m_Scene->AddComponent<PlayerControl>(Player);
		auto characterController = m_Physics.GetControllerManager()->createController(description);
		pc->Initalize(characterController);
		m_Scene->AddComponent<Transform>(Player)->SetActor(pc->GetActor());
		

		//Add Light
		PointLightComponent* light = m_Scene->AddComponent<PointLightComponent>(Player);
		light->Color = glm::fvec3({ 500.0f, 500.0f, 500.0f });
		m_Renderer->AddPointLight(&light->Color, &light->position, false);


		//--------------------------------------------------------------------------------------------
		//Ground Setup
		//--------------------------------------------------------------------------------------------
		//Create entity
		ECS::Entity Plane = m_Scene->CreateEntity();

		//Add Physics / rigidBody
		PxRigidStatic* groundPlane = PxCreatePlane(*m_Physics.GetPhysics(), PxPlane(0, 1, 0, 0), *myMat);
		m_Physics.GetScene()->addActor(*groundPlane);

		m_Scene->AddComponent<Transform>(Plane)->SetActor(groundPlane); // Attach rigidbody to Transform Component

		shape->release();

		//World Setup
		//-------------------------------------------
		ECS::Entity World = m_Scene->CreateEntity();
		m_Scene->AddComponent<Transform>(World);
		auto worldMesh = m_Scene->AddComponent<Mesh>(World);
		m_Scene->AddComponent<Material>(World)->SetTexturesFromFolder("./Assets/Textures/RedBrick");

		worldMesh->LoadFromFile("./Assets/Models/Cube.obj");
		m_Scene->AddComponent<VoxelContainer>(World)->ReadQubicBinaryFile("./Assets/Voxels/TestWorld.qb", worldMesh);

		//add light
		PointLightComponent* Wlight = m_Scene->AddComponent<PointLightComponent>(World);
		Wlight->Color = glm::fvec3({ 500.0f, 500.0f, 500.0f });
		m_Renderer->AddPointLight(&Wlight->Color, &Wlight->position, false);
		//----------------------------------------------

		return;
	}

	void DoUpdate(double deltaTime) override
	{
		return;
	}

private:

	float TotalRotation = 0;
};


////////////////////////////////////////////////////////////////////////////////////////////
//This Section is going to be replaced with a new window handler, most likely SDL
// 
//first I would like to make sure tha there is no issue with the original rendering 
////////////////////////////////////////////////////////////////////////////////////////////
const int FRAME_DELAY_SPRITE = 1000 / 60;


Demo *demo = new Demo();


int main(int argc, char **argv)
{

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		std::cout << "SDL was not Initalized.";
		return 1;
	}

	Uint32 WindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


	demo->m_Window = SDL_CreateWindow("Ocean Engine", 100, 100, 1280, 720, WindowFlags);
	SDL_GL_CreateContext(demo->m_Window);

	//---------------------------------------------------------

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW was not initalized\n";
		system("pause");
		return 0;
	}



	demo->Initalize();
	while (demo->Running)
	{
		demo->Update();
	}
	demo->Unload();
	
	return 0;

}