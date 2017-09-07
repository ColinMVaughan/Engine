#pragma once

//------------------------------------------------
//				DEPRICATED!
//	This class will be replaced by GPU Particles
//------------------------------------------------


#include <vector>
//#include <GMath/MathLibrary.h>
#include <glm\mat4x4.hpp>
#include "Texture.h"

struct ParticleList
{
	glm::fvec3 *Positions = nullptr;
	glm::fvec3 *Velocities = nullptr;
	float *Size = nullptr;
	float *Alpha = nullptr;
	float *Ages = nullptr;
	float *Lifetimes = nullptr;
};

class ParticleEffect
{

public:
	
	typedef glm::mat4 mat4;
	typedef glm::fvec3 vec3;
	typedef glm::fvec2 vec2;

public:
	ParticleEffect();
	~ParticleEffect();

	bool Init(const std::string &textureFile, unsigned int maxParticles, unsigned int rate);

	void Update(float elapsed);
	void Render();

	mat4 Transform;

	vec2 RangeX = vec2({ -10.0f, 10.0f });
	vec2 RangeY = vec2({ 0.0f, 10.0f });
	vec2 RangeZ = vec2({ -10.0f, 10.0f });
	vec2 RangeVelocity = vec2({ 0.1f, 1.0f });
	vec2 RangeLifetime = vec2({ 7.0f, 9.5f });
	//Lerp variables are dynamic based on lifetime of particle
	vec2 LerpAlpha = vec2({ 0.5f, 0.0f });
	vec2 LerpSize = vec2({ 0.0f, 1.0f });

private:
	ParticleList _Particles;
	Texture _Texture;

	float _Rate = 0.0f;
	unsigned int _MaxParticles = 0;
	unsigned int _NumCurrentParticles = 0;

	GLuint _VAO;
	GLuint _VBO_Position;
	GLuint _VBO_Size;
	GLuint _VBO_Alpha;
};
