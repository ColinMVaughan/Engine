#include "Renderer.h"
#include "Utilities.h"
#include "Timer.h"
#include "Game.h"
#include <random>

//---------------------------------------------------
// Purpose: Initalizes the Renderer's default shaders and framebuffers
//
// TODO: Allow for user submitted shaders
//---------------------------------------------------
void Renderer::Initalize()
{

	m_UpdateTimer = new Timer();
	InitFullScreenQuad();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	//-----------------------------------------

	if (!StaticGeometry.Load("./Assets/Shaders/CubeMap.vert", "./Assets/Shaders/CubeMap.frag"))
	{
		std::cout << "Shaders failed to initalize.\n";
		system("pause");
		exit(0);
	}

	//Default Gbuffer Shader.
	if (!GBufferPass.Load("./Assets/Shaders/StaticGeometry.vert", "./Assets/Shaders/GBufferPass.frag"))
	{
		std::cout << "Shaders failed to initalize.\n";
		system("pause");
		exit(0);
	}

	if (!SSAO.Load("./Assets/Shaders/PassThorugh.vert", "./Assets/Shaders/SSAO.frag"))
	{
		std::cout << "Shaders failed to initalize.\n";
		system("pause");
		exit(0);
	}

	if (!GBufferPassInstanced.Load("./Assets/Shaders/InstancedGeometry.vert", "./Assets/Shaders/GBufferPass.frag"))
	{
		std::cout << "Shaders failed to initalize.\n";
		system("pause");
		exit(0);
	}

	if (!PointLightPassShader.Load("./Assets/Shaders/PassThorugh.vert", "./Assets/Shaders/PBR_Lightpass.frag"))
	{
		std::cout << "Shaders failed to initalize.\n";
		system("pause");
		exit(0);
	}

	if (!DirectionalLightPassShader.Load("./Assets/Shaders/PassThorugh.vert", "./Assets/Shaders/PBR_Lightpass.frag"))
	{
		std::cout << "Shaders failed to initalize.\n";
		system("pause");
		exit(0);
	}

	if (!LightingCombinedShader.Load("./Assets/Shaders/PassThorugh.vert", "./Assets/Shaders/LightingCombined.frag"))
	{
		std::cout << "Shaders failed to initalize.\n";
		system("pause");
		exit(0);
	}

	if (!UICombinedShader.Load("./Assets/Shaders/PassThorugh.vert", "./Assets/Shaders/Basic.frag"))
	{
		std::cout << "Shaders failed to initalize.\n";
		system("pause");
		exit(0);
	}

	//Initalize Gbuffer
	//-------------------------------------------------------------------------------------------------------------------------------------------------
	GBuffer.InitDepthTexture(m_WindowWidth, m_WindowHeight);
	GBuffer.InitColorTexture(0, m_WindowWidth, m_WindowHeight, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);	//Flat Color  ///Might fuck things up!!!
	GBuffer.InitColorTexture(1, m_WindowWidth, m_WindowHeight, GL_RGB32F, GL_NEAREST, GL_CLAMP_TO_EDGE);	//Normals (xyz)
	GBuffer.InitColorTexture(2, m_WindowWidth, m_WindowHeight, GL_RGB32F, GL_NEAREST, GL_CLAMP_TO_EDGE);	//ViewSpace Positions (xyz)
	GBuffer.InitColorTexture(3, m_WindowWidth, m_WindowHeight, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);	//Roughness Factor
	GBuffer.InitColorTexture(4, m_WindowWidth, m_WindowHeight, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);	//Metallic Factor
	GBuffer.InitColorTexture(5, m_WindowWidth, m_WindowHeight, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);	//AO Factor
	if (!GBuffer.CheckFBO())
	{
		std::cout << "FBO Failed to Initalize.\n";
		system("pause");
		exit(0);
	}

	//--------------------------------------------------------------------------------------------------------------------------------------------------
	LightpassBuffer.InitColorTexture(0, m_WindowWidth, m_WindowHeight, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	if (!LightpassBuffer.CheckFBO())
	{
		std::cout << "FBO Failed to Initalize.\n";
		system("pause");
		exit(0);
	}

	CombinedLighingBuffer.InitColorTexture(0, m_WindowWidth, m_WindowHeight, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	if(!CombinedLighingBuffer.CheckFBO())
	{
		std::cout << "Combined Lighing FBO failed to init.\n";
		system("pause");
		exit(0);
	}

	UIBuffer.InitColorTexture(0, m_WindowWidth, m_WindowHeight, GL_RGBA8, GL_LINEAR, GL_CLAMP_TO_EDGE);
	if (!UIBuffer.CheckFBO())
	{
		std::cout << "UI FBO failed to initalize. \n";
		system("pause");
		exit(0);
	}
	InitalizeSSAO();
}

void Renderer::InitalizeDefaultMaterial()
{
	m_DefaultMaterial.Roughness.Load("");
	m_DefaultMaterial.Metallic.Load("");

	m_DefaultMaterial.Albedo.Load("");
	m_DefaultMaterial.Normal.Load("");
	m_DefaultMaterial.AO.Load("");

}

//-----------------------------------------------------------
// Purpose: Loads an HDR equairectangular map and converts it to a cube map.
//			Then creates nessisary textures for IBL rendering.
//
// Params: -filepath to an HDR Equirectangular map.
//------------------------------------------------------------
void Renderer::InitalizePBREnvironmentMaps(std::string filepath)
{
	//Load & convert equarectangular environment map (HDR) to Cube Map
	EnvironmentmapToCubemap(filepath, m_CubeMap);

	//Generate diffuse irradiance map from the environment cube map
	CubemapToIrradianceMap(m_CubeMap, m_IrradianceMap);

	//Genrate prefilter map from cube map
	CubemapToPrefiltermap(m_CubeMap, m_PrefilterMap);

	//Calculate and generate a BRDF lookup texture
	CalculateBRDF(m_BDRFMap);
}

//---------------------------------------------------
// Purpose: Adds mesh to be rendered to the scene with material
// Params: -Pointer to mesh class, -Pointer to material class
//
// TODO: Actually add directional Light
//---------------------------------------------------
void Renderer::AddMesh(Mesh* mesh, Material* material)
{
	MeshList.push_back(mesh);
	MaterialList.push_back(material);
}

//---------------------------------------------------
// Purpose: Adds point light to the scene
// Params: -colour of the light, -direction vector of the light, -if the light casts shadows
//
// TODO: Add safegaurds
//---------------------------------------------------
void Renderer::AddPointLight(glm::fvec3* lightColor, glm::fvec3* lightpPosition, bool castsShadows)
{
	m_PointLightPositions.push_back(lightpPosition);
	m_PointLightColors.push_back(lightColor);
}


//---------------------------------------------------
// Purpose: Adds Directional light to the scene
// Params: -colour of the light, -direction vector of the light, -if the light casts shadows
//
// TODO: Actually add directional Light
//---------------------------------------------------
void Renderer::AddDirectionalLight(glm::fvec3 lightColor, glm::fvec3 lightDirection, bool castsShadows)
{

}


void Renderer::PreRender()
{
	//--------------------------------------------------------
	//			Get Ready for Render
	//--------------------------------------------------------
	glViewport(0, 0, m_WindowWidth, m_WindowHeight);
	glClearColor(0.0, 0.0, 0.0, 0);
	LightpassBuffer.Clear();
	GBuffer.Clear();


	//-----------------------------------------------
	//			Render Skybox
	//----------------------------------------------
	CombinedLighingBuffer.Bind();
	StaticGeometry.Bind();


	StaticGeometry.SendUniformMat4("view", &glm::inverse(m_Camera->m_Transform)[0][0], false);
	StaticGeometry.SendUniformMat4("projection", &m_Camera->m_Projection[0][0], false);
	StaticGeometry.SendUniform("environmentMap", 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMap.TexObj);
	DrawCube();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	StaticGeometry.UnBind();
	CombinedLighingBuffer.UnBind();

}

//---------------------------------
// Purpose: Renders the scene with the current list of renderables & default shaders
//
//---------------------------------
void Renderer::Render(Mesh* mesh, Material* material, const float* matrix)
{
	//-------------------------------------------------------------------------------
	//			Render Geometry to GBuffer
	//-------------------------------------------------------------------------------
	//Draw each mesh in Meshlist.
	ShaderProgram* GBP = &GBufferPass;
	if (mesh->IsInstanced)
	{
		GBP = &GBufferPassInstanced;
	}

	GBP->Bind();
	GBP->SendUniformMat4("uView", &glm::inverse(m_Camera->m_Transform)[0][0], false);
	GBP->SendUniformMat4("uProj", &m_Camera->m_Projection[0][0], false);

	GBP->SendUniform("AO", 4);
	GBP->SendUniform("Normal", 3);
	GBP->SendUniform("Albedo", 2);
	GBP->SendUniform("Roughness", 1);
	GBP->SendUniform("Metallic", 0);
	GBuffer.Bind();
	GBP->SendUniformMat4("uModel", matrix, false);

		glActiveTexture(GL_TEXTURE0);
		material->Metallic.Bind();
		glActiveTexture(GL_TEXTURE1);
		material->Roughness.Bind();
		glActiveTexture(GL_TEXTURE2);
		material->Albedo.Bind();
		glActiveTexture(GL_TEXTURE3);
		material->Normal.Bind();
		glActiveTexture(GL_TEXTURE4);
		material->AO.Bind();

		glBindVertexArray(mesh->VAO);
		if (mesh->IsInstanced)
			glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->GetNumVertices(), mesh->InstanceNumber);
		else
			glDrawArrays(GL_TRIANGLES, 0, mesh->GetNumVertices());
		glBindVertexArray(0);

		material->AO.UnBind();
		glActiveTexture(GL_TEXTURE3);
		material->Normal.UnBind();
		glActiveTexture(GL_TEXTURE2);
		material->Albedo.UnBind();
		glActiveTexture(GL_TEXTURE1);
		material->Roughness.UnBind();
		glActiveTexture(GL_TEXTURE0);
		material->Metallic.UnBind();
		GBP->UnBind();
		GBuffer.UnBind();
	
}

//--------------------------------------------------------
//			Deffered Point Lighting Pass
//--------------------------------------------------------
void Renderer::PointLightPass()
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	PointLightPassShader.Bind();

	PointLightPassShader.SendUniform("albedoMap", 0);
	PointLightPassShader.SendUniform("normalMap", 1);
	PointLightPassShader.SendUniform("positionMap", 2);

	PointLightPassShader.SendUniform("roughnessMap", 3);
	PointLightPassShader.SendUniform("metallicMap", 4);

	PointLightPassShader.SendUniform("camPos", m_Camera->GetPosition());


	LightpassBuffer.Bind();
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(0));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(1));
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(2));
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(3));
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(4));

	for (int i = 0; i < m_PointLightPositions.size(); ++i)
	{
		PointLightPassShader.SendUniform("lightPosition", *m_PointLightPositions[i]);
		PointLightPassShader.SendUniform("lightColor", *m_PointLightColors[i]);

		DrawFullScreenQuad();
	}

	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	LightpassBuffer.UnBind();
	PointLightPassShader.UnBind();

	glDisable(GL_BLEND);

}

void Renderer::CombineUI()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	UICombinedShader.Bind();
	UICombinedShader.SendUniform("uTex", 0);

	CombinedLighingBuffer.Bind();
	glBindTexture(GL_TEXTURE_2D, UIBuffer.GetColorHandle(0));
	DrawFullScreenQuad();
	glBindTexture(GL_TEXTURE_2D, 0);
	CombinedLighingBuffer.UnBind();

	UICombinedShader.Bind();

	glDisable(GL_BLEND);
}

//--------------------------------------------------------
//			Deffered Directional Lighting Pass
//--------------------------------------------------------
void Renderer::DirectionalLightPass()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	DirectionalLightPassShader.Bind();

	DirectionalLightPassShader.SendUniform("albedoMap", 0);
	DirectionalLightPassShader.SendUniform("normalMap", 1);
	DirectionalLightPassShader.SendUniform("positionMap", 2);

	DirectionalLightPassShader.SendUniform("roughnessMap", 3);
	DirectionalLightPassShader.SendUniform("metallicMap", 4);

	DirectionalLightPassShader.SendUniform("camPos", m_Camera->GetPosition());


	LightpassBuffer.Bind();
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(0));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(1));
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(2));
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(3));
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(4));

	for (int i = 0; i < m_PointLightPositions.size(); ++i)
	{
		DirectionalLightPassShader.SendUniform("lightColor", *m_PointLightColors[i]);
		DrawFullScreenQuad();
	}

	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	LightpassBuffer.UnBind();
	DirectionalLightPassShader.UnBind();

	glDisable(GL_BLEND);

}

//--------------------------------------------------------
//				IBL + Composite Lighting
//
// Combines The composite lighpass with the IBL 
// lighting + Gamma correction for the final image.
//--------------------------------------------------------
void Renderer::CombineLighting()
{

	CombinedLighingBuffer.Bind();
	LightingCombinedShader.Bind();

	LightingCombinedShader.SendUniform("albedoMap", 0);
	LightingCombinedShader.SendUniform("normalMap", 1);
	LightingCombinedShader.SendUniform("positionMap", 2);

	LightingCombinedShader.SendUniform("roughnessMap", 3);
	LightingCombinedShader.SendUniform("metallicMap", 4);

	LightingCombinedShader.SendUniform("irradianceMap", 5);
	LightingCombinedShader.SendUniform("prefilterMap", 6);
	LightingCombinedShader.SendUniform("brdfLUT", 7);

	LightingCombinedShader.SendUniform("aoMap", 8);
	LightingCombinedShader.SendUniform("combinedLights", 9);
	LightingCombinedShader.SendUniform("camPos", m_Camera->GetPosition());


	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(0));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(1));
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(2));
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(3));
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(4));
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap.TexObj);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterMap.TexObj);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, m_BDRFMap.TexObj);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, SSAOBuffer.GetColorHandle(0));
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, LightpassBuffer.GetColorHandle(0));
	DrawFullScreenQuad();
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, GL_NONE);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, GL_NONE);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	LightingCombinedShader.UnBind();
	CombinedLighingBuffer.UnBind();

	//------------------------------------------------------------------------------
}


void Renderer::SetCamera(Camera* cam)
{
	m_Camera = cam;
}

void Renderer::InitalizeSSAO()
{
	std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
	std::default_random_engine generator;


	//Generate SSAO samples
	for (int i = 0; i < 64; i++)
	{
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator));
		
		sample = glm::normalize(sample);
		sample += randomFloats(generator);

		float scale = (float)i / 64.0f;
		scale = LERP(0.1f, 0.1f, scale*scale);
		sample *= scale;
		m_SSAOKernal.push_back(sample);
	}

	//Generate SSAO rotations
	std::vector<glm::vec3> ssaoNoise;
	for (int i = 0; i < 16; ++i)
	{
		glm::vec3 noise(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			0.0f);
	}



	//Generate noise Texture
	glGenTextures(1, &m_SSAONoiseTex.TexObj);
	glBindTexture(GL_TEXTURE_2D, m_SSAONoiseTex.TexObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise.data()[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Init SSAO framebuffer
	SSAOBuffer.InitColorTexture(0, m_WindowWidth, m_WindowHeight, GL_RGBA32F, GL_NEAREST, GL_CLAMP);
	if (!SSAOBuffer.CheckFBO())
	{
		std::cout << "FBO Failed to Initalize.\n";
		system("pause");
		exit(0);
	}

}

void Renderer::SSAOPass()
{
	SSAOBuffer.Bind();
	SSAO.Bind();

	SSAO.SendUniformMat4("Projection" ,&m_Camera->m_Projection[0][0], false);
	SSAO.SendUniformMat4("View", &glm::inverse(m_Camera->m_Transform)[0][0], false);
	SSAO.SendUniformArray("Samples", m_SSAOKernal.data(), m_SSAOKernal.size());

	SSAO.SendUniform("PositionTex", 0);
	SSAO.SendUniform("NormalTex", 1);
	SSAO.SendUniform("NoiseTex", 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(2)); //position
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(1)); //normal
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_SSAONoiseTex.TexObj); //noiseTexture
	DrawFullScreenQuad();
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	SSAO.UnBind();
	SSAOBuffer.UnBind();
}

void Renderer::SubmitFrame()
{
	CombinedLighingBuffer.MoveToBackBuffer(m_WindowWidth, m_WindowHeight);
	//UIBuffer.MoveToBackBuffer(m_WindowWidth, m_WindowHeight);
	//SSAOBuffer.MoveToBackBuffer(m_WindowWidth, m_WindowHeight);
	//LightpassBuffer.MoveToBackBuffer(m_WindowWidth, m_WindowHeight);
	//glutSwapBuffers();
	SDL_GL_SwapWindow(m_Window);
}
