#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "PxPhysicsAPI.h"
using namespace physx;


class RigidActor
{
public:
	PxRigidActor* m_RigidActor;
};

class RigidBody
{
public:
	PxRigidBody* m_RigidBody;
};



class PhysicsSystem
{
public:


	bool Initalize();
	void Unload();

	PxScene* CreateScene(PxSceneDesc a_SceneDescription);
	PxPhysics* GetPhysics();
	PxScene* GetScene();

	void StepPhysics();

private:


	PxDefaultErrorCallback m_defaultErrorCallback;
	PxDefaultAllocator m_defaultAllocator;

	PxFoundation* m_Foundation;
	PxPhysics* m_Physics;

	PxDefaultCpuDispatcher* m_Dispatcher;
	PxScene* m_Scene;

	PxMaterial* m_DefaultMaterial;
	PxPvd* m_Pvd;

};

#endif