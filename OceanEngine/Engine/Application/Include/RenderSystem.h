#pragma once
#include "Mesh.h"
#include "Renderer.h"
#include "BaseSystem.h"
#include <PxPhysicsAPI.h>
#include <imgui.h>
#include <ComponentReflection.h>
#include <AssetRegistration.h>
#include "Asset.h"
#include <glm\gtc\matrix_transform.hpp>

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







class MaterialFilter
{
public:
	void ExposeToEditor()
	{
		if (ImGui::TreeNodeEx("Textures"))
		{

			ImGui::Text("Albedo: ");
			ImGui::SameLine();
			ImGui::Selectable(m_Albedo.m_AssetName.c_str(), true);
			if (EditorRequestAsset<Texture>(m_Albedo, "Texture", "Albedo: "))
				m_Material.Albedo = m_Albedo.m_Asset;

			ImGui::Text("Normal: ");
			ImGui::SameLine();
			ImGui::Selectable(m_Normal.m_AssetName.c_str(), true);
			if (EditorRequestAsset<Texture>(m_Normal, "Texture", "Normal: "))
				m_Material.Normal = m_Normal.m_Asset;

			ImGui::Text("AO: ");
			ImGui::SameLine();
			ImGui::Selectable(m_AO.m_AssetName.c_str(), true);
			if (EditorRequestAsset<Texture>(m_AO, "Texture", "AO: "))
				m_Material.AO = m_AO.m_Asset;

			ImGui::Text("Roughness: ");
			ImGui::SameLine();
			ImGui::Selectable(m_Roughness.m_AssetName.c_str(), true);
			if (EditorRequestAsset<Texture>(m_Roughness, "Texture", "Roughness: "))
				m_Material.Roughness = m_Roughness.m_Asset;

			ImGui::Text("Metallic: ");
			ImGui::SameLine();
			ImGui::Selectable(m_Metallic.m_AssetName.c_str(), true);
			if (EditorRequestAsset<Texture>(m_Metallic, "Texture", "Metallic: "))
				m_Material.Metallic = m_Metallic.m_Asset;

			ImGui::Text("Emissive: ");
			ImGui::SameLine();
			ImGui::Selectable(m_Emissive.m_AssetName.c_str(), true);
			if (EditorRequestAsset<Texture>(m_Emissive, "Texture", "Metallic: "))
				m_Material.Emissive = m_Emissive.m_Asset;

			ImGui::TreePop();
		}
	}

	COMPONENT_SERIALIZE(m_Albedo, m_Normal, m_AO, m_Roughness, m_Metallic, m_Emissive)
	void serialize_asset(EventManager& manager)
	{
		m_Albedo.RequestAsset(manager);
		m_Normal.RequestAsset(manager);
		m_AO.RequestAsset(manager);
		m_Roughness.RequestAsset(manager);
		m_Metallic.RequestAsset(manager);
		m_Emissive.RequestAsset(manager);

		m_Material.Albedo = m_Albedo.m_Asset;
		m_Material.Normal = m_Normal.m_Asset;
		m_Material.AO = m_AO.m_Asset;
		m_Material.Roughness = m_Roughness.m_Asset;
		m_Material.Metallic = m_Metallic.m_Asset;
		m_Material.Emissive = m_Emissive.m_Asset;
	}


	Material m_Material;
private:
	Asset<Texture> m_Albedo;
	Asset<Texture> m_Normal;
	Asset<Texture> m_AO;
	Asset<Texture> m_Roughness;
	Asset<Texture> m_Metallic;	
	Asset<Texture> m_Emissive;
};
COMPONENT_REGISTER(MaterialFilter, "MaterilFilter");





class DrawGizmoEvent : public IEvent
{
public:
	Mesh * mesh;
	const float* matrix;
};

//-----------------------------------------------------
//	RenderSystem is used in conjunction with ECS to fit old code into
//	the new framework.
//
//					How to Use:
//	Add system to system Manager, and send avalid Renderer pointer.
//-----------------------------------------------------

class RenderSystem : public ECS::System<MeshFilter, MaterialFilter, Transform>
{
private:
	Renderer* m_Renderer;
public:

	RenderSystem(ECS::ComponentManager* a_Cmanager, EventManager& a_eManager)
	:System(a_Cmanager, a_eManager)
	{
		//Add event listner for drawing debug/gizmo meshes
		std:std::function<void(DrawGizmoEvent&)> f2 = [this](DrawGizmoEvent& eve) {DrawDebugMesh(eve); };
		a_eManager.AddListner<DrawGizmoEvent>(f2);
	}

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


	void Update(double deltaTime, ECS::Entity& entity) override
	{
		auto mat =entity.GetComponent<Transform>()->GetGlobalTransformMatrix();
		m_Renderer->Render(&entity.GetComponent<MeshFilter>()->m_Mesh.m_Asset, &entity.GetComponent<MaterialFilter>()->m_Material, mat.front());
	}
	void PreUpdate(double deltaTime) override
	{
		m_Renderer->PreRender();
	}
	

	void DrawDebugMesh(DrawGizmoEvent& a_event)
	{
		m_Renderer->RenderDebug(*a_event.mesh, a_event.matrix);
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

	template<typename Archive>
	void serialize(Archive& arc)
	{
		arc(BaseColour[0], BaseColour[1], BaseColour[2], Intensity);
		Color = BaseColour * Intensity;
	}

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

	void PreUpdate(double deltaTime)
	{
		m_Renderer->PrePointLightPass();
	}

	void Update(double deltaTime, ECS::Entity& entity) override
	{
		PxVec3 Pos =entity.GetComponent<Transform>()->GetTransform()->p;
		PointLightComponent* light = entity.GetComponent<PointLightComponent>();

		m_Renderer->PointLightPass(glm::fvec3(Pos.x, Pos.y, Pos.z), light->Color);
	}

	void PostUpdate(double deltaTime)
	{
		m_Renderer->PostPointLightPass();
	}

	void EntityRegistered(ECS::Entity& entity) override
	{
		PointLightComponent* light = entity.GetComponent<PointLightComponent>();
		m_Renderer->AddPointLight(&light->Color, &light->position, false);
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
		:m_ShadowMap(0) {}

	void ExposeToEditor()
	{
		ImGui::ColorEdit3("Light Colour", &BaseColour.x);
		ImGui::DragFloat("Intensity", &Intensity, 0.1f, 0.0f, 1000.0f);
		ImGui::Checkbox("Casts Shadow", &CastsShadow);

		Colour = BaseColour * Intensity;
	}


	glm::vec3 Colour; //Colour of the light
	glm::mat4 shadowTransform;

	bool CastsShadow = true;
	FrameBuffer m_ShadowMap;
private:
	glm::vec3 BaseColour;
	float Intensity = 0;;
};
COMPONENT_REGISTER(DirectionalLightComponent, "DirectionalLightComp")



class DirectionalLightSystem : ECS::System<DirectionalLightComponent, Transform>
{
public:
	DirectionalLightSystem(ECS::ComponentManager* cmanager, EventManager& emanager)
		:System(cmanager, emanager) {}


	void EntityRegistered(ECS::Entity& entity) override
	{
		auto light = entity.GetComponent<DirectionalLightComponent>();

		//initalize the shadow map
		light->m_ShadowMap.InitDepthTexture(500, 500);

		//set up Transform for shadow map
		
		light->shadowTransform = glm::translate(light->shadowTransform, glm::vec3(0, 0, 5));
		light->shadowTransform = glm::rotate(light->shadowTransform, 45.0f, glm::vec3(1,0,0));

		//Register the framebuffer and transform matrix
		m_Renderer->AddShadowCaster(light->m_ShadowMap, light->shadowTransform);
		
	}

	void Update(double deltaTime, ECS::Entity& entity) override
	{
		//Resolve shadowMap transform
		auto light = entity.GetComponent<DirectionalLightComponent>();

		light->shadowTransform = glm::mat4();
		light->shadowTransform = glm::translate(light->shadowTransform, glm::vec3(0, 0, 5));
		light->shadowTransform = glm::rotate(light->shadowTransform, 45.0f, glm::vec3(1, 0, 0));

		//Directional Light Pass
		m_Renderer->DirectionalLightPass();
	}

private:
	Renderer * m_Renderer;
};

//Used for rendering shadowMaps
class ShadowSystem : public ECS::System<Transform, MeshFilter, MaterialFilter>
{
public:
	ShadowSystem(ECS::ComponentManager* cmanager, EventManager& emanager)
		:System(cmanager, emanager) {}


	void Update(double deltaTime, ECS::Entity& entity)
	{
		auto transform = entity.GetComponent<Transform>();
		auto mesh = entity.GetComponent<MeshFilter>();

		m_Renderer->RenderToShadowMap(&mesh->m_Mesh.m_Asset, 
			transform->GetGlobalTransformMatrix().front());
	}

private:
	std::vector<DirectionalLightComponent*> m_RequiredMaps;
	Renderer* m_Renderer;
};



//---------------------------------------------------------------------------
//POST PROCESS PASS
//------------------------------------------------------------------------

class PostProccessSystem : public ECS::System<NullComponent>
{
public:
	PostProccessSystem(ECS::ComponentManager* a_Cmanager, EventManager& a_eManager)
		:System(a_Cmanager, a_eManager) {}


	void PostUpdate(double deltaTime) override
	{
		m_Renderer->SSAOPass();
		m_Renderer->CombineLighting();

		//Post Processing pass
		m_Renderer->CombineDebug();
		//m_Renderer->CombineUI();
		//m_Renderer->SubmitFrame();
	}

	void SetRenderer(Renderer* renderer)
	{
		if (renderer)
			m_Renderer = renderer;
	}

private:
	Renderer* m_Renderer;
};