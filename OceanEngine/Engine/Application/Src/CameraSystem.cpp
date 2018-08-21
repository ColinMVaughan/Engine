#include "CameraSystem.h"

DebugCameraControlSystem::DebugCameraControlSystem(ECS::ComponentManager* a_cmanager, EventManager& a_eManager)
	:System(a_cmanager, a_eManager)
{
	m_Translation = PxVec3(0);
	m_Rotation = PxVec2(0);
	m_MotionType = MOTION_NONE;

	m_Position = PxVec3(0);
	

	RegisterEventListner<KeyPressedEvent>(a_eManager, [this](KeyPressedEvent& eve) {MyKeyDown(eve); });
	//RegisterEventListner<KeyReleasedEvent>(a_eManager, [this](KeyReleasedEvent& eve) {MyKeyUp(eve); });
	RegisterEventListner<MouseMovedEvent>(a_eManager, [this](MouseMovedEvent& eve) {MyMouseMoved(eve); });
	RegisterEventListner<ScrollWheelEvent>(a_eManager, [this](ScrollWheelEvent& eve) {MouseScroll(eve); });
	RegisterEventListner<MouseClickedEvent>(a_eManager, [this](MouseClickedEvent& eve) {ButtonDown(eve); });
	RegisterEventListner<MouseReleasedEvent>(a_eManager, [this](MouseReleasedEvent& eve) {ButtonUp(eve); });
}

void DebugCameraControlSystem::PreUpdate(double deltaTime)
{

}


void DebugCameraControlSystem::Update(double deltaTime, ECS::Entity& entity)
{
	//Get Transform and camera components of entity
	auto transform = entity.GetComponent<Transform>();

	//initalize matricies as identity
	PxMat44 Zoom = PxMat44(physx::PxIdentity);
	PxMat44 Rot = PxMat44(physx::PxIdentity);
	PxMat44 Translate = PxMat44(physx::PxIdentity);

	PxQuat quatRot = PxQuat(m_Rotation.x, PxVec3(0, 1, 0)) * PxQuat(m_Rotation.y, PxVec3(1, 0, 0));
	m_Position += quatRot.rotate(m_Translation);
	m_Translation *= 0;

	//Set up position, rotation, and zoom matricies
	Zoom.setPosition(PxVec3(0.0f, 0.0f, m_Zoom));
	Rot = PxMat44(quatRot);
	Translate.setPosition(m_Position);

	PxMat44 finalMat = Translate * Rot * Zoom;

	//give the transform component the final position and rotation
	transform->GetTransform()->p = finalMat.transform(PxVec3(0));
	transform->GetTransform()->q = quatRot;

	return;
}

void DebugCameraControlSystem::PostUpdate(double deltaTime)
{
}


//Checks that the relivent modifier key has been pressed.
void DebugCameraControlSystem::MyKeyDown(KeyPressedEvent& key)
{
}

void DebugCameraControlSystem::MyKeyUp(KeyReleasedEvent& key)
{
}

// If the Mouse is moved, Rotate the Camera's View Matrix 
// and change the foreward direction used for movement
// 
void DebugCameraControlSystem::MyMouseMoved(MouseMovedEvent& motion)
{
	const float rotationSpeed = 0.005f;
	const float dragSpeed = 0.009f;

	switch (m_MotionType)
	{
	case MOTION_TRANSLATE:
		m_Translation.y += motion.InputEvent.yrel * dragSpeed;
		m_Translation.x -= motion.InputEvent.xrel * dragSpeed; //set to minus to give a dragging effect
		break;

	case MOTION_ROTATE:
		m_Rotation.x += motion.InputEvent.xrel * rotationSpeed;
		m_Rotation.y += motion.InputEvent.yrel * rotationSpeed;
		break;
	}


}

void DebugCameraControlSystem::MouseScroll(ScrollWheelEvent & eve)
{
	const float speed = 1.0f;
	m_Zoom -= eve.InputEvent.y * speed;

	//clamp MyZoom at 0.5 so that we dont run into strange camera behaviour at negitive (or near 0) zoom
	m_Zoom = m_Zoom < 0.5 ? 0.5 : m_Zoom; 
}

void DebugCameraControlSystem::ButtonDown(MouseClickedEvent & eve)
{
	if (eve.InputEvent.button == SDL_BUTTON_RIGHT)
		m_MotionType = MOTION_TRANSLATE;

	if (eve.InputEvent.button == SDL_BUTTON_LEFT)
		m_MotionType = MOTION_ROTATE;

}

void DebugCameraControlSystem::ButtonUp(MouseReleasedEvent & eve)
{
	if (eve.InputEvent.button == SDL_BUTTON_LEFT || eve.InputEvent.button == SDL_BUTTON_RIGHT)
		m_MotionType = MOTION_NONE;

}
