#pragma once
#include "Mesh.h"
#include "Renderer.h"
#include "BaseSystem.h"
#include <PxPhysicsAPI.h>

//-----------------------------------------------------
//	RenderSystem is used in conjunction with ECS to fit old code into
//	the new framework.
//
//					How to Use:
//	Add system to system Manager, and send avalid Renderer pointer.
//-----------------------------------------------------

class RenderSystem : public ECS::System<Mesh, Material, Transform>
{
private:
	Renderer* m_Renderer;
public:

	RenderSystem(ECS::ComponentManager* a_Cmanager)
	:System(a_Cmanager){}

	Renderer* GetRenderer()
	{
		return m_Renderer;
	}

	//THIS IS BAD! LETS FIX THIS.
	void SetRenderer(Renderer* a_renderer)
	{
		if (a_renderer != nullptr)
			m_Renderer = a_renderer;
		else
			std::cout << "\nDid not send RenderSystem a valid Renderer";
	}

	void PreUpdate(double deltaTime) override
	{
		m_Renderer->PreRender();
	}

	void Update(double deltaTime, ECS::Entity& entity) override
	{
		auto mat =entity.GetComponent<Transform>()->GetGlobalTransformMatrix();
		m_Renderer->Render(entity.GetComponent<Mesh>(), entity.GetComponent<Material>(), mat.front());
	}
	void PostUpdate(double deltaTime) override
	{
		m_Renderer->PointLightPass();
		m_Renderer->SSAOPass();
		m_Renderer->CombineLighting();
	}

};

//-----------------------------------------------------------------------------
//						Point Light System
//-----------------------------------------------------------------------------
class PointLightComponent
{
public:
	glm::fvec3 position;
	glm::fvec3 Color;
};

class PointLightSystem : public ECS::System<PointLightComponent, Transform>
{
public:

	PointLightSystem(ECS::ComponentManager* a_Cmanager)
		:System(a_Cmanager) {}

	void Update(double deltaTime, ECS::Entity& entity) override
	{
		PxVec3 Pos =entity.GetComponent<Transform>()->GetTransform()->p;
		PointLightComponent* light = entity.GetComponent<PointLightComponent>();

		light->position[0] = Pos.x;
		light->position[1] = Pos.y;
		light->position[2] = Pos.z;

	}

};

//-------------------------------------------------------------------------------------------------
//								Directional Light System
//-------------------------------------------------------------------------------------------------

class DirectionalLight
{
public:
	glm::vec3 Colour;
	glm::vec3 Direction;
	bool CastsShadow;

private:
	FrameBuffer m_ShadowMap;
};