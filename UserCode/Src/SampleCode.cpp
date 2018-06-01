#include "SampleCode.h"


void ShiftComponent::ExposeToEditor()
{
	//std::cout << "At least this is working\n";
	ImGui::Text("Ok Just ckecking now!"); 
	ImGui::SliderFloat("Light Shift Speed", &ShiftSpeed, 0.1, 10);
}

void LightShiftSystem::Update(double deltaTime, ECS::Entity & entity)
{
	auto light = entity.GetComponent<PointLightComponent>();
	auto shift = entity.GetComponent<ShiftComponent>();
	shift->ShiftSpeed = 5.0f;

	//light->Color.r = sin(light->Color.r + deltaTime * shift->ShiftSpeed);
	//light->Color.g = cos(light->Color.g + deltaTime * shift->ShiftSpeed);
	//light->Color.b = sin(light->Color.b + deltaTime * shift->ShiftSpeed);
}
