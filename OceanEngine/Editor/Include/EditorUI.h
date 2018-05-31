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

	void DrawEntityInspector();
	void DrawResourceManager();
	void DrawRenderSettings();

	void DrawEditor();


	bool DrawSplitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f);
	void DrawMenuBar(double deltaTime);
	void DrawEntityListWindow();
	void DrawInspectorWindow();
	void DrawSceneWindow();
	void DrawAssetsWindow();



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

	int WindowSizeX, WindowSizeY;

};