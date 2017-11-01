#pragma once
#include <windows.h>
#include <GMath/MathLibrary.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Renderer\Renderer.h>


#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


#define WINDOW_WIDTH		800
#define WINDOW_HEIGHT		600
#define FRAMES_PER_SECOND	60
#define BLOOM_THRESHOLD		0.35f
#define BLOOM_DOWNSCALE		4.0f
#define BLOOM_BLUR_PASSES	2
#define SHADOW_RESOLUTION	4096

class Game
{

public:
	typedef GMath::vec3f vec3;
	typedef GMath::vec4f vec4;
	typedef GMath::mat4f mat4;
public:
	Game();
	~Game();

	void initializeGame();
	void update();
	void draw();


	/* input callback functions */
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);

	/* Data Members */
	Timer *updateTimer = nullptr;
	float TotalGameTime = 0.0f;



private:

	Renderer m_Renderer;
	Mesh Orb;
	Camera m_Camera;
	Material m_Materials[5];

	GMath::vec3f lightPos;
	GMath::vec3f lightColor;

	GMath::vec3f lightPos2;
	GMath::vec3f lightColor2;
};
