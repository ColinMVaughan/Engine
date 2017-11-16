#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H
#include <string>
#include <ECS.h>

void LoadSceneFromFile(std::string filePath, ECS::Scene& a_scene);
void SaveSceneToFile(std::string filePath, ECS::Scene& a_scene);

#endif