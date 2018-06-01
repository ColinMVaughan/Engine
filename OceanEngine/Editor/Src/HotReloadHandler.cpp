#include "HotReloadHandler.h"
#include <DynamicCodeDetail.h>
#include<imgui.h>


typedef void(__cdecl *ExportSysFunc)(ECS::DynamicDetail::DynamicSystemRegistry&);
typedef void(__cdecl *ExportCompFunc)(ECS::DynamicDetail::DynamicComponentRegistry&);
typedef void(__cdecl *LoadDLLFunc)(ImGuiContext*);





bool HotReloadHandler::LoadDLL()
{


	ExportSysFunc exportSystems;
	ExportCompFunc exportComponents;
	LoadDLLFunc loadDll;

	BOOL freeResult, runtimeLinkSuccess = FALSE;

	//Load UserCode library.
	m_LibInstance = LoadLibrary(TEXT("../Debug/UserCoded.dll"));

	//If the handle is valid, the library has been loaded
	if (m_LibInstance != NULL)
	{

		loadDll = (LoadDLLFunc)GetProcAddress(m_LibInstance, "LoadDLL");
		if (loadDll == NULL)
			return false;

		exportSystems = (ExportSysFunc)GetProcAddress(m_LibInstance, "ExportSystemList");
		if (exportSystems == NULL)
			return false;

		exportComponents = (ExportCompFunc)GetProcAddress(m_LibInstance, "ExportComponentList");
		if (exportComponents == NULL)
			return false;

		(loadDll)(ImGui::GetCurrentContext());
		(exportSystems)(ECS::DynamicDetail::GetDynamicSystemRegistry());
		(exportComponents)(ECS::DynamicDetail::GetDynamicComponentRegistry());



		ECS::DynamicDetail::DynamicSystemRegistry* registry = &ECS::DynamicDetail::GetDynamicSystemRegistry();
		//auto it = registry->find("UserComponent");

		//if (it != registry->end())
		//	return true;


		return true;
	}

	return false;
}


bool HotReloadHandler::UnloadDLL()
{
	BOOL freeResult = FALSE;

	if(m_LibInstance != NULL)
		freeResult = FreeLibrary(m_LibInstance);

	return freeResult;
}