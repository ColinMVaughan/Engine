#pragma once
#include "Mesh.h"
#include "Renderer.h"
#include "BaseSystem.h"
#include <PxPhysicsAPI.h>
#include <imgui.h>
#include <ComponentReflection.h>
#include <AssetRegistration.h>
#include "Asset.h"


//---------------------------------------------------------------------------------
//							Mesh and Material Components
//----------------------------------------------------------------------------------

class MeshFilter
{
public:

	void ExposeToEditor()
	{
		ImGui::Text("Mesh: ");
		ImGui::SameLine();
		ImGui::Selectable(m_Mesh.m_AssetName.c_str(), true);
		//If something is being dragged/dropped into our window
		if (ImGui::BeginDragDropTarget())
		{
			//If this is something we can accept
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Mesh"))
			{
				//Assert that this data is the correct size
				IM_ASSERT(payload->DataSize == sizeof(BaseAssetRequestEvent*));

				BaseAssetRequestEvent* base = nullptr;
				memcpy(&base, payload->Data, sizeof(BaseAssetRequestEvent*));

				auto meshRequest = static_cast<AssetRequestEvent<Mesh>*>(base);
				m_Mesh.m_AssetName = meshRequest->m_AssetName;
				m_Mesh.m_AssetType = "Mesh";
				m_Mesh.m_Asset = *meshRequest->Asset;

			}
			ImGui::EndDragDropTarget();
		}

	}

	COMPONENT_SERIALIZE(m_Mesh)
	void serialize_asset(EventManager& manager)		
	{												
		m_Mesh.RequestAsset(manager);			
	}
	Asset<Mesh> m_Mesh;
};
COMPONENT_REGISTER(MeshFilter, "MeshFilter")




//-----------------------------------------------------
//	RenderSystem is used in conjunction with ECS to fit old code into
//	the new framework.
//
//					How to Use:
//	Add system to system Manager, and send avalid Renderer pointer.
//-----------------------------------------------------

class RenderSystem : public ECS::System<MeshFilter, Material, Transform>
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
		m_Renderer->Render(&entity.GetComponent<MeshFilter>()->m_Mesh.m_Asset, entity.GetComponent<Material>(), mat.front());
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

class DirectionalLightComponent
{
public:

	DirectionalLightComponent()
	{
		m_ShadowMap = new FrameBuffer(0);
	}

	void ExposeToEditor()
	{
		ImGui::ColorEdit3("Light Colour", &BaseColour.x);
		ImGui::DragFloat("Intensity", &Intensity, 0.1f, 0.0f, 1000.0f);
		ImGui::Checkbox("Casts Shadow", &CastsShadow);

		Colour = BaseColour * Intensity;
	}


	glm::vec3 Colour;
	glm::vec3 Direction;
	bool CastsShadow = true;

private:
	FrameBuffer* m_ShadowMap;
	glm::vec3 BaseColour;
	float Intensity = 0;;
};
COMPONENT_REGISTER(DirectionalLightComponent, "DirectionalLightComp")
