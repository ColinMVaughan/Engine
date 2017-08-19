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
#include <GL\freeglut.h>

#include "Application.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GMath\MathLibrary.h>
#include <PhysicsSystem.h>
#include <RenderSystem.h>

class Demo : public Application
{
	void DoInitalize() override
	{
		//Rendering Setup

		m_ECS->AddSystem<PointLightSystem>();

		m_Renderer->SetCamera(&m_camera);
		m_Renderer->Initalize();
		m_Renderer->InitalizePBREnvironmentMaps("./Assets/Textures/Tokyo_BigSight_3k.hdr");

		GMath::SetFrustumProjection(m_camera.m_Projection, 45.0f, 1280.0f/720.0f, 0.1f, 1000.0f);
		glEnable(GL_DEPTH_TEST);

		//--------------------------------------------------------------------------------------------
		//Orb Setup
		//--------------------------------------------------------------------------------------------

		//Create entity
		Entity Orb = m_ECS->CreateEntity();

		//Add Mesh and Material
		m_ECS->AddComponent<Mesh>(Orb)->LoadFromFile("./Assets/Models/Orb.obj");
		m_ECS->AddComponent<Material>(Orb)->SetTexturesFromFolder("./Assets/Textures/Gold");


		//Add rigidbody / rigidActor
		PxRigidBody* body = m_Physics.GetPhysics()->createRigidDynamic(PxTransform(0, 10.0f, 0));
		body->setMass(100000.0f);
		m_Physics.GetScene()->addActor(*body);

		m_ECS->AddComponent<RigidActor>(Orb)->m_RigidActor = body; //Attach rigidbody to RigidActor component
		
		//Add Shape
		PxMaterial* myMat = m_Physics.GetPhysics()->createMaterial(0.5, 0.5, 0.5);
		PxShape* shape = m_Physics.GetPhysics()->createShape(PxSphereGeometry(3.0f), *myMat, true);
		
		body->attachShape(*shape);
		shape->release();

		//Add Light
		PointLightComponent* light = m_ECS->AddComponent<PointLightComponent>(Orb);
		light->Color = GMath::vec3f({ 424.0f, 350.0f, 110.0f });
		m_Renderer->AddPointLight(&light->Color, &light->position, false);


		//--------------------------------------------------------------------------------------------
		//Ground Setup
		//--------------------------------------------------------------------------------------------
		
		//Create entity
		Entity Plane = m_ECS->CreateEntity();

		//Add Mesh and Material
		m_ECS->AddComponent<Mesh>(Plane)->LoadFromFile("./Assets/Models/Plane.obj");
		m_ECS->AddComponent<Material>(Plane)->SetTexturesFromFolder("./Assets/Textures/Cobblestone");


		//Add Physics / rigidBody
		PxRigidStatic* groundPlane = PxCreatePlane(*m_Physics.GetPhysics(), PxPlane(0, 1, 0, 0), *myMat);
		m_Physics.GetScene()->addActor(*groundPlane);

		m_ECS->AddComponent<RigidActor>(Plane)->m_RigidActor = groundPlane; // Attach rigidbody to RigidActor Component





		//------------------------------------------------------------------------------------------------
		// Tiny Cubes Setup
		//-------------------------------------------------------------------------------------------------
		Mesh* cubeMesh = new Mesh();
		Material* cubeMat = new Material();

		cubeMesh->LoadFromFile("./Assets/Models/Cube.obj");
		cubeMat->SetTexturesFromFolder("./Assets/Textures/Cobblestone");

		PxMaterial* cubePhysMat = m_Physics.GetPhysics()->createMaterial(0.5, 0.5, 0.5);
		PxShape* cubeShape = m_Physics.GetPhysics()->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *cubePhysMat, false);


		//Create a 3d grid of dynamic cubes
		for (float y = 100.0f; y < 105.0f; y += 1.1f)
		{
			for (float x = 0.0f; x < 5.0f; x += 1.1f)
			{
				for (float z = 0.0f; z < 5.0f; z += 1.1f)
				{

					Entity cube = m_ECS->CreateEntity();
					
					//sharing mesh and material to avoid needlessly wasting video memory 
					//(Cpu mem still wasted but I want SOME accuracy :p)
					*m_ECS->AddComponent<Mesh>(cube) = *cubeMesh;
					*m_ECS->AddComponent<Material>(cube) = *cubeMat;

					//Add rigidbody / rigidActor
					PxRigidBody* cubeBody = m_Physics.GetPhysics()->createRigidDynamic(PxTransform(x, y, z));
					m_Physics.GetScene()->addActor(*cubeBody);
					m_ECS->AddComponent<RigidActor>(cube)->m_RigidActor = cubeBody; //Attach rigidbody to RigidActor component

					//Add Shape
					cubeBody->attachShape(*cubeShape);
					cubeBody->setMass(10.0f);
				}
			}
		}

		shape->release();


		return;
	}

	void DoUpdate() override
	{
		TotalRotation += 0.005f;

		m_camera.m_Transform = glm::mat4();
		m_camera.m_Transform = glm::rotate(m_camera.m_Transform, TotalRotation, glm::vec3(0, 1, 0));
		m_camera.m_Transform = glm::translate(m_camera.m_Transform, glm::vec3(0.0f, 5.5f, 20.0f));
		m_camera.m_Transform = glm::rotate(m_camera.m_Transform, -0.15f, glm::vec3(1, 0, 0));

		return;
	}



private:
	Camera m_camera;
	float TotalRotation = 0;

};


////////////////////////////////////////////////////////////////////////////////////////////
//This Section is going to be replaced with a new window handler, most likely SDL
// 
//first I would like to make sure tha there is no issue with the original rendering 
////////////////////////////////////////////////////////////////////////////////////////////
const int FRAME_DELAY_SPRITE = 1000 / 60;


Demo *demo = new Demo();

void DisplayCallbackFunction()
{
	demo->Update();
}

void TimerCallbackFunction(int value)
{
	glutPostRedisplay();
	glutTimerFunc(FRAME_DELAY_SPRITE, TimerCallbackFunction, 0);
}

void KeyboardCallbackFunction(unsigned char key, int x, int y)
{

}

void KeyboardUpCallbackFunction(unsigned char key, int x, int y)
{

}

void MouseClickCallbackFunction(int button, int state, int x, int y)
{
	glutPostRedisplay();
}

void MouseMotionCallbackFunction(int x, int y)
{
	glutPostRedisplay();

}

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitContextVersion(4, 2);
	glutInitWindowSize(1280, 720);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("TempWindow");

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW was not initalized\n";
		system("pause");
		return 0;
	}


	glutDisplayFunc(DisplayCallbackFunction);
	glutKeyboardFunc(KeyboardCallbackFunction);
	glutKeyboardUpFunc(KeyboardUpCallbackFunction);
	glutMouseFunc(MouseClickCallbackFunction);
	glutMotionFunc(MouseMotionCallbackFunction);
	glutTimerFunc(1,TimerCallbackFunction,0);
	std::cout << "\nCheck 1";
	demo->Initalize();


	glutMainLoop();

	demo->Unload();
	
	return 0;

}