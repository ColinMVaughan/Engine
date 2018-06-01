#include "DLLExport.h"


extern "C"
{
	void LoadDLL(ImGuiContext* context)
	{
		ImGui::SetCurrentContext(context);
	}

	void ExportSystemList(ECS::DynamicDetail::DynamicSystemRegistry& reg)
	{
		reg.clear();
		reg = ECS::DynamicDetail::GetDynamicSystemRegistry();
		return;
	}

	void ExportComponentList(ECS::DynamicDetail::DynamicComponentRegistry& reg)
	{
		reg.clear();
		reg = ECS::DynamicDetail::GetDynamicComponentRegistry();
	}

	void UnloadDLL()
	{

	}

}