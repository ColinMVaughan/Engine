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
		//Rendering Setup

		m_ECS->AddSystem<PointLightSystem>();
		RegisterKeyboardCallback(m_ECS->AddSystem<PlayerControlSystem>());


		m_Renderer->SetCamera(&m_camera);
		m_Renderer->Initalize();
		m_Renderer->InitalizePBREnvironmentMaps("./Assets/Textures/GCanyon_C_YumaPoint_3k.hdr");

		m_camera.m_Projection = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);



		//--------------------------------------------------------------------------------------------
		//Player Setup
		//--------------------------------------------------------------------------------------------
		//Create entity
		Entity Player = m_ECS->CreateEntity();

		//Add Mesh and Material
		auto characterMesh = m_ECS->AddComponent<Mesh>(Player);
		characterMesh->LoadFromFile("./Assets/Models/Cube.obj");

		m_ECS->AddComponent<VoxelContainer>(Player)->ReadQubicBinaryFile("./Assets/Voxels/character.qb", characterMesh);
		m_ECS->AddComponent<Material>(Player)->SetTexturesFromFolder("./Assets/Textures/Gold");

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
		
		auto pc = m_ECS->AddComponent<PlayerControl>(Player);
		pc->Initalize(m_Physics.GetControllerManager()->createController(description));
		m_ECS->AddComponent<Transform>(Player)->SetActor(pc->GetActor());
		

		//Add Light
		PointLightComponent* light = m_ECS->AddComponent<PointLightComponent>(Player);
		light->Color = glm::fvec3({ 500.0f, 500.0f, 500.0f });
		m_Renderer->AddPointLight(&light->Color, &light->position, false);

		//ConnectCamera
		//CameraPos.SetParentTransform(m_ECS->GetComponent<Transform>(Player));
		//CameraPos.GetTransform()->p = PxVec3(-25, 50, 0);

		//--------------------------------------------------------------------------------------------
		//Ground Setup
		//--------------------------------------------------------------------------------------------
		
		//Create entity
		Entity Plane = m_ECS->CreateEntity();


		//Add Physics / rigidBody
		PxRigidStatic* groundPlane = PxCreatePlane(*m_Physics.GetPhysics(), PxPlane(0, 1, 0, 0), *myMat);
		m_Physics.GetScene()->addActor(*groundPlane);

		m_ECS->AddComponent<Transform>(Plane)->SetActor(groundPlane); // Attach rigidbody to Transform Component


		//------------------------------------------------------------------------------------------------
		// Tiny Cubes Setup
		//-------------------------------------------------------------------------------------------------
		//Mesh* cubeMesh = new Mesh();
		//Material* cubeMat = new Material();
		//
		//cubeMesh->LoadFromFile("./Assets/Models/Cube.obj");
		//cubeMat->SetTexturesFromFolder("./Assets/Textures/Cobblestone");
		//
		//PxMaterial* cubePhysMat = m_Physics.GetPhysics()->createMaterial(0.5, 0.5, 0.5);
		//PxShape* cubeShape = m_Physics.GetPhysics()->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *cubePhysMat, false);
		//
		//
		////Create a 3d grid of dynamic cubes
		//for (float y = 100.0f; y < 105.0f; y += 1.1f)
		//{
		//	for (float x = 0.0f; x < 5.0f; x += 1.1f)
		//	{
		//		for (float z = 0.0f; z < 5.0f; z += 1.1f)
		//		{
		//
		//			Entity cube = m_ECS->CreateEntity();
		//			
		//			//sharing mesh and material to avoid needlessly wasting video memory 
		//			//(Cpu mem still wasted but I want SOME accuracy :p)
		//			*m_ECS->AddComponent<Mesh>(cube) = *cubeMesh;
		//			*m_ECS->AddComponent<Material>(cube) = *cubeMat;
		//
		//			//Add rigidbody / rigidActor
		//			PxRigidBody* cubeBody = m_Physics.GetPhysics()->createRigidDynamic(PxTransform(x, y, z));
		//			m_Physics.GetScene()->addActor(*cubeBody);
		//			m_ECS->AddComponent<Transform>(cube)->SetActor(cubeBody); //Attach rigidbody to Transform component
		//
		//			//Add Shape
		//			cubeBody->attachShape(*cubeShape);
		//			cubeBody->setMass(10.0f);
		//
		//			//Add light
		//
		//			//auto cubelight = m_ECS->AddComponent<PointLightComponent>(cube);
		//			//cubelight->Color = GMath::vec3f({ 10.0f, 10.0f, 10.0f });
		//			//m_Renderer->AddPointLight(&cubelight->Color, &cubelight->position, false);
		//		}
		//	}
		//}

		shape->release();

		//World Setup
		//-------------------------------------------

		Entity World = m_ECS->CreateEntity();
		m_ECS->AddComponent<Transform>(World);
		auto worldMesh = m_ECS->AddComponent<Mesh>(World);
		m_ECS->AddComponent<Material>(World)->SetTexturesFromFolder("./Assets/Textures/RedBrick");

		worldMesh->LoadFromFile("./Assets/Models/Cube.obj");
		m_ECS->AddComponent<VoxelContainer>(World)->ReadQubicBinaryFile("./Assets/Voxels/TestWorld.qb", worldMesh);

		//----------------------------------------------

		return;
	}

	void DoUpdate(double deltaTime) override
	{
		//for (int i = 0; i < 4; ++i)
		//{
		//	for (int j = 0; j < 4; ++j)
		//	{
		//		m_camera.m_Transform[i][j] = CameraPos.GetGlobalTransformMatrix()[i][j];
		//	}
		//}


		//m_camera.m_Transform = glm::rotate(m_camera.m_Transform, -2.5f, glm::vec3(0, 0, 1));
		//m_camera.m_Transform = glm::rotate(m_camera.m_Transform, -1.586f, glm::vec3(0, 1, 0));
		//m_camera.m_Transform = glm::rotate(m_camera.m_Transform, -0.6f, glm::vec3(1, 0, 0));
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