#include "CameraSystem.h"

DebugCameraControlSystem::DebugCameraControlSystem(ECS::ComponentManager* a_cmanager, EventManager& a_eManager)
	:System(a_cmanager, a_eManager)
{
	MoveDirection = PxVec3(0);
	rotations = PxVec2(0);
	m_MotionType = MOTION_NONE;

	//REGISTER_EVENT_LISTNER(KeyPressedEvent, MyKeyDown, a_eManager)
	//REGISTER_EVENT_LISTNER(KeyReleasedEvent, MyKeyUp, a_eManager)
	//REGISTER_EVENT_LISTNER(MouseMovedEvent, MyMouseMoved, a_eManager)

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
	//auto camera = entity.GetComponent<Camera>();

	//Rotate camera by whatever ammout
	//transform->GetTransform()->q = PxQuat(PxIdentity);
	//transform->GetTransform()->q *= PxQuat(rotations.x, PxVec3(0, 1, 0));
	//transform->GetTransform()->q *= PxQuat(rotations.y, PxVec3(1, 0, 0));

	//PxVec3 zoom(0, 0, Zoom);

	//zoom = transform->GetTransform()->q.rotate(zoom);

	//Translate camera in orientation corrected movement direction
	//transform->GetTransform()->p = zoom;//+= transform->GetTransform()->rotate(MoveDirection) * deltaTime;


	//---------------------------------------------------
	PxMat44 Zoom = PxMat44(physx::PxIdentity);
	PxMat44 Rot = PxMat44(physx::PxIdentity);
	PxMat44 Translate = PxMat44(physx::PxIdentity);

	PxQuat quatRot = PxQuat(rotations.x, PxVec3(0, 1, 0)) * PxQuat(rotations.y, PxVec3(1, 0, 0));


	Zoom.setPosition(PxVec3(0.0f, 0.0f, MyZoom));
	Rot = PxMat44(quatRot);
	Translate.setPosition(MoveDirection);

	PxMat44 finalMat = Translate * Rot * Zoom;
	//finalMat = Translate * finalMat;

	//*transform->GetTransform() = PxTransform(Translate);
	transform->GetTransform()->p = finalMat.transform(PxVec3(0));
	transform->GetTransform()->q = quatRot;

	//--------------------------------------------------


	// Convert physx::PxMat44 to glm::mat4 and send updated transform to camera component
	// (I would like to change this in the fututre)
	//camera->m_Transform = glm::make_mat4(transform->GetGlobalTransformMatrix().front());
	return;
}

void DebugCameraControlSystem::PostUpdate(double deltaTime)
{
	//zero out rotation for next update
	//rotation = PxQuat(PxIDENTITY::PxIdentity);

	//zero out move direction for next update
	//MoveDirection *= 0;


}


//Checks that the relivent modifier key has been pressed.
void DebugCameraControlSystem::MyKeyDown(KeyPressedEvent& key)
{
	float Speed = 0.5f;

	switch (key.InputEvent.keysym.sym)
	{
	case 'W':
		MoveDirection.z -= Speed;
		break;
	case 'A':
		MoveDirection.x -= Speed;
		break;
	case 'S':
		MoveDirection.z += Speed;
		break;
	case'D':
		MoveDirection.x += Speed;
		break;
	case 'E':
		MoveDirection.y += Speed;
		break;
	case 'Q':
		MoveDirection.y -= Speed;
		break;

	}
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
	const float dragSpeed = 0.005f;

	switch (m_MotionType)
	{
	case MOTION_TRANSLATE:
		MoveDirection.y += motion.InputEvent.yrel * dragSpeed;
		MoveDirection.x -= motion.InputEvent.xrel * dragSpeed; //set to minus to give a dragging effect
		break;

	case MOTION_ROTATE:
		rotations.x += motion.InputEvent.xrel * rotationSpeed;
		rotations.y += motion.InputEvent.yrel * rotationSpeed;
		break;
	}


}

void DebugCameraControlSystem::MouseScroll(ScrollWheelEvent & eve)
{
	const float speed = 1.0f;
	MyZoom -= eve.InputEvent.y * speed;
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
