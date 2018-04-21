#pragma once
#include <ComponentReflection.h>
#include <BaseSystem.h>
#include <iostream>
#include <imgui.h>
#include <Random.h>
#include <random>

class TestComp
{
public:
	float Number = 0;
	void ExposeToEditor()
	{
		ImGui::DragFloat("Serialization Test", &Number, 0.1f);
		if (ImGui::Button("Trigger Event"))
			Triggered = true;
	}

	template<typename Archive>
	void serialize(Archive & arc)
	{
		arc(Number);
	}

	bool Triggered = false;
};
//Registers the component with a string so it can be added int
COMPONENT_REGISTER(TestComp, "TestComp")

class ButtonClicked : public IEvent
{
public:
	bool Triggered = true;
};


//-----------------------------------------------------------------------------
class EventSystemTest : public ECS::System<TestComp>
{
public:
	EventSystemTest(ECS::ComponentManager* a_comp, EventManager& a_eve)
		:System(a_comp, a_eve) 
	{
		//REGISTER_EVENT_LISTNER(ButtonClicked, EventSystemTest::EventReciever, a_eve)
	}


	void Update(double deltaTime, ECS::Entity& entity) override
	{
		
	}

	void EntityRegistered(ECS::Entity& entity) override
	{
		std::cout << "Entity Registered";
	}

	void EventReciever(const ButtonClicked& test)
	{
		std::cout << "Event Triggered\n";
		Triggered = test.Triggered;
	}

private:
	bool Triggered = false;
};



//---------------------------------------------------------


class BlockResetSystem : public ECS::System<Transform, RigidBody, Collider, TestComp>
{
public:

	BlockResetSystem(ECS::ComponentManager* a_comp, EventManager& a_eve):System(a_comp, a_eve)
	{
		Position = PxVec3(0.0, 6.0, 0.0);
		distribution = std::uniform_real_distribution<float>(-4.0f, 4.0f);
	}


	void PreStart() override
	{
		running = true;
	}

	void Update(double deltaTime, ECS::Entity& entity) override
	{
		auto rigid = entity.GetComponent<RigidBody>();
		auto transform = entity.GetComponent<Transform>();


		if((rigid->m_RigidBody->isSleeping() || rigid->m_RigidBody->getGlobalPose().p.y < -30.0f) && running)
		{
			rigid->m_RigidBody->setLinearVelocity(PxVec3(0));
			rigid->m_RigidBody->setAngularVelocity(PxVec3(0));
			rigid->m_RigidBody->setGlobalPose(PxTransform(Position + PxVec3(distribution(generator), distribution(generator), distribution(generator))));
		}
	}

	void PreStop() override
	{
		running = false;
	}

private:

	PxVec3 spawnRange;
	PxVec3 Position;

	bool running = false;

	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution;
};