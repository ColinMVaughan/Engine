//#pragma once
//#include "Renderer.h"
//#include <BaseSystem.h>


//class RenderSystem : System<Mesh, Material>
//{
//public:
//	void PreUpdate() override
//	{
//		m_renderer.PreRender();
//	}
//
//	void Update(unsigned int Entity) override
//	{
//		Mesh* mesh = m_CManager->GetComponent<Mesh>(Entity);
//		Material* material = m_CManager->GetComponent<Material>(Entity);
//
//		m_renderer.Render(mesh,material);
//	}
//
//	void PostUpdate() override
//	{
//		m_renderer.PostRender();
//	}
//
//private:
//	Renderer m_renderer;
//};