//=========================================================================
//	Editor is used to set up and test your scenes. It also allows for 
//	compiled c++ code to be hotloaded into the engine at runtime
//
//	
//
//=========================================================================

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


	//Methods for drawing the various editor windows.//
	void DrawEditor();
	bool DrawSplitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f);
	void DrawMenuBar(double deltaTime);
	void DrawEntityListWindow();
	void DrawInspectorWindow();
	void DrawSceneWindow();
	void DrawAssetsWindow();
	void DrawMaterialWindow();

	//Opens serialized scene file.//
	void OpenScene(std::string path);

	//Hot-Reloads User code//
	void TriggerDLLReload();
	void TriggerDLLUnload();


	
	bool IsMaterialEditorActive = false;
	bool IsEntityInspectorActive = false;
	bool IsResourceManagerActive = false;
	bool IsRenderSettingsActive = false;
	int SelectedEntity = 0;
	bool LookMode = false;

	bool GameRunning = false;
	bool GameCompiling = false;
	HotReloadHandler CodeReload;

	int WindowSizeX, WindowSizeY;
	double m_DeltaTime;

};