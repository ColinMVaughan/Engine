#pragma once
#include <BaseSystem.h>
#include <PhysicsSystem.h>
#include <ComponentReflection.h>
#include <SystemReflection.h>

class UserComponent
{
public:
	bool isSexy = true;
};
COMPONENT_REGISTER(UserComponent, "UserComponent")


class UserComponent2
{
public:
	bool IsVerySexy = false;
};
COMPONENT_REGISTER(UserComponent2, "MyUserComponent2")


class UserSystem : public ECS::System<UserComponent, Transform>
{
public:
	UserSystem(ECS::ComponentManager* cManager, EventManager& eManager)
		:System(cManager, eManager) {}



	void Update(double deltaTime, ECS::Entity& entity);
	void EntityRegistered(ECS::Entity& entity);
};
SYSTEM_REGISTER(UserSystem, "UserSystem")