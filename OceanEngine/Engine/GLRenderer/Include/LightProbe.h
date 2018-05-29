#ifndef LIGHT_PROBE_H
#define LIGHT_PROBE_H

#include <glm\vec4.hpp>
#include "Utilities.h"
#include "Texture.h"
//Lightprobe holds a position, boundry, and settings 
//used to capture irradience and reflection cubemaps for IBL
class LightProbe
{
public:
	void ExposeToEditor();

public:
	glm::vec4 AABB;
	Texture Cubemap;
};

#endif