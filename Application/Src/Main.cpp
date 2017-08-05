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




class Demo : public Application
{
	void DoInitalize() override
	{
		LightPosition = new GMath::vec3f({0.0f, 15.0f, -10.0f});
		LightColour = new GMath::vec3f({1000.0f, 1000.0f, 1000.0f});

		Entity Orb = m_ECS->CreateEntity();
		m_ECS->AddComponent<Mesh>(Orb)->LoadFromFile("./Assets/Models/SampleSphere.obj");
		Mesh* mesh = m_ECS->GetComponent<Mesh>(Orb);


		m_ECS->AddComponent<Material>(Orb)->SetTexturesFromFolder("./Assets/Textures/Wall");


		
		m_Renderer->SetCamera(&m_camera);
		m_Renderer->Initalize();
		m_Renderer->InitalizePBREnvironmentMaps("./Assets/Textures/Tokyo_BigSight_3k.hdr");
		m_Renderer->AddPointLight(LightColour, LightPosition, false);

		GMath::SetFrustumProjection(m_camera.m_Projection, 45.0f, 1280.0f/720.0f, 0.1f, 1000.0f);

		glEnable(GL_DEPTH_TEST);
		return;
	}

	void DoUpdate() override
	{
		TotalRotation += 0.005f;

		m_camera.m_Transform = glm::mat4();
		m_camera.m_Transform = glm::rotate(m_camera.m_Transform, TotalRotation, glm::vec3(0, 1, 0));
		m_camera.m_Transform = glm::translate(m_camera.m_Transform, glm::vec3(0.0f, 5.5f, 10.0f));
		m_camera.m_Transform = glm::rotate(m_camera.m_Transform, -0.15f, glm::vec3(1, 0, 0));

		return;
	}

private:
	Camera m_camera;
	float TotalRotation = 0;
	GMath::vec3f* LightPosition;
	GMath::vec3f* LightColour;
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
	
	return 0;

}