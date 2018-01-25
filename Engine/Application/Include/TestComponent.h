#pragma once
#include <ComponentReflection.h>
#include <BaseSystem.h>
#include <iostream>
#include <imgui.h>
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

class TestEvent : public IEvent
{
public:
	bool Triggered = true;
};


//-----------------------------------------------------------------------------
class EventSystemTest : public ECS::System<TestComp>
{
public:
	EventSystemTest(ECS::ComponentManager* a_comp, EventManager* a_eve)
		:System(a_comp, a_eve) 
	{
		a_eve->AddListner<TestEvent>(std::bind(&EventSystemTest::EventReciever, this, std::placeholders::_1));
	}


	void Update(double deltaTime, ECS::Entity& entity) override
	{
		
	}

	void EventReciever(const TestEvent& test)
	{
		std::cout << "Event Triggered\n";
		Triggered = test.Triggered;
	}

private:
	bool Triggered = false;
};


