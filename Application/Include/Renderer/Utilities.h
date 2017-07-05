#pragma once 
#include <string>
#include "Texture.h"

void InitFullScreenQuad();
void InitCube();

void DrawFullScreenQuad();
void DrawCube();

void EnvironmentmapToCubemap(std::string filepath, Texture& CubeMap);
void CubemapToIrradianceMap(Texture& Cubemap, Texture& Irradiancemap);
void CubemapToPrefiltermap(Texture& Cubemap, Texture& Prefiltermap);
void CalculateBRDF(Texture& brdfLUT);