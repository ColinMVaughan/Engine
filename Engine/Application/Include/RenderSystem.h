#pragma once
#include "Mesh.h"
#include "Renderer.h"
#include "BaseSystem.h"
#include <PxPhysicsAPI.h>
#include <imgui.h>
#include <ComponentReflection.h>

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

	RenderSystem(ECS::ComponentManager* a_Cmanager, EventManager& a_eManager)
	:System(a_Cmanager, a_eManager){}

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
		m_Renderer->CombineUI();
		m_Renderer->SubmitFrame();
	}
	
};

//-----------------------------------------------------------------------------
//						Point Light System
//-----------------------------------------------------------------------------
class PointLightComponent
{
public:

	void ExposeToEditor()
	{
		
		ImGui::ColorEdit3("Light Colour", &BaseColour.x);
		ImGui::DragFloat("Intensity", &Intensity, 0.1f, 0.0f, 1000.0f);

		Color = BaseColour * Intensity;
	}

	glm::fvec3 position;
	glm::fvec3 Color;
	bool Registered = false;

	COMPONENT_SERIALIZE(BaseColour[0], BaseColour[1], BaseColour[2], Intensity)

private:
	glm::vec3 BaseColour;
	float Intensity = 0;
};
COMPONENT_REGISTER(PointLightComponent, "PointLightComponent")


class PointLightSystem : public ECS::System<PointLightComponent, Transform>
{
public:

	PointLightSystem(ECS::ComponentManager* a_Cmanager, EventManager& a_eManager)
		:System(a_Cmanager, a_eManager) {}

	void SetRenderer(Renderer* a_Renderer)
	{
		m_Renderer = a_Renderer;
	}

	void Update(double deltaTime, ECS::Entity& entity) override
	{

		PxVec3 Pos =entity.GetComponent<Transform>()->GetTransform()->p;
		PointLightComponent* light = entity.GetComponent<PointLightComponent>();

		if (!light->Registered)
		{
			m_Renderer->AddPointLight(&light->Color, &light->position, false);
			light->Registered = true;
		}

		light->position[0] = Pos.x;
		light->position[1] = Pos.y;
		light->position[2] = Pos.z;

	}

private:
	Renderer* m_Renderer;
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