#include <PhysicsSystem.h>
#include <ECS.h>

PhysicsSystem::PhysicsSystem(ECS::ComponentManager* a_CompManager, EventManager& a_EveManager)
	: System(a_CompManager, a_EveManager)
{

}

void PhysicsSystem::Start(ECS::Entity & entity)
{
	auto rigid = entity.GetComponent<RigidBody>();
	auto transform = entity.GetComponent<Transform>();

	rigid->m_RigidBody->setGlobalPose(*transform->GetTransform());
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
