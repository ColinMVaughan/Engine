#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "PxPhysicsAPI.h"
using namespace physx;

//Note: probably should inline these member functions
class Transform
{
public:

	PxMat44 GetGlobalTransformMatrix();
	PxMat44 GetLocalTransformMatrix();

	void SetActor(PxRigidActor* transform);
	void SetParentActor(PxRigidActor* parent);

	PxTransform GetTransform();

private:
	PxRigidActor* m_Actor = nullptr;
	PxRigidActor* m_Parent = nullptr;
};



class RigidBody
{
public:
	PxRigidBody* m_RigidBody;
};



//------------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------------

struct PhysicsSettings
{
	// DEFAULT: 0
	// Must Pass the OpenGL context if using GPU rigidbodies. 
	// If not, this may remain 0.
	int OpenGLContext = 0;

	// DEFAULT: 2
	// The number of threads Physx will use
	unsigned int NumberOfThreads = 2;

	// DEFAULT: true
	// Allows Physx to use CUDA to simulate rigidbodies.
	// Must pass GLContext if enabled
	bool GpuRigidbodies = false;

	// DEFAULT: false
	// Use the Physx Visual Debugger
	bool UsePVD = false;

};


class PhysicsSystem
{
public:

	bool Initalize();
	void Unload();

	PxScene* CreateScene(PxSceneDesc a_SceneDescription);
	PxPhysics* GetPhysics();
	PxScene* GetScene();
	PxControllerManager* GetControllerManager();

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

	PxControllerManager* m_ControllerManager;

	PxCudaContextManager* m_CudaContextManager;

};

#endif