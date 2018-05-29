#include "HotReloadHandler.h"
#include <DynamicCodeDetail.h>



typedef void(__cdecl *ExportSysFunc)(ECS::DynamicDetail::DynamicSystemRegistry&);
typedef void(__cdecl *ExportCompFunc)(ECS::DynamicDetail::DynamicComponentRegistry&);





bool HotReloadHandler::LoadDLL()
{


	ExportSysFunc exportSystems;
	ExportCompFunc exportComponents;

	BOOL freeResult, runtimeLinkSuccess = FALSE;

	//Load UserCode library.
	m_LibInstance = LoadLibrary(TEXT("UserCoded.dll"));

	//If the handle is valid, the library has been loaded
	if (m_LibInstance != NULL)
	{

		exportSystems = (ExportSysFunc)GetProcAddress(m_LibInstance, "ExportSystemList");
		if (exportSystems == NULL)
			return false;

		exportComponents = (ExportCompFunc)GetProcAddress(m_LibInstance, "ExportComponentList");
		if (exportComponents == NULL)
			return false;


		(exportSystems)(ECS::DynamicDetail::GetDynamicSystemRegistry());
		(exportComponents)(ECS::DynamicDetail::GetDynamicComponentRegistry());



		ECS::DynamicDetail::DynamicComponentRegistry* registry = &ECS::DynamicDetail::GetDynamicComponentRegistry();
		auto it = registry->find("UserComponent");

		if (it != registry->end())
			return true;


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