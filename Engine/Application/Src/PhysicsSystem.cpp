#include <PhysicsSystem.h>
#include <ECS.h>
#include "RenderSystem.h"
#include "DebugShapes.h"



PhysicsSystem::PhysicsSystem(ECS::ComponentManager* a_CompManager, EventManager& a_EveManager)
	: System(a_CompManager, a_EveManager)
{

}

void PhysicsSystem::Start(ECS::Entity & entity)
{
	auto rigid = entity.GetComponent<RigidBody>();
	auto transform = entity.GetComponent<Transform>();

	rigid->m_RigidBody->setGlobalPose(*transform->GetTransform());
	rigid->m_RigidBody->setAngularVelocity(PxVec3(0));
	rigid->m_RigidBody->setLinearVelocity(PxVec3(0));
}

void PhysicsSystem::Stop(ECS::Entity & entity)
{
}

void PhysicsSystem::PreUpdate(double deltaTime)
{
	m_PhysX->StepPhysics(deltaTime);
}

//Update entity transforms based on rigidbody simulation
void PhysicsSystem::Update(double deltaTime, ECS::Entity & entity)
{
	auto transform = entity.GetComponent<Transform>();
	auto rigidbody = entity.GetComponent<RigidBody>();

	*transform->GetTransform() = rigidbody->m_RigidBody->getGlobalPose();
}

void PhysicsSystem::PostUpdate(double deltaTime)
{
}

void PhysicsSystem::EntityRegistered(ECS::Entity & entity)
{
	//Get Components
	auto rigidbody = entity.GetComponent<RigidBody>();
	auto transform = entity.GetComponent<Transform>();

	//Create Rigidbody
	auto rigid = m_PhysX->GetPhysics()->createRigidDynamic(*transform->GetTransform());

	rigid->setMass(rigidbody->m_Mass);
	rigid->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, rigidbody->m_IsKinematic);


	m_PhysX->GetScene()->addActor(*rigid);

	rigidbody->m_RigidBody = rigid;

	//Add collision shape if it exists and is initalized
	auto collider = entity.GetComponent<Collider>();
	if(collider && collider->m_CollisionShape)
	{
		rigidbody->m_RigidBody->attachShape(*collider->m_CollisionShape);
	}
}

CollisionSystem::CollisionSystem(ECS::ComponentManager * a_CompManager, EventManager & a_EveManager)
	:System(a_CompManager, a_EveManager)
{
	LoadDebugCube(CubeCollisionShape);
	LoadDebugSphere(SphereCollisionShape);
}

void CollisionSystem::EntityRegistered(ECS::Entity & entity)
{
	auto rigid = entity.GetComponent<RigidBody>();
	auto collider = entity.GetComponent<Collider>();

	//Create physics material and shape
	collider->m_Material = m_Physx->GetPhysics()->createMaterial(collider->m_StaticFriction, collider->m_DynamicFriction, collider->m_Restitution);
	collider->m_CollisionShape = m_Physx->GetPhysics()->createShape(collider->m_Geometry, *collider->m_Material, true);

	//check that rigidbody is registered, and has no other shapes attached.
	if(rigid->m_RigidBody && rigid->m_RigidBody->getNbShapes() < 1)
	{
		//Attack shape to rigidbody
		rigid->m_RigidBody->attachShape(*collider->m_CollisionShape);
	}
}

void CollisionSystem::DrawGizmo(ECS::Entity& entity)
{
	//Get relevant components
	auto transform = entity.GetComponent<Transform>();
	auto collider = entity.GetComponent<Collider>();

	//get transform matrix
	auto matrix = transform->GetGlobalTransformMatrix();
	DrawGizmoEvent meshEvent;

	//Depending on the shape, scale the matrix differently
	switch(collider->m_Geometry.getType())
	{
	case PxGeometryType::eBOX:
	{
		PxBoxGeometry box;
		collider->m_CollisionShape->getBoxGeometry(box);
		matrix.scale(PxVec4(box.halfExtents, 1.0f));
		meshEvent.mesh = &CubeCollisionShape;
	}
		break;

	case PxGeometryType::eSPHERE:
	{
		PxSphereGeometry sphere;
		collider->m_CollisionShape->getSphereGeometry(sphere);
		matrix.scale(PxVec4(sphere.radius, sphere.radius, sphere.radius, 1.0));
		meshEvent.mesh = &SphereCollisionShape;
	}
		break;
	}

	//set the matrix and dispatch the mesh to be rendered

	PxMat44 newMatrix = PxMat44(collider->m_CollisionShape->getLocalPose()) * matrix;
	meshEvent.matrix = newMatrix.front();

	entity.DispatchEvent<DrawGizmoEvent>(meshEvent);
}