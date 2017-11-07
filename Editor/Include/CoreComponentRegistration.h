#ifndef CORE_COMP_REG_H
#define CORE_COMP_REG_H
#include <ComponentReflection.h>
#include <Renderer.h>
#include <PhysicsSystem.h>

//REGISTERS SOME OF THE CORE COMPONENTS OF THE ENGINE WITH THE EDITOR.


COMPONENT_REGISTER(Transform, "Transform")
COMPONENT_REGISTER(Mesh, "Mesh")
COMPONENT_REGISTER(Material, "Material")
COMPONENT_REGISTER(DebugControl, "DebugControl")
COMPONENT_REGISTER(Camera, "Camera")



#endif