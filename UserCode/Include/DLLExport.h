#ifndef _DLL_EXPORT_LIB_H_
#define _DLL_EXPORT_LIB_H_

#include <DynamicCodeDetail.h>
#include <imgui.h>

#ifndef EXPORT_DLL
#define EXPORT_DLL __declspec(dllexport)
#endif

extern "C"
{
	EXPORT_DLL void LoadDLL(ImGuiContext* context);

	EXPORT_DLL void ExportSystemList(ECS::DynamicDetail::DynamicSystemRegistry& reg);
	EXPORT_DLL void ExportComponentList(ECS::DynamicDetail::DynamicComponentRegistry& reg);

	EXPORT_DLL void UnloadDLL();
}


#endif