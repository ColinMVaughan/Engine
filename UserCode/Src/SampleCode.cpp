#include "SampleCode.h"


void ShiftComponent::ExposeToEditor()
{
	//std::cout << "At least this is working\n";
	ImGui::Text("Ok Just ckecking now!"); 
	ImGui::SliderFloat("Light Shift Speed", &ShiftSpeed, 0.1, 5);
}

void LightShiftSystem::PreUpdate(double deltaTime)
{
	time += deltaTime;
}

void LightShiftSystem::Update(double deltaTime, ECS::Entity & entity)
{
	auto light = entity.GetComponent<PointLightComponent>();
	auto shift = entity.GetComponent<ShiftComponent>();

	const float pi = 3.14;
	const float frequency = shift->ShiftSpeed; // Frequency in Hz

	light->Color.r = 125 * (1 + sin(2 * pi * frequency * time));
	light->Color.g = 125 * (1 + sin(4 * pi * frequency * time));
	light->Color.b = 125 * (1 + sin(1 * pi * frequency * time));
}
