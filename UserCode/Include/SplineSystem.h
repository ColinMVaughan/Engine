#pragma once

#include <BaseSystem.h>

#include <ComponentReflection.h>
#include <SystemReflection.h>
#include <PhysicsSystem.h>

#include <glm/gtc/quaternion.hpp>
#include <cereal/types/vector.hpp>


struct SplineNode
{
	SplineNode(glm::vec3 pos)
		:Position(pos) {}

	SplineNode() = default;


	template<typename Archive>
	void serialize(Archive& arc)
	{
		arc(Position.x, Position.y, Position.z, OrientationTargetIndex);
	}

	glm::vec3 Position;
	unsigned int OrientationTargetIndex = 0;
};

class SplineComponent
{
public:
	std::vector<SplineNode> nodes;
	std::vector<SplineNode> LookTargets;

	float time = 0.1f;
	float speed = 0.30f;

	void ExposeToEditor();

	template<typename Archive>
	void serialize(Archive& arc)
	{
		arc(nodes);
	}

	glm::vec3 GetSplinePoint(float t);
	glm::quat GetSplineOrientation(float t);

private:
	glm::quat LookAtNode(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0, 1, 0));

	//Creates a Quaternion rotated towards the direction vector
	glm::quat QuaternionLookAt(const glm::vec3& direction, const glm::vec3& up);
};

USER_COMPONENT_REGISTER(SplineComponent, "SplineComponent")


class SplineMoveSystem : public ECS::System<Transform, SplineComponent>
{
public:
	SplineMoveSystem(ECS::ComponentManager* cManager, EventManager& eManager);

	void Update(double deltaTime, ECS::Entity& entity) override;

	float GetT(float t, glm::vec3 p0, glm::vec3 p1);
	void EntityRegistered(ECS::Entity& entity) override;
	void DrawGizmo(ECS::Entity& entity) override;

	std::vector<glm::vec3> pathDrawBuffer;
	DrawGizmoEvent m_DrawSplineEvent;
	DrawGizmoEvent m_DrawNodeEvent;
	DrawGizmoEvent m_DrawTargetEvent;

	glm::mat4 mat;
	Mesh m_pathMesh;
	Mesh m_nodeMesh;
};

USER_SYSTEM_REGISTER(SplineMoveSystem, "SplineMoveSystem")