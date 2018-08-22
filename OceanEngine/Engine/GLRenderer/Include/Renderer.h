#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "Timer.h"
#include "FrameBuffer.h"
#include "Material.h"
#include "Utilities.h"

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
	float Exposure = 1.0f;
};

class Renderer
{
public:
	Renderer(unsigned windowHeight, unsigned windowWidth,SDL_Window* window, Camera* camera)
		:m_WindowWidth(windowWidth), m_WindowHeight(windowHeight), 
		m_Camera(camera),m_Window(window),
		GBuffer(7), LightpassBuffer(1), CombinedLighingBuffer(1), SSAOBuffer(1), UIBuffer(1),DebugBuffer(1),PostProccessBuffer(1), FinalBuffer(&CombinedLighingBuffer){}

	void Initalize();
	void InitalizePBREnvironmentMaps(std::string filepath);
	void AddMesh(Mesh* mesh, Material* material);
	void AddPointLight(glm::fvec3* lightColor, glm::fvec3* lightpPsition, bool castsShadows);
	void AddDirectionalLight(glm::fvec3 lightColor, glm::fvec3 lightDirection, bool castsShadows);
	

	void PreRender();
	void Render(Mesh* mesh, Material* material, const float* matrix);
	void RenderVoxel(Mesh* mesh, Texture* texture, const float* matrix);

	inline void PreShadowMapRender() { glViewport(0, 0, 2048, 2048); for (int i = 0; i < m_ShadowMaps.size(); ++i) { m_ShadowMaps[i].Clear(); } }
	void RenderToShadowMap(Mesh* mesh, const float* matrix);
	inline void PostShadowMapRender() { glViewport(0,0,m_WindowWidth, m_WindowHeight); }

	void RenderDebug(Mesh& mesh, const float* matrix, glm::vec3 colour, GLenum renderMode = GL_LINE_STRIP);

	void CombineLighting();
	void CreateUIBuffer(unsigned int width, unsigned int height)
	{
		UIBuffer.InitColorTexture(0, width, height, GL_RGBA8, GL_LINEAR, GL_CLAMP_TO_EDGE);
		if (!UIBuffer.CheckFBO())
		{
			std::cout << "UI FBO failed to initalize. \n";
			system("pause");
			exit(0);
		}
	}

	void PrePointLightPass();
	inline void PointLightPass(glm::fvec3& lightPos, glm::fvec3& lightColour)
	{

		PointLightPassShader.SendUniform("lightPosition", lightPos);
		PointLightPassShader.SendUniform("lightColor", lightColour);

		DrawFullScreenQuad();
		
	}
	void PostPointLightPass();

	bool AddShadowCaster(FrameBuffer** shadowMap, glm::mat4& direction, glm::mat4& projection);

	void CombineUI();
	void CombineDebug();

	void InitalizeSSAO();
	void SSAOPass();

	void InitalizePostProcessing();
	void PostProccessPass();

	void SubmitFrame();

	void InitalizeShadowMapping();
	void PreDirectionalLightPass();
	inline void DirectionalLightPass(glm::fvec3 direction, glm::fvec3 lightColour, FrameBuffer& shadowMap, glm::mat4 someMatrixProbably)
	{
		DirectionalLightPassShader.SendUniform("lightDirection", direction);
		DirectionalLightPassShader.SendUniform("lightColor", lightColour);
		DirectionalLightPassShader.SendUniformMat4("lightSpaceMatrix", &someMatrixProbably[0][0], false);
		DirectionalLightPassShader.SendUniform("shadowMap", 4);

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, shadowMap.GetDepthHandle());
		DrawFullScreenQuad();
	}
	void PostDirectionalLightPass();

	void SetCamera(Camera* cam);
	void SetBufferToDisplay(unsigned int index);

	FrameBuffer UIBuffer;
	FrameBuffer DebugBuffer;
	FrameBuffer GBuffer;
	FrameBuffer SSAOBuffer;
	FrameBuffer* FinalBuffer;
	Camera* m_Camera;
private:
	void InitalizeDefaultMaterial();

	unsigned m_WindowWidth;
	unsigned m_WindowHeight;



	//Mesh and materials
	std::vector<Mesh*> MeshList;
	std::vector<Material*> MaterialList;

	//Lights
	std::vector<glm::fvec3*> m_PointLightColors;
	std::vector<glm::fvec3*> m_PointLightPositions;

	//shadows
	std::vector<FrameBuffer> m_ShadowMaps;
	std::vector<glm::mat4*> m_ShadowTransforms;
	std::vector<glm::mat4*> m_ShadowProjections;
	glm::mat4 m_ShadowMapBias;
	//glm::mat4 m_ShadowOrtho;

	//updateTimer
	Timer* m_UpdateTimer;

	//Framebuffers
	FrameBuffer LightpassBuffer;
	FrameBuffer CombinedLighingBuffer;
	FrameBuffer PostProccessBuffer;


	//Shaders used for rendering
	ShaderProgram StaticGeometry;
	ShaderProgram GBufferPass;
	ShaderProgram GBufferPassInstanced;
	ShaderProgram PointLightPassShader;
	ShaderProgram DirectionalLightPassShader;

	ShaderProgram UICombinedShader;
	ShaderProgram LightingCombinedShader;
	ShaderProgram SSAO;
	ShaderProgram DebugShader;
	ShaderProgram VoxelShader;
	ShaderProgram ShadowCastShader;
	
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
