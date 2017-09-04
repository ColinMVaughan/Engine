#include "PhysicsSystem.h"

bool PhysicsSystem::Initalize(PhysicsSettings a_physicsSettings)
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
		cudaDesc.graphicsDevice = a_physicsSettings.OpenGLContext;
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

void PhysicsSystem::Unload()
{
	PX_UNUSED(false);
	m_Scene->release();
	//m_Pvd->release(); //will remove pvd stuff
	m_Physics->release();
	m_Foundation->release();
}


PxScene* PhysicsSystem::CreateScene(PxSceneDesc a_SceneDescription)
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

PxPhysics* PhysicsSystem::GetPhysics()
{
	return m_Physics;
}

PxScene* PhysicsSystem::GetScene()
{
	return m_Scene;
}

PxControllerManager* PhysicsSystem::GetControllerManager()
{
	return m_ControllerManager;
}

void PhysicsSystem::StepPhysics(double deltaTime)
{
	PX_UNUSED(false); 

	//if less time has passed than needed to simulate a physics step
	//then return immediatly
	m_Accumulator += deltaTime;
	if (m_Accumulator < (1.0 / 30.0))
		return;

	//Otherwise subtract the step size from the accumulator and simulate
	m_Accumulator -= (1.0/30.0);
	m_Scene->simulate(1.0f / 30.0f);
	m_Scene->fetchResults(true);
	return;
}

void  PhysicsSystem::FetchResults()
{
	m_Scene->fetchResults(true);
}
//-------------------------------------------------------------------------------

PxMat44 Transform::GetGlobalTransformMatrix()
{
	if(m_Parent != nullptr)
		return PxMat44(m_Actor->getGlobalPose().transform(m_Parent->getGlobalPose()));
	else
		return PxMat44(m_Actor->getGlobalPose());
}

PxMat44 Transform::GetLocalTransformMatrix()
{
	return PxMat44(m_Actor->getGlobalPose());
}


void Transform::SetActor(PxRigidActor* actor)
{
	m_Actor = actor;
}

void Transform::SetParentActor(PxRigidActor* parent)
{
	m_Parent = parent;
}

PxTransform Transform::GetTransform()
{
	return m_Actor->getGlobalPose();
}