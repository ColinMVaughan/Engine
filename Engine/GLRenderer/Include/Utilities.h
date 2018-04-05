#pragma once 
#include <string>
#include "Texture.h"
#include "Mesh.h"
#include <glm\mat4x4.hpp>

void InitFullScreenQuad();
void InitCube();

void DrawFullScreenQuad();
void DrawCube();

void LoadCubeMesh(Mesh& a_Mesh);

void EnvironmentmapToCubemap(std::string filepath, Texture& CubeMap);
void CubemapToIrradianceMap(Texture& Cubemap, Texture& Irradiancemap);
void CubemapToPrefiltermap(Texture& Cubemap, Texture& Prefiltermap);
void CalculateBRDF(Texture& brdfLUT);

void LoadAsCube(Mesh& a_Mesh);

glm::vec3 LERP(glm::vec3 vec1, glm::vec3 vec2, float t);
float LERP(float p1, float p2, float t);