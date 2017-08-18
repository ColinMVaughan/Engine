#include "PhysicsSystem.h"

bool PhysicsSystem::Initalize()
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

	m_Dispatcher = PxDefaultCpuDispatcherCreate(2);

	PxSceneDesc defaultSceneDescription(m_Physics->getTolerancesScale());
	defaultSceneDescription.gravity = PxVec3(0.0f, -9.81f, 0.0f);

	m_Scene = CreateScene(defaultSceneDescription);
}

void PhysicsSystem::Unload()
{
	PX_UNUSED(false);
	m_Scene->release();
	m_Pvd->release();
	m_Physics->release();
	m_Foundation->release();
}


PxScene* PhysicsSystem::CreateScene(PxSceneDesc a_SceneDescription)
{

	a_SceneDescription.cpuDispatcher = m_Dispatcher;
	a_SceneDescription.filterShader = PxDefaultSimulationFilterShader;

	PxScene* tempScene = m_Physics->createScene(a_SceneDescription);

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

void PhysicsSystem::StepPhysics()
{
	PX_UNUSED(false); //I don't know what this does
	m_Scene->simulate(1.0f / 20.0f);
	m_Scene->fetchResults(true);

}