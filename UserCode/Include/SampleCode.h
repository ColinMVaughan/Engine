#pragma once
#include <BaseSystem.h>
#include <PhysicsSystem.h>
#include <ComponentReflection.h>
#include <SystemReflection.h>
#include <RenderSystem.h>

class ShiftComponent
{
public:
	void ExposeToEditor();
	float ShiftSpeed = 0.1;
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

private:
	double time = 0;
};
USER_SYSTEM_REGISTER(LightShiftSystem, "LightShiftSystem")




class SplineComponent
{
public:
	std::vector<glm::vec3> nodes;
	uint8_t current_node;

	glm::vec3 GetSplinePoint(float t)
	{
		int p0, p1, p2, p3;
		p1 = (int)t + 1;
		p2 = p1 + 1;
		p3 = p2 + 1;
		p0 = p1 - 1;

		//get floor of float
		t = t - (int)t;

		//cache some pow(t) because we will be using it a lot
		float tt = t * t;
		float ttt = tt * t;

		float q1 = -ttt + 2.0f*tt + 2.0f;
		float q2 = 3.0f*ttt - 5.0f*tt + 2.0;
		float q3 = -3.0f * ttt + 4.0f*tt + t;
		float q4 = ttt - tt;

		float tx = 0.5f * (nodes[p0].x * q1 + nodes[p1].x * q2 + nodes[p2].x * q3 + nodes[p3].x * q4);
		float ty = 0.5f * (nodes[p0].y * q1 + nodes[p1].y * q2 + nodes[p2].y * q3 + nodes[p3].y * q4);
		float tz = 0.5f * (nodes[p0].z * q1 + nodes[p1].z * q2 + nodes[p2].z * q3 + nodes[p3].z * q4);

		return glm::vec3(tx, ty, tz);
	}

private:
};


class SplineMoveSystem : public ECS::System<Transform, PointLightComponent>
{
public:
	SplineMoveSystem(ECS::ComponentManager* cManager, EventManager& eManager)
		:System(cManager, eManager) {}


	void Update(double deltaTime, ECS::Entity& entity) override
	{


		float t0 = 0.0f;
		//float t1 = GetT(t0)
	}


	float GetT(float t, glm::vec3 p0, glm::vec3 p1)
	{
		float a = pow((p1.x - p0.x), 2.0f) + pow((p1.y - p0.y), 2.0f) + pow((p1.z-p0.z), 2.0f);
		float b = pow(a, 0.5f);
		float c = pow(b, 0.5); //alpha

		return (c + t);
	}

	void EntityRegistered(ECS::Entity& entity) override
	{
		auto spline = entity.GetComponent<SplineComponent>();
		auto transform = entity.GetComponent<Transform>();

		auto p = transform->GetTransform()->p;
		glm::vec3 position(p.x, p.y, p.z);

		spline->nodes.push_back(position);
	}


	void draw()
	{
		for (int t = 0.0f; t < 1.0f; t += 0.05)
		{

		}
	}
};