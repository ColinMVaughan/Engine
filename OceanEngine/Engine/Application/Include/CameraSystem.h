#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include <ECS.h>
#include <PhysicsSystem.h>
#include <Renderer.h>

#include <glm\mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ControlEvents.h>


//----------------------------------------------------------------------------
//						Camera Component
//
// Camera component allows the user to switch between different cameras.
// Cameras will be set to either 
//
// -Editor (the debug camera), 
// -Default (a camera that can be switched to during gameplay),
// -Main (the current active camera during gameplay)
//
//
// 
//
//----------------------------------------------------------------------------
class CameraComponent
{
public:
	enum CameraTag{Editor = 0, Default, Main};
	CameraTag m_Tag;

public:

	void ExposeToEditor()
	{
		static int selected = 0;
		if(ImGui::BeginCombo("Camera Type",nullptr))
		{
			if (ImGui::Selectable("Default")) { m_Tag = CameraTag::Default; }
			if (ImGui::Selectable("Main Camera")) { m_Tag = CameraTag::Main; }
			
			ImGui::EndCombo();
		}

		ImGui::SliderFloat("Exposure", &m_Camera.Exposure, 0.1f, 3.0f);
	}

	COMPONENT_SERIALIZE(m_CameraID)

	Camera m_Camera;
	int m_CameraID;
};

class SetMainCameraEvent : public IEvent
{
public: 
	SetMainCameraEvent() = default;
	SetMainCameraEvent(CameraComponent* cam) :m_Camera(cam) {}
	CameraComponent * m_Camera;
};

class SetEditorCameraEvent : public IEvent
{
public:
	SetEditorCameraEvent() = default;
	SetEditorCameraEvent(CameraComponent* cam):camera(cam) {}
	CameraComponent* camera;
};

//-----------------------------------------------------------------------------
//					CameraSystem
//
//	CameraSystem sends the entitie's transform matrix to the Camera component
//	once per frame.
//	
//	Required Components: Camera, Transform
//-----------------------------------------------------------------------------

class CameraSystem : public ECS::System<CameraComponent, Transform>
{
public:
	CameraSystem(ECS::ComponentManager* a_CompManager, EventManager& a_EveManager)
		:System(a_CompManager, a_EveManager) 
	{
		//std:std::function<void(SetEditorCameraEvent&)> f2 = [this](SetEditorCameraEvent& eve) {SetEditorCamera(eve); };
		//a_EveManager.AddListner<SetEditorCameraEvent>(f2);
		REGISTER_EVENT_LISTNER(SetEditorCameraEvent,SetEditorCamera,a_EveManager)
	}

	// This Updates the Camera Component's transform menmber to the Entities global transform.
	// This Will most likley be refactored later.
	void Update(double deltaTime, ECS::Entity& entity) override
	{
		//Get Required Components
		auto camera		= entity.GetComponent<CameraComponent>();
		auto transform  = entity.GetComponent<Transform>();

		//Set the Camera's Transform Component to the Transform of the Entity
		camera->m_Camera.m_Transform = glm::make_mat4(transform->GetGlobalTransformMatrix().front());
	}


	void Start(ECS::Entity& entity) override
	{
		auto camera = entity.GetComponent<CameraComponent>();

		if (camera->m_Tag == CameraComponent::Main)
			m_MainCamera = camera;
	}

	void PostStart() override
	{
		if (m_MainCamera)
			m_Renderer->SetCamera(&m_MainCamera->m_Camera);
	}

	void Stop(ECS::Entity& entity) override
	{

	}

	void PostStop() override
	{
		m_Renderer->SetCamera(&m_EditorCamera->m_Camera);
	}

	void EntityRegistered(ECS::Entity& entity) override
	{
		auto cam = entity.GetComponent<CameraComponent>();
		
		//set default projection
		cam->m_Camera.m_Projection = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 300.0f);

		
		//if there is no editor camera set, use this camera
		if (m_EditorCamera == nullptr)
		{
			m_EditorCamera = cam;
			m_Renderer->SetCamera(&cam->m_Camera);
		}

		//if there is no main camera set, use this one
		if (m_MainCamera == nullptr)
			m_MainCamera = cam;
	}

	void SetEditorCamera(SetEditorCameraEvent& a_event)
	{
		m_EditorCamera = a_event.camera;
		m_MainCamera = m_EditorCamera;
		m_Renderer->SetCamera(&m_EditorCamera->m_Camera);
	}

	void SetMainCamera(SetMainCameraEvent& a_event)
	{
		m_MainCamera = a_event.m_Camera;
	}

	void SetRendrer(Renderer* rendPtr)
	{
		m_Renderer = rendPtr;
	}
private:
	Renderer * m_Renderer;
	CameraComponent* m_EditorCamera = nullptr;
	CameraComponent* m_MainCamera = nullptr;

};


//---------------------------------------------------------------------------------------
//						Debug Camera Control
//
//	Debug Camera Control is used for controlling the camera without being connected to the player
//	or gravity & collisions.
//
//	Required Components: DebugControl, Transform, Camera
//---------------------------------------------------------------------------------------

struct DebugControl
{
public:
	COMPONENT_SERIALIZE(bla)
private:
	short int bla;
};

class DebugCameraControlSystem : public ECS::System<DebugControl, Transform, CameraComponent>
{
public:

	DebugCameraControlSystem(ECS::ComponentManager* a_cmanager, EventManager& a_eManager);

	void PreUpdate(double deltaTime) override;
	void Update(double deltaTime, ECS::Entity& entity) override;
	void PostUpdate(double deltaTime) override;

	void MyKeyDown(KeyPressedEvent& eve);
	void MyKeyUp(KeyReleasedEvent& eve);
	void MyMouseMoved(MouseMovedEvent& eve);
	void MouseScroll(ScrollWheelEvent& eve);
	void ButtonDown(MouseClickedEvent& eve);
	void ButtonUp(MouseReleasedEvent& eve);

private:







	enum MotionType{MOTION_NONE=0, MOTION_TRANSLATE = 2, MOTION_ROTATE = 4} m_MotionType;


	float m_Zoom = 1.0f; // Construct zoom
	PxVec3 m_Position;
	PxVec3 m_Translation;
	PxVec2 m_Rotation;
	float speed = 0.3f;
};

#endif