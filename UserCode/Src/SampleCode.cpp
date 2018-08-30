#include "SampleCode.h"


void ShiftComponent::ExposeToEditor()
{
	ImGui::SliderFloat("Light Shift Speed", &m_ShiftSpeed, 0.1, 5.0);

	if (ImGui::TreeNodeEx("Colours"))
	{
		for (int i = 0; i < m_Colours.size(); ++i)
		{
			ImGui::ColorEdit3(std::string("Colour " + std::to_string(i)).c_str(), &m_Colours[i].colour.x);
		}

		if (ImGui::Button("+")) { m_Colours.push_back(glm::vec3(0,0,0)); }
		ImGui::SameLine();
		if (ImGui::Button("-")) { m_Colours.pop_back(); }

		ImGui::TreePop();
	}
}

void LightShiftSystem::PreUpdate(double deltaTime)
{
}

void LightShiftSystem::Update(double deltaTime, ECS::Entity & entity)
{
	auto light = entity.GetComponent<PointLightComponent>();
	auto shift = entity.GetComponent<ShiftComponent>();

	if (shift->m_Colours.size() >= 2)
	{
		shift->m_Time += deltaTime * shift->m_ShiftSpeed;
		float t = shift->m_Time;

		//Get the index of the colour we need to access. Use modulo to make sure we wrap around
		//instead of overflowing the index.
		int colourIndex = (int)t % shift->m_Colours.size();
		int colourIndex2 = ((int)t + 1) % shift->m_Colours.size();

		t = t - (int)t;

		light->SetBaseColour(LerpColour(shift->m_Colours[colourIndex].colour, shift->m_Colours[colourIndex2].colour, t));
	}

}

glm::vec3 LightShiftSystem::LerpColour(glm::vec3 colour1, glm::vec3 colour2, float t)
{
	return (1 - t) * colour1 + t * colour2;
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
