#include "Game.h"
#include <Renderer\Utilities.h>

Game::Game()
	: m_Renderer(WINDOW_HEIGHT, WINDOW_WIDTH, &m_Camera)
{
	lightPos = GMath::vec3f({ 0.0f, 15.0f, -10.0f });
	lightColor = GMath::vec3f({ 1000.0f, 1000.0f, 1000.0f });


	lightPos2 = GMath::vec3f({ 0.0f, 15.0f, 10.0f });
	lightColor2 = GMath::vec3f({ 1000.0f, 1000.0f, 1000.0f });
}

Game::~Game()
{
	delete updateTimer;


}

void Game::initializeGame()
{
	updateTimer = new Timer();
	InitFullScreenQuad();

	glEnable(GL_DEPTH_TEST);
	GMath::SetFrustumProjection(m_Camera.m_Projection, 45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 10000.0f);


	//Load Orb / different materials & initalize renderer
	//-----------------------------------------------------------------
	if (!Orb.LoadFromFile("./Assets/Models/SampleSphere.obj"))
	{
		std::cout << "Couldn't find the monkey.\n";
		system("pause");
		exit(0);
	}

	m_Renderer.Initalize();
	m_Renderer.InitalizePBREnvironmentMaps("./Assets/Textures/Tokyo_BigSight_3k.hdr");
	m_Renderer.AddPointLight(&lightColor, &lightPos, false);
	//m_Renderer.AddPointLight(&lightColor2, &lightPos2, false);



	std::string folders[] = { "Whethered_Steel","Gold","Blood_Wood","Rust","Cobblestone" };
	for (int i = 0; i< 5; ++i)
	{
		if (!m_Materials[i].Albedo.Load("./Assets/Textures/" + folders[i] + "/Albedo.png"))
		{
			system("pause");
			exit(0);
		}
		if (!m_Materials[i].Metallic.Load("./Assets/Textures/" + folders[i] + "/Metallic.png"))
		{
			system("pause");
			exit(0);
		}
		if (!m_Materials[i].Roughness.Load("./Assets/Textures/" + folders[i] + "/Roughness.png"))
		{
			system("pause");
			exit(0);
		}
		if (!m_Materials[i].Normal.Load("./Assets/Textures/" + folders[i] + "/Normal.png"))
		{
			system("pause");
			exit(0);
		}
		if (!m_Materials[i].AO.Load("./Assets/Textures/" + folders[i] + "/AO.png"))
		{
			system("pause");
			exit(0);
		}

	}
	m_Renderer.AddMesh(&Orb, &m_Materials[0]);
	return;

}

void Game::update()
{

	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	m_Camera.m_Transform = glm::mat4();
	m_Camera.m_Transform = glm::rotate(m_Camera.m_Transform, float(TotalGameTime * 0.2f), glm::vec3(0, 1, 0));
	m_Camera.m_Transform = glm::translate(m_Camera.m_Transform, glm::vec3(0.0f, 5.5f, 10.0f));
	m_Camera.m_Transform = glm::rotate(m_Camera.m_Transform, -0.15f, glm::vec3(1, 0, 0));

}




void Game::draw()
{
	m_Renderer.PreRender();
	m_Renderer.Render(&Orb, &m_Materials[0]);
	m_Renderer.PostRender();
	return;

}


void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	switch (key)
	{
	case '1':
		break;

	case '2':
		break;

	case '3':
		break;

	case '4':
		break;

	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	}
}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	switch (key)
	{
	case 32: // the space bar
		break;
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	}
}

void Game::mouseClicked(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		switch (button)
		{
		case GLUT_LEFT_BUTTON:

			break;
		case GLUT_RIGHT_BUTTON:

			break;
		case GLUT_MIDDLE_BUTTON:

			break;
		}
	}
	else
	{

	}
}

/*
* mouseMoved(x,y)
* - this occurs only when the mouse is pressed down
*   and the mouse has moved.  you are given the x,y locations
*   in window coordinates (from the top left corner) and thus
*   must be converted to screen coordinates using the screen to window pixels ratio
*   and the y must be flipped to make the bottom left corner the origin.
*/
void Game::mouseMoved(int x, int y)
{
}
