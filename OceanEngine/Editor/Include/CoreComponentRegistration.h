#ifndef CORE_COMP_REG_H
#define CORE_COMP_REG_H
#include <ComponentReflection.h>
#include <Renderer.h>
#include <PhysicsSystem.h>
#include "AssetRegistration.h"
#include "PrefabEntity.h"

//REGISTERS SOME OF THE CORE COMPONENTS OF THE ENGINE WITH THE EDITOR.


COMPONENT_REGISTER(Transform, "Transform")
COMPONENT_REGISTER(DebugControl, "DebugControl")
COMPONENT_REGISTER(Camera, "Camera")
COMPONENT_REGISTER(RigidBody, "RigidBody")
COMPONENT_REGISTER(Collider, "Collider")

//Registeres some of the core assets

REGISTER_ASSET(".obj", "Mesh", Mesh)
REGISTER_ASSET(".mat","Material", Material)
REGISTER_ASSET(".png", "Texture", Texture)
REGISTER_ASSET(".prefab","Prefab",PrefabEntity)

#endif