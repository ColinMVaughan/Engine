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
	void ExposeToEditor();


	template <typename Archive>
	void serialize(Archive& arc)
	{
		arc(m_IsKinematic, 
			m_Mass, 
			m_AngularDampening, 
			m_LinearDampening);
	}


	PxRigidDynamic* m_RigidBody = nullptr;
	bool m_IsKinematic = false;
	float m_Mass = 1.0f;
	float m_AngularDampening = 0;
	float m_LinearDampening = 0;

};


class Collider
{
public:

	void ExposeToEditor();
	
	template<typename Archive>
	void save(Archive& arc) const
	{
		arc(m_StaticFriction,
			m_SavedType,
			m_Restitution, 
			m_DynamicFriction,
			m_LocalPosition.x, m_LocalPosition.y, m_LocalPosition.z,
			m_Dimentions.x, m_Dimentions.y, m_Dimentions.z);
	}


	template<typename Archive>
	void load(Archive& arc)
	{
		arc(m_StaticFriction,
			m_SavedType,
			m_Restitution,
			m_DynamicFriction,
			m_LocalPosition.x, m_LocalPosition.y, m_LocalPosition.z,
			m_Dimentions.x, m_Dimentions.y, m_Dimentions.z);

		switch(m_SavedType)
		{
		case PxGeometryType::eBOX:
			m_Geometry = PxBoxGeometry(m_Dimentions);
			m_SelectedShape = 0;
			break;

		case PxGeometryType::eSPHERE:
			m_Geometry = PxSphereGeometry(m_Dimentions.x);
			m_SelectedShape = 1;
			break;

		case PxGeometryType::eCAPSULE:
			m_Geometry = PxCapsuleGeometry(m_Dimentions.y,m_Dimentions.x);
			m_SelectedShape = 2;
			break;
		}
	}

public:
	PxShape * m_CollisionShape = nullptr;
	PxMaterial* m_Material = nullptr;
	PxGeometry m_Geometry = PxBoxGeometry(0.5,0.5,0.5);

	PxVec3 m_Dimentions = PxVec3(1,1,1);
	PxGeometryType::Enum m_SavedType;

	float m_StaticFriction = 1.0;
	float m_Restitution = 1.0;
	float m_DynamicFriction = 1.0;

private:
	int m_SelectedShape = 0;
	PxVec3 m_LocalPosition;
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