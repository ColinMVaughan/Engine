#ifndef CORE_COMP_REG_H
#define CORE_COMP_REG_H
#include <ComponentReflection.h>
#include <Renderer.h>
#include <CameraSystem.h>
#include <PhysicsSystem.h>
#include "AssetRegistration.h"
#include "PrefabEntity.h"
#include "AssetLoader.h"

//REGISTERS SOME OF THE CORE COMPONENTS OF THE ENGINE WITH THE EDITOR.


COMPONENT_REGISTER(Transform, "Transform")
COMPONENT_REGISTER(DebugControl, "DebugControl")
COMPONENT_REGISTER(CameraComponent, "CameraComponent")
COMPONENT_REGISTER(RigidBody, "RigidBody")
COMPONENT_REGISTER(Collider, "Collider")

//Registeres some of the core assets

REGISTER_ASSET(".obj", "Mesh", Mesh)
REGISTER_ASSET(".mat","Material", Material)
REGISTER_ASSET(".png", "Texture", Texture)
REGISTER_ASSET(".prefab","Prefab",PrefabEntity)





//Test function for loading assets
class TestLoad : public BaseAssetLoader
{
public:
	virtual void LoadFile(AssetManager& manager, std::string filepath) override
	{

	}
};
REGISTER_FILETYPE(".fbx", TestLoad)



#endif