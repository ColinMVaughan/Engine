#pragma once
#include <BaseSystem.h>
#include <PhysicsSystem.h>
#include <ComponentReflection.h>
#include <SystemReflection.h>
#include <RenderSystem.h>

#include <string>

#include <glm/gtc/quaternion.hpp>
#include <cereal/types/vector.hpp>


class ShiftComponent
{
public:
	struct Colour
	{
		Colour() = default;
		Colour(glm::vec3 col) : colour(col) {}

		template<typename Archive>
		void serialize(Archive& arc)
		{
			arc(colour.x, colour.y, colour.z);
		}

		glm::vec3 colour;
	};

public:
	void ExposeToEditor();
	float m_ShiftSpeed = 1.0f;
	float m_Time = 0.0f;


	template<typename Archive>
	void serialize(Archive& arc)
	{
		arc(m_ShiftSpeed, m_Colours);
	}


	std::vector<Colour> m_Colours;
};
USER_COMPONENT_REGISTER(ShiftComponent, "ShiftComponent")



class LightShiftSystem : public ECS::System<ShiftComponent,PointLightComponent, Transform>
{
public:
	LightShiftSystem(ECS::ComponentManager* cManager, EventManager& eManager)
		:System(cManager, eManager) {}


	void PreUpdate(double deltaTime);
	void Update(double deltaTime, ECS::Entity& entity);
	//void EntityRegistered(ECS::Entity& entity);

	glm::vec3 LerpColour(glm::vec3 colour1, glm::vec3 colour2, float t);
private:
	double time = 0;
};
USER_SYSTEM_REGISTER(LightShiftSystem, "LightShiftSystem")




class FireflyMotionComponent
{
public:
	float MovementStrength = 0.1f;
	glm::vec3 Motion;
	glm::vec3 Target;
};
//USER_COMPONENT_REGISTER(FireflyMotionComponent, "FireflyMotionComp")

class FireflyMotionSystem : public ECS::System<Transform, FireflyMotionComponent>
{
public:
	FireflyMotionSystem(ECS::ComponentManager* cManager, EventManager& eManager)
		:System(cManager, eManager) {}

	void Start(ECS::Entity& entity) override;

	void Update(double deltaTime, ECS::Entity& entity) override;
	void EntityRegistered(ECS::Entity& entity) override;
};
//USER_SYSTEM_REGISTER(FireflyMotionSystem, "FireflyMotionSys")