#pragma once
#include <Application.h>


class Editor : public Application
{
public:
	Editor();
	void DoInitalize() override;
	void DoUpdate(double deltaTime) override;

private:
	void DrawMenuBar(double deltaTime);
	void DrawEntityInspector();
	void DrawResourceManager();


	bool IsEntityListActive = false;
	bool IsEntityInspectorActive = false;
	bool IsResourceManagerActive = false;
	int SelectedEntity = 0;
};