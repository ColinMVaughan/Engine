//--------------------------------------------------------------------------------------------------
//									DEMO APP
//								  Colin Vaughan 
//
//
//	Demo App includes selection of different lighting and material scenareos with
//	multiple point lights, voxel destruction, and physics. The demo showcases the ECS pattern design 
//  and a component / memory manager
//	
//---------------------------------------------------------------------------------------------------

#include <GL/glew.h>
#include <SDL\SDL.h>

#include <Application.h>
#include <glm/gtc/matrix_transform.hpp>
#include <PhysicsSystem.h>
#include <RenderSystem.h>
#include <PlayerControlSystem.h>
#include <Voxels.h>
#include <VoxelDestructionSystem.h>

#include <imgui.h>
#include "imgui_SDL.h"

#include <EditorUI.h>

class Demo : public Application
{
	void DoInitalize() override
	{
		//-----------------------------------------------------------
		//	Add Desired Systems & Callbacks
		//-----------------------------------------------------------
		m_Scene->AddSystem<PointLightSystem>();
		auto vds = m_Scene->AddSystem<VoxelDestructionSystem>();
		vds->Initalize(&m_Physics);

		RegisterKeyboardCallback(m_Scene->AddSystem<FPSControlSystem>());
		RegisterKeyboardCallback(vds);

		//---------------------------------------------------
		//  Rendering Setup
		//-------------------------------------------------
		m_Renderer->InitalizePBREnvironmentMaps("./Assets/Textures/Footprint_Court_2k.hdr");

		//---------------------------------------------------------------
		//	Camera Setup
		//--------------------------------------------------------------
		//Create entity & add Camera
		ECS::Entity Player = m_Scene->CreateEntity();
		auto camera = Player.AddComponent<Camera>();
		Player.AddComponent<Transform>();

		m_Renderer->SetCamera(camera);
		camera->m_Projection = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);

		//--------------------------------------------------------------------------------------------
		//Player Constrol Setup
		//-------------------------------------------------------------------------------------------
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

		auto pc = Player.AddComponent<PlayerControl>();
		auto characterController = m_Physics.GetControllerManager()->createController(description);
		pc->Initalize(characterController);

		Player.AddComponent<Transform>()->SetActor(pc->GetActor());

		//Add Light
		PointLightComponent* light = Player.AddComponent<PointLightComponent>();
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

		Plane.AddComponent<Transform>()->SetActor(groundPlane); // Attach rigidbody to Transform Component

		shape->release();

		//World Setup
		//-------------------------------------------
		ECS::Entity World = m_Scene->CreateEntity();
		World.AddComponent<Transform>();
		auto worldMesh = World.AddComponent<Mesh>();
		World.AddComponent<Material>()->SetTexturesFromFolder("./Assets/Textures/RedBrick");

		worldMesh->LoadFromFile("./Assets/Models/Cube.obj");
		World.AddComponent<VoxelContainer>()->ReadQubicBinaryFile("./Assets/Voxels/TestWorld.qb", worldMesh);

		//add light
		PointLightComponent* Wlight = m_Scene->AddComponent<PointLightComponent>(World);
		Wlight->Color = glm::fvec3({ 500.0f, 500.0f, 500.0f });
		m_Renderer->AddPointLight(&Wlight->Color, &Wlight->position, false);
		//----------------------------------------------

		return;
	}

	void DoUpdate(double deltaTime) override
	{
		ImGui_ImplSdlGL3_ProcessEvent(&InputEvent);
		ImGui_ImplSdlGL3_NewFrame(m_Window);



		m_Renderer->UIBuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		m_Renderer->UIBuffer.UnBind();

		return;
	}

private:

	float TotalRotation = 0;
};


////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////
const int FRAME_DELAY_SPRITE = 1000 / 60;

Editor *demo = new Editor();

int main(int argc, char **argv)
{
	//-------------------------------------------------------------------------
	//		Init SDL
	//-------------------------------------------------------------------------
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
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	//-----------------------------------------------------------
	//		Init GLEW
	//---------------------------------------------------------
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW was not initalized\n";
		system("pause");
		return 0;
	}

	//-----------------------------------------------------------
	//	Init IMGUI
	//-----------------------------------------------------------
	ImGui_ImplSdlGL3_Init(demo->m_Window);

	//-----------------------------------------------------------
	//	Init & Run Application
	//------------------------------------------------------------
	demo->Initalize();
	while (demo->Running)
	{
		demo->Update();
	}
	demo->Unload();


	ImGui_ImplSdlGL3_Shutdown();
	return 0;

}