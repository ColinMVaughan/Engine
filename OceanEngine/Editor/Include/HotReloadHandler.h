#pragma once

#include <Windows.h>

class HotReloadHandler
{
public:

	HotReloadHandler() = default;

	bool TriggerDLLReload();
	bool UnloadDLL();
	bool LoadDLL();


private:

	HINSTANCE m_LibInstance;
};