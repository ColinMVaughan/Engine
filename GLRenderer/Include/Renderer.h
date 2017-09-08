#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "Timer.h"
#include "FrameBuffer.h"
#include <glm/mat4x4.hpp>
#include <SDL\SDL.h>

//-----------------------------
// Material represents the look and feel of an object.
// use a combination of texture maps to achieve this.
//----------------------------
struct Material
{
	Material(){}
	~Material(){}

	void SetTexturesFromFolder(std::string FolderPath)
	{
		Albedo.Load(FolderPath + "/Albedo.png", GL_NEAREST);
		Normal.Load(FolderPath + "/Normal.png", GL_NEAREST);
		AO.Load(FolderPath + "/AO.png", GL_NEAREST);
		Metallic.Load(FolderPath + "/Metallic.png", GL_NEAREST);
		Roughness.Load(FolderPath + "/Roughness.png", GL_NEAREST);
	}

	void SetAlbedo(){}
	void SetNormal(){}
	void SetAO(){}

	void SetRoughness(){}
	void SetMetallic(){}


	Texture Albedo;
	Texture Normal;
	Texture AO;

	Texture Metallic;
	Texture Roughness;

	static Texture defaultTex;
};

struct PointLight
{
public:
	PointLight();
private:
	glm::vec3 LightPosition;
	glm::vec3 LightColor;
};

class Camera
{
public:
	glm::fvec3 GetPosition()
	{
		glm::vec4 pos(0,0,0,1);
		pos = m_Transform * pos;

		return glm::fvec3({ pos[0], pos[1], pos[2] });
	}


	glm::mat4 m_Projection;
	glm::mat4 m_Transform;
};

class Renderer
{
public:
	Renderer(unsigned windowHeight, unsigned windowWidth,SDL_Window* window, Camera* camera)
		:m_WindowWidth(windowWidth), m_WindowHeight(windowHeight), 
		m_Camera(camera),m_Window(window),
		GBuffer(6), LightpassBuffer(1), CombinedLighingBuffer(1){}

	void Initalize();
	void InitalizePBREnvironmentMaps(std::string filepath);
	void AddMesh(Mesh* mesh, Material* material);
	void AddPointLight(glm::fvec3* lightColor, glm::fvec3* lightpPsition, bool castsShadows);
	void AddDirectionalLight(glm::fvec3 lightColor, glm::fvec3 lightDirection, bool castsShadows);
	

	void PreRender();
	void Render(Mesh* mesh, Material* material, const float* matrix);
	void RenderInstanced();
	void PostRender();

	void SetCamera(Camera* cam);

private:
	void InitalizeDefaultMaterial();


	unsigned m_WindowWidth;
	unsigned m_WindowHeight;

	 Camera* m_Camera;

	//Mesh and materials
	std::vector<Mesh*> MeshList;
	std::vector<Material*> MaterialList;

	//Lights
	std::vector<glm::fvec3*> m_PointLightColors;
	std::vector<glm::fvec3*> m_PointLightPositions;

	//updateTimer
	Timer* m_UpdateTimer;

	//Framebuffers
	FrameBuffer GBuffer;
	FrameBuffer LightpassBuffer;
	FrameBuffer CombinedLighingBuffer;

	//Shaders used for rendering
	ShaderProgram StaticGeometry;
	ShaderProgram GBufferPass;
	ShaderProgram GBufferPassInstanced;
	ShaderProgram LightPassShader;
	ShaderProgram LightingCombinedShader;
	
	//Environment maps
	Texture m_CubeMap;
	Texture m_IrradianceMap;
	Texture m_PrefilterMap;
	Texture m_BDRFMap;

	//Default Material
	Material m_DefaultMaterial;

	//Window Pointer 
	SDL_Window* m_Window;
};
