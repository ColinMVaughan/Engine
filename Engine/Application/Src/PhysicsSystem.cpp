#include <PhysicsSystem.h>
#include <ECS.h>

PhysicsSystem::PhysicsSystem(ECS::ComponentManager* a_CompManager, EventManager& a_EveManager)
	: System(a_CompManager, a_EveManager)
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
	m_PhysX->GetScene()->addActor(*rigid);

	rigidbody->m_RigidBody = rigid;
}

CollisionSystem::CollisionSystem(ECS::ComponentManager * a_CompManager, EventManager & a_EveManager)
	:System(a_CompManager, a_EveManager)
{
}

void CollisionSystem::EntityRegistered(ECS::Entity & entity)
{
}
