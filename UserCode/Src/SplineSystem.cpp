#include "SplineSystem.h"

#include <RenderSystem.h>
#include <DebugShapes.h>

#include <string>

//SPLINE COMPONENT
//-----------------------------------------------------------------------------------------------------

void SplineComponent::ExposeToEditor()
{
	ImGui::DragFloat("Speed", &speed, 0.01f, 0.0001f);

	for (int i = 0; i < nodes.size(); ++i)
	{
		ImGui::DragFloat3((std::string("Node ") + std::to_string(i)).c_str(), &nodes[i].Position.x, 0.1f);
		ImGui::SameLine();

		//get the label for the combo box, if the target is 0, use "no target", otherwiae use the index
		std::string label = nodes[i].OrientationTargetIndex == 0 ? "No Target" : std::string("Target " + std::to_string(nodes[i].OrientationTargetIndex));

		if (ImGui::BeginCombo(std::string(std::to_string(i) + "Targets").c_str(), label.c_str()))
		{
			if (ImGui::Selectable(std::string("No Target ").c_str())) { nodes[i].OrientationTargetIndex = 0; }

			for (int j = 0; j < LookTargets.size(); ++j)
			{
				if (ImGui::Selectable(std::string("Target " + std::to_string(j)).c_str())) { nodes[i].OrientationTargetIndex = j + 1; }
			}

			ImGui::EndCombo();
		}

	}

	if (nodes.size() > 0)
		if (ImGui::Button("+"))
		{
			//create a new node with the same starting position of the previous node.
			nodes.push_back(SplineNode(glm::vec3(nodes.back().Position)));
		}

	if (nodes.size() > 1)
	{
		ImGui::SameLine(); if (ImGui::Button("-"))
		{
			//Remove the last created node.
			nodes.pop_back();
		}
	}

	if(ImGui::TreeNodeEx("Orientation Targets"))
	{
		for (int i = 0; i < LookTargets.size(); ++i)
		{
			ImGui::DragFloat3((std::string("Target ") + std::to_string(i)).c_str(), &LookTargets[i].Position.x, 0.1f);
		}

		if (ImGui::Button("+"))
		{
			//create a new node with the same starting position of the previous node.
			LookTargets.push_back(SplineNode(glm::vec3(nodes.back().Position)));
		}
		ImGui::TreePop();
	}
}

glm::vec3 SplineComponent::GetSplinePoint(float t)
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

	float q1 = -ttt + 2.0f*tt - t;
	float q2 = 3.0f*ttt - 5.0f*tt + 2.0f;
	float q3 = -3.0f * ttt + 4.0f*tt + t;
	float q4 = ttt - tt;

	float tx = 0.5f * (nodes[p0].Position.x * q1 + nodes[p1].Position.x * q2 + nodes[p2].Position.x * q3 + nodes[p3].Position.x * q4);
	float ty = 0.5f * (nodes[p0].Position.y * q1 + nodes[p1].Position.y * q2 + nodes[p2].Position.y * q3 + nodes[p3].Position.y * q4);
	float tz = 0.5f * (nodes[p0].Position.z * q1 + nodes[p1].Position.z * q2 + nodes[p2].Position.z * q3 + nodes[p3].Position.z * q4);

	return glm::vec3(tx, ty, tz);
}

glm::quat SplineComponent::GetSplineOrientation(float t)
{
	int p0, p1, p2;
	p0 = (int)t;
	p1 = p0 + 1;
	p2 = p1 + 1;



	//glm::quat rotation1;
	//glm::quat rotation2;
	//if the orientation node is not overriden
	if (false)
	{

		////Calculate a quaternion pointing towards the next node
		//rotation1 = LookAtNode(GetSplinePoint(t), GetSplinePoint((float)p1 - 0.0001));

		////calculate the direction quaternion for the next node
		//if (p2 < nodes.size() - 2)
		//	rotation2 = LookAtNode(GetSplinePoint(t), GetSplinePoint((float)p2 - 0.0001f));
		//else
		//	rotation2 = LookAtNode(GetSplinePoint(t), GetSplinePoint((float)p1 - 0.0001f));
		////Interpolate between the two quaternions

		//t = t - (int)t;
		//return glm::slerp(rotation1, rotation2, t);
	}
	else
	{
		//if (p1 < nodes.size() - 2)
		//	rotation1 = LookAtNode(GetSplinePoint(t), GetSplinePoint(t + 0.1f));
		//else
		//	rotation1 = glm::inverse(LookAtNode(GetSplinePoint(t), GetSplinePoint(t - 0.001)));

		//return rotation1;
	}
//-----------------------------------------------------------------------------------------------

	glm::quat targetRot[2];

	for (int i = 0; i < 2; ++i)
	{
		if (nodes[p0 + i].OrientationTargetIndex != 0)
		{
			auto target = LookTargets[nodes[p0 + i].OrientationTargetIndex - 1];
			targetRot[i] = LookAtNode(GetSplinePoint(t), target.Position);
		}
		else
		{
			if (p0 + i >= nodes.size() - 2) { targetRot[i] = glm::inverse(LookAtNode(GetSplinePoint(t), GetSplinePoint(t - 0.001))); continue; }
			targetRot[i] = LookAtNode(GetSplinePoint(t), GetSplinePoint(t + 0.1f));
		}
	}

	t = t - (int)t;
	return glm::slerp(targetRot[0], targetRot[1], t);
}

glm::quat SplineComponent::LookAtNode(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{

	auto matrix = glm::lookAt(position, target, up);
	matrix = glm::transpose(matrix);

	return glm::quat_cast(matrix);
}

//Creates a Quaternion rotated towards the direction vector
glm::quat SplineComponent::QuaternionLookAt(const glm::vec3& direction, const glm::vec3& up)
{
	glm::vec3 xAxis = glm::cross(direction, up);
	xAxis = glm::normalize(xAxis);

	glm::vec3 yAxis = glm::cross(direction, xAxis);
	yAxis = glm::normalize(yAxis);

	glm::mat3 rotationMatrix;

	rotationMatrix[0].x = xAxis.x;
	rotationMatrix[0].y = yAxis.x;
	rotationMatrix[0].z = direction.x;

	rotationMatrix[1].x = xAxis.y;
	rotationMatrix[1].y = yAxis.y;
	rotationMatrix[1].z = direction.y;

	rotationMatrix[2].x = xAxis.z;
	rotationMatrix[2].y = yAxis.z;
	rotationMatrix[2].z = direction.z;


	return glm::quat_cast(rotationMatrix);
}


//SPLINE SYSTEM
//------------------------------------------------------------------------------

SplineMoveSystem::SplineMoveSystem(ECS::ComponentManager* cManager, EventManager& eManager)
	:System(cManager, eManager)
{
	m_DrawSplineEvent.colour = glm::vec3(1, 0, 0);
	m_DrawSplineEvent.drawMode = GL_LINES;
	m_DrawSplineEvent.matrix = &mat[0][0];
	m_DrawSplineEvent.mesh = &m_pathMesh;



	m_DrawNodeEvent.colour = glm::vec3(0, 1, 0);
	m_DrawNodeEvent.drawMode = GL_TRIANGLES;

	m_DrawTargetEvent.colour = glm::vec3(0, 0, 1);
	m_DrawTargetEvent.drawMode = GL_TRIANGLES;

	LoadDebugSphere(m_nodeMesh);
	m_DrawNodeEvent.mesh = &m_nodeMesh;
	m_DrawTargetEvent.mesh = &m_nodeMesh;
}

void SplineMoveSystem::Update(double deltaTime, ECS::Entity& entity)
{
	//Get Components
	auto spline = entity.GetComponent<SplineComponent>();
	auto transform = entity.GetComponent<Transform>();

	if (spline->nodes.size() >= 4)
	{
		//if the current t is greater than the max node - 1, it will be out of range and raise an exception
		if (spline->time >= spline->nodes.size() - 3)
			spline->time = 0.1f; //reset T

		auto newPos = spline->GetSplinePoint(spline->time);
		auto newRot = spline->GetSplineOrientation(spline->time);
		*transform->GetTransform() = PxTransform(PxVec3(newPos.x, newPos.y, newPos.z), PxQuat(newRot.x, newRot.y, newRot.z, newRot.w));

		spline->time += deltaTime * spline->speed;
	}

}

float SplineMoveSystem::GetT(float t, glm::vec3 p0, glm::vec3 p1)
{
	float a = pow((p1.x - p0.x), 2.0f) + pow((p1.y - p0.y), 2.0f) + pow((p1.z - p0.z), 2.0f);
	float b = pow(a, 0.5f);
	float c = pow(b, 0.5); //alpha

	return (c + t);
}

void SplineMoveSystem::EntityRegistered(ECS::Entity& entity)
{
	auto spline = entity.GetComponent<SplineComponent>();
	auto transform = entity.GetComponent<Transform>();

	auto p = transform->GetTransform()->p;
	glm::vec3 position(p.x, p.y, p.z);

	spline->nodes.push_back(position);
}

void SplineMoveSystem::DrawGizmo(ECS::Entity& entity)
{
	auto spline = entity.GetComponent<SplineComponent>();
	auto transform = entity.GetComponent<Transform>();
	pathDrawBuffer.clear();

	for (float t = 0.0f; t < (float)spline->nodes.size() - 3.05f; t += 0.05f)

		//for (float t = 0.0f; t < 0.95f; t += 0.05)
	{
		pathDrawBuffer.push_back(spline->GetSplinePoint(t));
		pathDrawBuffer.push_back(spline->GetSplinePoint(t + 0.05f));
	}

	//Load the path into a vertex buffer
	LoadDebugLineStrip(m_pathMesh, &pathDrawBuffer[0], pathDrawBuffer.size());
	entity.DispatchEvent<DrawGizmoEvent>(m_DrawSplineEvent);



	for (int i = 0; i < spline->nodes.size(); ++i)
	{
		glm::mat4 tempMat;

		tempMat = glm::translate(tempMat, glm::vec3(spline->nodes[i].Position));
		tempMat = glm::scale(tempMat, glm::vec3(0.1, 0.1, 0.1));
		m_DrawNodeEvent.matrix = &tempMat[0][0];

		entity.DispatchEvent<DrawGizmoEvent>(m_DrawNodeEvent);
	}

	for (int i = 0; i < spline->LookTargets.size(); ++i)
	{
		glm::mat4 tempMat;

		tempMat = glm::translate(tempMat, glm::vec3(spline->LookTargets[i].Position));
		tempMat = glm::scale(tempMat, glm::vec3(0.1, 0.1, 0.1));
		m_DrawTargetEvent.matrix = &tempMat[0][0];

		entity.DispatchEvent<DrawGizmoEvent>(m_DrawTargetEvent);
	}
}