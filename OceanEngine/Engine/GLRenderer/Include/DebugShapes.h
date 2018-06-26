#pragma once
#include "Mesh.h"

void LoadDebugCube(Mesh& a_Mesh);
void LoadDebugSphere(Mesh& a_Mesh);
void LoadDebugCapsule(Mesh& a_Mesh);

void LoadDebugArrows(Mesh& a_Mesh);

void LoadDebugLineStrip(Mesh& a_Mesh, glm::vec3* points, unsigned int pointsSize);