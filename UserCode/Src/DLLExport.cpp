#include "DLLExport.h"


extern "C"
{
	void LoadDLL()
	{

	}

	void ExportSystemList(ECS::DynamicDetail::DynamicSystemRegistry& reg)
	{
		reg.clear();
		reg = ECS::detail::GetSystemRegistry();
	}

	void ExportComponentList(ECS::DynamicDetail::DynamicComponentRegistry& reg)
	{
		reg.clear();
		reg = ECS::detail::GetComponentRegistry();
	}

	void UnloadDLL()
	{

	}

}