#pragma once
#include <ComponentReflection.h>
#include <iostream>
#include <imgui.h>
class TestComp
{
public:
	float Number = 0;
	void ExposeToEditor()
	{
		ImGui::DragFloat("Serialization Test", &Number, 0.1f);
	}

	template<typename Archive>
	void serialize(Archive & arc)
	{
		arc(Number);
	}
};

//Registers the component with a string so it can be added int
COMPONENT_REGISTER(TestComp, "TestComp")
