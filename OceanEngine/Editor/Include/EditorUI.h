#pragma once
#include <Application.h>
#include "HotReloadHandler.h"

class Editor : public Application
{
public:
	Editor();
	void DoInitalize() override;

	void PreUpdate(double deltaTime) override;
	void DoUpdate(double deltaTime) override;
	void PostUpdate(double deltaTime) override;

private:
	void DrawMenuBar(double deltaTime);
	void DrawEntityInspector();
	void DrawResourceManager();
	void DrawRenderSettings();

	void TriggerDLLReload();
	void TriggerDLLUnload();

	bool IsEntityListActive = false;
	bool IsEntityInspectorActive = false;
	bool IsResourceManagerActive = false;
	bool IsRenderSettingsActive = false;
	int SelectedEntity = 0;
	bool LookMode = false;

	bool GameRunning = false;
	bool GameCompiling = false;
	HotReloadHandler CodeReload;

};