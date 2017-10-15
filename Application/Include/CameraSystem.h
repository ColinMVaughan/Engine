#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include <ECS.h>
#include <PhysicsSystem.h>
#include <Renderer.h>

#include <glm\mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>



class CameraSystem : public ECS::System<Camera, Transform>
{
	CameraSystem(ECS::ComponentManager* a_CompManager)
		:System(a_CompManager) {}

	// This Updates the Camera Component's transform menmber to the Entities global transform.
	// This Will most likley be refactored later.
	void Update(double deltaTime, unsigned int Entity) override
	{
		//Get Required Components
		auto camera		= m_CManager->GetComponent<Camera>(Entity);
		auto transform  = m_CManager->GetComponent<Transform>(Entity);

		//Set the Camera's Transform Component to the Transform of the Entity
		camera->m_Transform = glm::make_mat4(transform->GetGlobalTransformMatrix().front());
	}
};


#endif