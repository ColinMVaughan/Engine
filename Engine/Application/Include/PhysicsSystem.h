#pragma once
#include <PhysXWrapper.h>
#include <BaseSystem.h>


class PhysicsSystem : public ECS::System<Transform, RigidBody>
{
public:
	PhysicsSystem(ECS::ComponentManager* a_CompManager, EventManager& a_EveManager);


	void PreUpdate(double deltaTime) override;
	void Update(double deltaTime, ECS::Entity& entity) override;
	void PostUpdate(double deltaTime) override;

	void EntityRegistered(ECS::Entity& entity) override;

private:
	PhysXWrapper* m_PhysX;
};


//For Handling Physics Collisions
class CollisionSystem : public ECS::System<Transform, RigidBody, Collider>
{
public:
	CollisionSystem(ECS::ComponentManager* a_CompManager, EventManager& a_EveManager);

	void EntityRegistered(ECS::Entity& entity) override;

private:
	PhysXWrapper* m_Physx;
};