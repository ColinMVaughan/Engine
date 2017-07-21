#pragma once
#include "Mesh.h"
#include "Renderer.h"
#include "BaseSystem.h"

class RenderSystem : public System<Mesh, Material>
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
		m_Renderer = a_renderer;
	}

	void PreUpdate() override
	{
		m_Renderer->PreRender();
	}

	void Update(unsigned int entity) override
	{
		m_Renderer->Render(m_CManager->GetComponent<Mesh>(entity), m_CManager->GetComponent<Material>(entity));
	}
	void PostUpdate() override
	{
		m_Renderer->PostRender();
	}

};
