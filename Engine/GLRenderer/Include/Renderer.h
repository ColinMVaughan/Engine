#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "Timer.h"
#include "FrameBuffer.h"
#include "Material.h"

#include <glm/mat4x4.hpp>
#include <SDL\SDL.h>


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
		GBuffer(6), LightpassBuffer(1), CombinedLighingBuffer(1), SSAOBuffer(1), UIBuffer(1){}

	void Initalize();
	void InitalizePBREnvironmentMaps(std::string filepath);
	void AddMesh(Mesh* mesh, Material* material);
	void AddPointLight(glm::fvec3* lightColor, glm::fvec3* lightpPsition, bool castsShadows);
	void AddDirectionalLight(glm::fvec3 lightColor, glm::fvec3 lightDirection, bool castsShadows);
	

	void PreRender();
	void Render(Mesh* mesh, Material* material, const float* matrix);

	void CombineLighting();
	void PointLightPass();
	void CombineUI();

	void InitalizeSSAO();
	void SSAOPass();

	void SubmitFrame();

	void InitalizeShadowMapping();
	void DirectionalLightPass();

	void SetCamera(Camera* cam);

	FrameBuffer UIBuffer;
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
	FrameBuffer SSAOBuffer;


	//Shaders used for rendering
	ShaderProgram StaticGeometry;
	ShaderProgram GBufferPass;
	ShaderProgram GBufferPassInstanced;
	ShaderProgram PointLightPassShader;
	ShaderProgram DirectionalLightPassShader;

	ShaderProgram UICombinedShader;
	ShaderProgram LightingCombinedShader;
	ShaderProgram SSAO;
	
	//Environment maps
	Texture m_CubeMap;
	Texture m_IrradianceMap;
	Texture m_PrefilterMap;
	Texture m_BDRFMap;
	Texture m_SSAONoiseTex;


	//KernalForSsao
	std::vector<glm::vec3> m_SSAOKernal;
	//Default Material
	Material m_DefaultMaterial;

	//Window Pointer 
	SDL_Window* m_Window;
};
