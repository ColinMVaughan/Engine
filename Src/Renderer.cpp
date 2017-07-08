#include "Renderer.h"
#include "Utilities.h"
#include "Timer.h"
#include "Game.h"


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
	//-----------------------------------------

	if (!StaticGeometry.Load("./Assets/Shaders/CubeMap.vert", "./Assets/Shaders/CubeMap.frag"))
	{
		std::cout << "Shaders failed to initalize.\n";
		system("pause");
		exit(0);
	}

	//Default Gbuffer Shader.
	//TODO: Make this shader customizable
	if (!GBufferPass.Load("./Assets/Shaders/StaticGeometry.vert", "./Assets/Shaders/GBufferPass.frag"))
	{
		std::cout << "Shaders failed to initalize.\n";
		system("pause");
		exit(0);
	}

	if (!LightPassShader.Load("./Assets/Shaders/PassThorugh.vert", "./Assets/Shaders/PBR_Lightpass.frag"))
	{
		std::cout << "Shaders failed to initalize.\n";
		system("pause");
		exit(0);
	}

	if (!LightPassShader.Load("./Assets/Shaders/PassThorugh.vert", "./Assets/Shaders/PBR_Lightpass.frag"))
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
void Renderer::AddPointLight(GMath::vec3f* lightColor, GMath::vec3f* lightpPosition, bool castsShadows)
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
void Renderer::AddDirectionalLight(GMath::vec3f lightColor, GMath::vec3f lightDirection, bool castsShadows)
{

}


void Renderer::PreRender()
{
	//--------------------------------------------------------
	//			Get Ready for Render
	//--------------------------------------------------------

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0);
	LightpassBuffer.Clear();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	GBuffer.Clear();


	glViewport(0, 0, m_WindowWidth, m_WindowHeight);

	GBufferPass.Bind();
	GBufferPass.SendUniformMat4("uModel", &glm::mat4()[0][0], false);
	GBufferPass.SendUniformMat4("uView", &glm::inverse(m_Camera->m_Transform)[0][0], false);
	GBufferPass.SendUniformMat4("uProj", m_Camera->m_Projection.GetData(), false);

	GBufferPass.SendUniform("AO", 4);
	GBufferPass.SendUniform("Normal", 3);
	GBufferPass.SendUniform("Albedo", 2);
	GBufferPass.SendUniform("Roughness", 1);
	GBufferPass.SendUniform("Metallic", 0);
	GBuffer.Bind();
}

//---------------------------------
// Purpose: Renders the scene with the current list of renderables & default shaders
//
//---------------------------------
inline void Renderer::Render(Mesh* mesh, Material* material)
{

	//-------------------------------------------------------------------------------
	//			Render Geometry to GBuffer
	//-------------------------------------------------------------------------------
	//Draw each mesh in Meshlist.

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
	
}


void Renderer::PostRender()
{
	GBufferPass.UnBind();
	GBuffer.UnBind();

	//-----------------------------------------------
	//			Render Skybox
	//----------------------------------------------
	CombinedLighingBuffer.Bind();
	StaticGeometry.Bind();


	StaticGeometry.SendUniformMat4("view", &glm::inverse(m_Camera->m_Transform)[0][0], false);
	StaticGeometry.SendUniformMat4("projection", m_Camera->m_Projection.GetData(), false);
	StaticGeometry.SendUniform("environmentMap", 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMap.TexObj);
	DrawCube();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	StaticGeometry.UnBind();
	CombinedLighingBuffer.UnBind();

	//--------------------------------------------------------
	//			Deffered Lighting Pass
	//--------------------------------------------------------
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	LightpassBuffer.Clear();

	glViewport(0, 0, m_WindowWidth, m_WindowHeight);

	for (int i = 0; i < m_PointLightPositions.size(); ++i)
	{
		LightPassShader.Bind();

		LightPassShader.SendUniform("albedoMap", 0);
		LightPassShader.SendUniform("normalMap", 1);
		LightPassShader.SendUniform("positionMap", 2);

		LightPassShader.SendUniform("roughnessMap", 3);
		LightPassShader.SendUniform("metallicMap", 4);


		LightPassShader.SendUniform("aoMap", 5);

		LightPassShader.SendUniform("camPos", m_Camera->GetPosition());
		LightPassShader.SendUniform("lightPosition", *m_PointLightPositions[i]);
		LightPassShader.SendUniform("lightColor", *m_PointLightColors[i]);

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
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(5));
		DrawFullScreenQuad();
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

		LightpassBuffer.UnBind();
		LightPassShader.UnBind();
	}

	glDisable(GL_BLEND);
	//--------------------------------------------------------
	//				IBL + Composite Lighting
	//
	// Combines The composite lighpass with the IBL 
	// lighting + Gamma correction for the final image.
	//--------------------------------------------------------
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
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(5));
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

	CombinedLighingBuffer.MoveToBackBuffer(m_WindowWidth, m_WindowHeight);
	//LightpassBuffer.MoveToBackBuffer(m_WindowWidth, m_WindowHeight);
	glutSwapBuffers();
	//------------------------------------------------------------------------------
}