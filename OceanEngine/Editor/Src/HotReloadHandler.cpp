#include "HotReloadHandler.h"
#include <DynamicCodeDetail.h>
#include<imgui.h>


typedef void(__cdecl *ExportSysFunc)(ECS::DynamicDetail::DynamicSystemRegistry&);
typedef void(__cdecl *ExportCompFunc)(ECS::DynamicDetail::DynamicComponentRegistry&);
typedef void(__cdecl *LoadDLLFunc)(ImGuiContext*);


bool HotReloadHandler::LoadDLL()
{

	ExportSysFunc exportSystems; //function for retrieving user created systems
	ExportCompFunc exportComponents; //function for retrieving user created components
	LoadDLLFunc loadDll; //function for loading the dll and assigning the IMGUI context for the editor

	BOOL freeResult, runtimeLinkSuccess = FALSE;

#ifdef _DEBUG
	//Load UserCode library.
	m_LibInstance = LoadLibrary(TEXT("../Debug/UserCoded.dll"));
#else
	m_LibInstance = LoadLibrary(TEXT("../Release/UserCode.dll"));
#endif
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

		//Call the three loading functions
		(loadDll)(ImGui::GetCurrentContext());
		(exportSystems)(ECS::DynamicDetail::GetDynamicSystemRegistry());
		(exportComponents)(ECS::DynamicDetail::GetDynamicComponentRegistry());

		ECS::DynamicDetail::DynamicSystemRegistry* registry = &ECS::DynamicDetail::GetDynamicSystemRegistry();


		return true;
	}

	return false;
}

//----------------------------------
// Frees the dll usercode library.
// Returns true on success.
//---------------------------------
bool HotReloadHandler::UnloadDLL()
{
	BOOL freeResult = FALSE;

	if(m_LibInstance != NULL)
		freeResult = FreeLibrary(m_LibInstance);

	return freeResult;
}