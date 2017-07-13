#pragma once
#include "Mesh.h"
#include "Renderer.h"
#include "BaseSystem.h"

class RenderSystem : System<Mesh, Material>
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

	void PreUpdate() override
	{
		m_Renderer->PreRender();
	}

	void Update(unsigned int entity) override
	{
		m_Renderer->Render(m_CManager->GetComponent<Mesh>(entity), 
							m_CManager->GetComponent<Material>(entity));
	}
	void PostUpdate() override
	{
		m_Renderer->PostRender();
	}

};
