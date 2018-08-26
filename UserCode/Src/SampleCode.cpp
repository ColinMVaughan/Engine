#include "SampleCode.h"


void ShiftComponent::ExposeToEditor()
{
	ImGui::SliderFloat("Light Shift Speed", &ShiftSpeed, 0.1, 3.0);
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

void FireflyMotionSystem::Start(ECS::Entity & entity)
{
	auto transform = entity.GetComponent<Transform>();
	auto motion = entity.GetComponent<FireflyMotionComponent>();


	auto pos = transform->GetTransform()->p;
	motion->Target = glm::vec3(pos.x, pos.y, pos.z);
	motion->Motion = glm::vec3(0, 0, 0);
}

void FireflyMotionSystem::Update(double deltaTime, ECS::Entity & entity)
{
	auto transform = entity.GetComponent<Transform>();
	auto motion = entity.GetComponent<FireflyMotionComponent>();

	auto pxPos = transform->GetTransform()->p;
	glm::vec3 position(pxPos.x + 0.01, pxPos.y = + 0.01, pxPos.z + 0.01);

	glm::vec3 difference = position - motion->Target;
	float length = difference.length();

	difference = glm::normalize(difference);

	if (length = 0.0f) { length = 0.000001f; }

	motion->Motion += difference * (1.0f / length) * motion->MovementStrength * (float)deltaTime;
	transform->GetTransform()->p += PxVec3(motion->Motion.x, motion->Motion.y, motion->Motion.z) ;
}

void FireflyMotionSystem::EntityRegistered(ECS::Entity& entity)
{

}
