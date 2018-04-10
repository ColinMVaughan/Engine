#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "PxPhysicsAPI.h"
#include <imgui.h>
using namespace physx;

//Note: probably should inline these member functions
class Transform
{
public:
	Transform();

	PxMat44 GetGlobalTransformMatrix();
	PxMat44 GetLocalTransformMatrix();

	void SetActor(PxRigidActor* transform);
	void SetParentTransform(Transform* parent);

	PxTransform* GetTransform();
	void ExposeToEditor();

	template<typename Archive>
	void serialize(Archive& arc)
	{
		arc(m_Transform.p.x, m_Transform.p.y, m_Transform.p.z, m_EulerRotations.x, m_EulerRotations.y, m_EulerRotations.z);
		m_Transform.q = PxQuat(PxIdentity);
		m_Transform.q *= PxQuat(m_EulerRotations.x, PxVec3(1, 0, 0));
		m_Transform.q *= PxQuat(m_EulerRotations.y, PxVec3(0, 1, 0));
		m_Transform.q *= PxQuat(m_EulerRotations.z, PxVec3(0, 0, 1));
	}

private:
	PxRigidActor* m_Actor = nullptr;
	Transform* m_Parent = nullptr;
	PxTransform m_Transform;
	PxVec3 m_EulerRotations;
};



class RigidBody
{
public:
	PxRigidBody* m_RigidBody;
};


class Collider
{
	
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
	void* OpenGLContext = nullptr;

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


class PhysXWrapper
{
public:

	bool Initalize(PhysicsSettings a_physicsSettings);
	void Unload();

	PxScene* CreateScene(PxSceneDesc a_SceneDescription);
	PxPhysics* GetPhysics();
	PxScene* GetScene();
	PxControllerManager* GetControllerManager();

	void StepPhysics(double deltaTime);
	void FetchResults();

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
	double m_Accumulator = 0;
	bool m_GPURigidbodiesEnabled = false;
};

#endif