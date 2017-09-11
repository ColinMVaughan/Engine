#include "Utilities.h"
#include <GL/glew.h>
#include <glm/mat4x2.hpp>
#include "ShaderProgram.h"
#include <glm/gtc/matrix_transform.inl>

#define BUFFER_OFFSET(i) ((char *) + (i))

GLuint _FullScreenQuadVAO = GL_NONE;
GLuint _FullScreenQuadVBO = GL_NONE;

GLuint _CubeVAO = GL_NONE;
GLuint _CubeVBO = GL_NONE;

glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
glm::mat4 captureViews[] =
{
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
};

void InitFullScreenQuad()
{
	float VBO_DATA[] = 
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,

		1.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
	};

	int vertexSize = 6 * 3 * sizeof(float);
	int texCoordSize = 6 * 2 * sizeof(float);

	glGenVertexArrays(1, &_FullScreenQuadVAO);
	glBindVertexArray(_FullScreenQuadVAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &_FullScreenQuadVBO);

	glBindBuffer(GL_ARRAY_BUFFER, _FullScreenQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, vertexSize + texCoordSize, VBO_DATA, GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertexSize));

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);
}

void InitCube()
{
	float VBO_DATA[] =
	{
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	int vertexSize = 6 * 6 * 3 * sizeof(float);
	glGenVertexArrays(1, &_CubeVAO);
	glBindVertexArray(_CubeVAO);

	glEnableVertexAttribArray(0);
	glGenBuffers(1, &_CubeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, _CubeVBO);
	glBufferData(GL_ARRAY_BUFFER, vertexSize, VBO_DATA, GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);

}

void DrawFullScreenQuad()
{
	glBindVertexArray(_FullScreenQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(GL_NONE);
}

void DrawCube()
{
	glBindVertexArray(_CubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(GL_NONE);
}

void CubemapToIrradianceMap(Texture& CubeMap, Texture& IrradianceMap)
{
	unsigned int captureFBO, captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);


	ShaderProgram irradiance;
	irradiance.Load("./Assets/Shaders/EQ_to_Cube.vert", "./Assets/Shaders/IrradianceMap.frag");

	IrradianceMap.CreateCubeMap(32, false);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

	irradiance.Bind();
	irradiance.SendUniform("environmentMap", 0);
	irradiance.SendUniformMat4("projection", &captureProjection[0][0], false);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMap.TexObj);

	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (int i = 0; i<6; ++i)
	{
		irradiance.SendUniformMat4("view", &captureViews[i][0][0], false);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, IrradianceMap.TexObj, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		DrawCube();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	irradiance.UnBind();
}

void CubemapToPrefiltermap(Texture& CubeMap, Texture& PrefilterMap)
{
	
	unsigned int captureFBO, captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);
	
	
	PrefilterMap.CreateCubeMap(128, true);

	ShaderProgram prefilter;
	prefilter.Load("./Assets/Shaders/EQ_to_Cube.vert", "./Assets/Shaders/Prefilter.frag");

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);


	prefilter.Bind();
	prefilter.SendUniform("environmentMap", 0);
	prefilter.SendUniformMat4("projection", &captureProjection[0][0], false);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMap.TexObj);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

	unsigned int maxMipLevel = 5;
	for (unsigned int mip = 0; mip < maxMipLevel; ++mip)
	{
		unsigned int mipWidth = 128 * std::pow(0.5, mip);
		unsigned int mipHeight = 128 * std::pow(0.5, mip);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevel - 1);
		prefilter.SendUniform("roughness", roughness);

		for (int i = 0; i < 6; ++i)
		{
			prefilter.SendUniformMat4("view", &captureViews[i][0][0], false);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, PrefilterMap.TexObj, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			DrawCube();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	prefilter.UnBind();
}

void CalculateBRDF(Texture& BRDFMap)
{
	unsigned int captureFBO, captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	ShaderProgram brdfShader;
	brdfShader.Load("./Assets/Shaders/PassThorugh.vert", "./Assets/Shaders/brdf.frag");

	glGenTextures(1, &BRDFMap.TexObj);
	glBindTexture(GL_TEXTURE_2D, BRDFMap.TexObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, BRDFMap.TexObj, 0);

	glViewport(0, 0, 512, 512);
	brdfShader.Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawFullScreenQuad();

	brdfShader.UnBind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void EnvironmentmapToCubemap(std::string filepath, Texture& CubeMap)
{
	unsigned int captureFBO, captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
	
	
	InitCube();

	ShaderProgram eqConvertShader;
	eqConvertShader.Load("./Assets/Shaders/EQ_to_Cube.vert", "./Assets/Shaders/EQ_to_Cube.frag");

	Texture TexHouse;
	TexHouse.LoadHDR(filepath);
	CubeMap.CreateCubeMap(512, false);


	//------------------------------------------------------------------------------------------

	eqConvertShader.Bind();
	eqConvertShader.SendUniform("equirectangularMap", 0);
	eqConvertShader.SendUniformMat4("projection", &captureProjection[0][0], false);

	glActiveTexture(GL_TEXTURE0);
	TexHouse.Bind();

	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (int i = 0; i<6; ++i)
	{
		eqConvertShader.SendUniformMat4("view", &captureViews[i][0][0], false);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, CubeMap.TexObj, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		DrawCube();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	TexHouse.UnBind();
	eqConvertShader.UnBind();
}


glm::vec3 LERP(glm::vec3 vec1, glm::vec3 vec2, float t)
{
	return vec1 + t * (vec2 - vec1);
}

float LERP(float p1, float p2, float t)
{
	return p1 + t *(p2 - p1);
}