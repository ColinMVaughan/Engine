#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H
#include <ECS.h>
#include <PhysicsSystem.h>
#include <glm\mat4x4.hpp>

class CameraComp
{
public:

	glm::mat4 Projection;
};


class CameraSystem : public System<CameraComp, Transform>
{

};


#endif