#pragma once
#include <PhysXWrapper.h>
#include <BaseSystem.h>
#include "Renderer.h"
#include "NullComponent.h"
#include "RenderSystem.h"

//For Handling Physics Simulation
class PhysicsSystem : public ECS::System<Transform, RigidBody>
{
public:
	PhysicsSystem(ECS::ComponentManager* a_CompManager, EventManager& a_EveManager);

	void Start(ECS::Entity& entity) override;
	void Stop(ECS::Entity& entity) override;

	void PreUpdate(double deltaTime) override;
	void Update(double deltaTime, ECS::Entity& entity) override;
	void PostUpdate(double deltaTime) override;

	void EntityRegistered(ECS::Entity& entity) override;


	PhysXWrapper* m_PhysX;
};


//For Handling Physics Collisions
class CollisionSystem : public ECS::System<Transform, RigidBody, Collider>
{
public:
	CollisionSystem(ECS::ComponentManager* a_CompManager, EventManager& a_EveManager);

	void EntityRegistered(ECS::Entity& entity) override;

	void DrawGizmo(ECS::Entity& entity);


	PhysXWrapper* m_Physx;

private:
	Mesh CubeCollisionShape;
	Mesh SphereCollisionShape;
};

//Dummy system to draw arrows for selectes entities with a transform component.
class TransformSystem : public ECS::System<Transform>
{
public: 
	TransformSystem(ECS::ComponentManager* a_CompManager, EventManager& a_EveManager);

	void DrawGizmo(ECS::Entity& entity);

private:
	Mesh ArrowShape;
	physx::PxMat44 rotatex;
	physx::PxMat44 rotatez;

	DrawGizmoEvent Y_meshEvent;
	DrawGizmoEvent X_meshEvent;
	DrawGizmoEvent Z_meshEvent;
};