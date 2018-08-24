#pragma once
#include <BaseSystem.h>
#include <PhysicsSystem.h>
#include <ComponentReflection.h>
#include <SystemReflection.h>
#include <RenderSystem.h>

#include <string>

#include <glm/gtc/quaternion.hpp>
#include <cereal/types/vector.hpp>

class ShiftComponent
{
public:
	void ExposeToEditor();
	float ShiftSpeed = 0.1;
};
USER_COMPONENT_REGISTER(ShiftComponent, "ShiftComponent")



class LightShiftSystem : public ECS::System<ShiftComponent,PointLightComponent, Transform>
{
public:
	LightShiftSystem(ECS::ComponentManager* cManager, EventManager& eManager)
		:System(cManager, eManager) {}


	void PreUpdate(double deltaTime);
	void Update(double deltaTime, ECS::Entity& entity);
	//void EntityRegistered(ECS::Entity& entity);

private:
	double time = 0;
};
USER_SYSTEM_REGISTER(LightShiftSystem, "LightShiftSystem")


