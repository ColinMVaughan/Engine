#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "Timer.h"
#include "FrameBuffer.h"
#include <glm/mat4x4.hpp>

//-----------------------------
// Material represents the look and feel of an object.
// use a combination of texture maps to achieve this.
//----------------------------
struct Material
{
	Material(){}
	~Material(){}

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
	GMath::vec3f GetPosition()
	{
		glm::vec4 pos(0,0,0,1);
		pos = m_Transform * pos;

		return GMath::vec3f({ pos[0], pos[1], pos[2] });
	}


	GMath::mat4f m_Projection;
	glm::mat4 m_Transform;
};

class Renderer
{
public:
	Renderer(unsigned windowHeight, unsigned windowWidth, Camera* camera)
		:m_WindowWidth(windowWidth), m_WindowHeight(windowHeight), 
		m_Camera(camera),
		GBuffer(6), LightpassBuffer(1), CombinedLighingBuffer(1){}

	void Initalize();
	void InitalizePBREnvironmentMaps(std::string filepath);
	void AddMesh(Mesh* mesh, Material* material);
	void AddPointLight(GMath::vec3f* lightColor, GMath::vec3f* lightpPsition, bool castsShadows);
	void AddDirectionalLight(GMath::vec3f lightColor, GMath::vec3f lightDirection, bool castsShadows);
	

	void PreRender();
	void Render(Mesh* mesh, Material* material);
	void PostRender();

private:
	void InitalizeDefaultMaterial();




	unsigned m_WindowWidth;
	unsigned m_WindowHeight;

	 Camera* m_Camera;

	//Mesh and materials
	std::vector<Mesh*> MeshList;
	std::vector<Material*> MaterialList;

	//Lights
	std::vector<GMath::vec3f*> m_PointLightColors;
	std::vector<GMath::vec3f*> m_PointLightPositions;

	//updateTimer
	Timer* m_UpdateTimer;

	//Framebuffers
	FrameBuffer GBuffer;
	FrameBuffer LightpassBuffer;
	FrameBuffer CombinedLighingBuffer;

	//Shaders used for rendering
	ShaderProgram StaticGeometry;
	ShaderProgram GBufferPass;
	ShaderProgram LightPassShader;
	ShaderProgram LightingCombinedShader;
	
	//Environment maps
	Texture m_CubeMap;
	Texture m_IrradianceMap;
	Texture m_PrefilterMap;
	Texture m_BDRFMap;

	//Default Material
	Material m_DefaultMaterial;
};
