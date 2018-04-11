#include <imgui.h>
#include "PhysXWrapper.h"

bool PhysXWrapper::Initalize(PhysicsSettings a_physicsSettings)
{
	//Create Foundation for Physics
	m_Foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, m_defaultAllocator, m_defaultErrorCallback);
	if (!m_Foundation)
		return false;

	//Set Up PVD for connectin to external physics debugger
	m_Pvd = PxCreatePvd(*m_Foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	//Create physics class
	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true, m_Pvd);
	if (!m_Physics)
		return false;

	//if GPU physics are enabled and the user ha provided a valid opengl context
	//initalize gpu rigidbodies
	if (a_physicsSettings.OpenGLContext != nullptr && a_physicsSettings.GpuRigidbodies)
	{
		PxCudaContextManagerDesc cudaDesc;
		//cudaDesc.appGUID = 0;
		//cudaDesc.graphicsDevice = a_physicsSettings.OpenGLContext;

		m_CudaContextManager = PxCreateCudaContextManager(*m_Foundation, cudaDesc);
		m_GPURigidbodiesEnabled = true;
	}

	//Create Dispatcher with 2 threads
	m_Dispatcher = PxDefaultCpuDispatcherCreate(2);

	//Set The unit scale and gravity for the default scene
	PxSceneDesc defaultSceneDescription(m_Physics->getTolerancesScale());
	defaultSceneDescription.gravity = PxVec3(0.0f, -9.81f, 0.0f);

	//Create the scene
	m_Scene = CreateScene(defaultSceneDescription);

	//Create the controller manager
	m_ControllerManager = PxCreateControllerManager(*m_Scene);
}

void PhysXWrapper::Unload()
{
	PX_UNUSED(false);
	m_Scene->release();
	//m_Pvd->release(); //will remove pvd stuff
	m_Physics->release();
	m_Foundation->release();
}


PxScene* PhysXWrapper::CreateScene(PxSceneDesc a_SceneDescription)
{
	//Set scene settings that will be constant across the apllication
	a_SceneDescription.cpuDispatcher = m_Dispatcher;
	a_SceneDescription.filterShader = PxDefaultSimulationFilterShader;

	if (m_GPURigidbodiesEnabled)
	{
		a_SceneDescription.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
		a_SceneDescription.broadPhaseType = PxBroadPhaseType::eGPU;
		a_SceneDescription.gpuDispatcher = m_CudaContextManager->getGpuDispatcher();
	}

	PxScene* tempScene = m_Physics->createScene(a_SceneDescription);

	//Sets up scene to transmit to PVD
	PxPvdSceneClient* pvdClient = tempScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	return tempScene;
}

PxPhysics* PhysXWrapper::GetPhysics()
{
	return m_Physics;
}

PxScene* PhysXWrapper::GetScene()
{
	return m_Scene;
}

PxControllerManager* PhysXWrapper::GetControllerManager()
{
	return m_ControllerManager;
}

void PhysXWrapper::StepPhysics(double deltaTime)
{
	PX_UNUSED(false); 
	m_Accumulator += deltaTime;

	while (true)
	{
		//if less time has passed than needed to simulate a physics step
		//then return immediatly
		if (m_Accumulator < (1.0 / 30.0))
			break;

		//Otherwise subtract the step size from the accumulator and simulate
		m_Accumulator -= (1.0 / 30.0);
		m_Scene->simulate(1.0f / 30.0f);
		m_Scene->fetchResults(true);
	}

	return;
}

void  PhysXWrapper::FetchResults()
{
	m_Scene->fetchResults(true);
}


//-------------------------------------------------------------------------------
//							TRANSFORM
//-------------------------------------------------------------------------------

Transform::Transform()
{
	m_Transform = PxTransform(0, 0, 0);
	m_EulerRotations = PxVec3(0, 0, 0);
}

PxMat44 Transform::GetGlobalTransformMatrix()
{
	if (m_Actor)
		m_Transform = m_Actor->getGlobalPose();

	if (m_Parent)
		return PxMat44(m_Transform.transform(*m_Parent->GetTransform()));

	return PxMat44(m_Transform);
}

PxMat44 Transform::GetLocalTransformMatrix()
{
	if (m_Actor)
		m_Transform = m_Actor->getGlobalPose();

	return PxMat44(m_Transform);
}


void Transform::SetActor(PxRigidActor* actor)
{
	m_Actor = actor;
}

void Transform::SetParentTransform(Transform* parent)
{
	m_Parent = parent;
}

PxTransform* Transform::GetTransform()
{
	if (m_Actor)
		m_Transform = m_Actor->getGlobalPose();

	return &m_Transform;
}

void Transform::ExposeToEditor()
{
	ImGui::DragFloat3("Position", &m_Transform.p.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("Rotation", &m_EulerRotations.x, 0.01f, -6.28319f, 6.28319f);

	m_Transform.q = PxQuat(PxIdentity);
	m_Transform.q *= PxQuat(m_EulerRotations.x, PxVec3(1, 0, 0));
	m_Transform.q *= PxQuat(m_EulerRotations.y, PxVec3(0, 1, 0));
	m_Transform.q *= PxQuat(m_EulerRotations.z, PxVec3(0, 0, 1));
}


//----------------------------------------------------------------
//					RIGIDBODY
//----------------------------------------------------------------
void RigidBody::ExposeToEditor()
{
	ImGui::SliderFloat("Mass", nullptr, 0.0f, 1000.0f);
	ImGui::DragFloat("Drag", nullptr);
	ImGui::DragFloat("Friction", nullptr);
}




//----------------------------------------------------------------
//					COLLIDER
//----------------------------------------------------------------
void Collider::ExposeToEditor()
{
	//If the user selects a different kind of geometry, change to it.
	if(ImGui::Combo("CollisionShape", &m_SelectedShape, "Box\0Sphere\0Capsule\0Plane\0\0"))
	{
		switch (m_SelectedShape)
		{
			//Box
		case 0:
		{PxBoxGeometry box; }
			break;

			//Sphere
		case 1:
		{PxSphereGeometry sphere; }
			break;

		case 2:
		{PxCapsuleGeometry cap; }
			break;

		case 3:
		{PxPlaneGeometry plane; }
			break;

		}
	}
		
	ImGui::Separator();

	//Different settings based on kind of geometry
	switch(m_CollisionShape->getGeometryType())
	{
	case PxGeometryType::eBOX:
	{
		PxBoxGeometry boxInfo;
		m_CollisionShape->getBoxGeometry(boxInfo);
		ImGui::SliderFloat3("Box Size:", &boxInfo.halfExtents.x, 0.0001f, 10000.0f);
	}
		break;

	case PxGeometryType::eSPHERE:
	{
		PxSphereGeometry sphereInfo;
		m_CollisionShape->getSphereGeometry(sphereInfo);
		ImGui::SliderFloat("Radius:", &sphereInfo.radius, 0.0000f, 10000.0f);
	}
		break;

	case PxGeometryType::eCAPSULE:
		break;

	case PxGeometryType::ePLANE:
		break;
	}

}
