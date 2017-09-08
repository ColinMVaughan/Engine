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

class RenderSystem : public System<Mesh, Material, Transform>
{
private:
	Renderer* m_Renderer;
public:

	RenderSystem(ComponentManager* a_Cmanager)
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

	void Update(double deltaTime, unsigned int entity) override
	{
		auto mat = m_CManager->GetComponent<Transform>(entity)->GetGlobalTransformMatrix();
		//mat.scale(PxVec4(1, 1, 1, 1));
		m_Renderer->Render(m_CManager->GetComponent<Mesh>(entity), m_CManager->GetComponent<Material>(entity), mat.front());
	}
	void PostUpdate(double deltaTime) override
	{
		m_Renderer->PostRender();
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

class PointLightSystem : public System<PointLightComponent, Transform>
{
public:

	PointLightSystem(ComponentManager* a_Cmanager)
		:System(a_Cmanager) {}

	void Update(double deltaTime, unsigned int entity) override
	{
		PxVec3 Pos = m_CManager->GetComponent<Transform>(entity)->GetTransform().p;
		PointLightComponent* light = m_CManager->GetComponent<PointLightComponent>(entity);

		light->position[0] = Pos.x;
		light->position[1] = Pos.y;
		light->position[2] = Pos.z;

	}

};