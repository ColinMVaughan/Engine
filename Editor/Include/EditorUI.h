#pragma once
#include <Application.h>
#include <EntityInspector.h>


class Editor : public Application
{
public:
	Editor();
	void DoInitalize() override;
	void DoUpdate(double deltaTime) override;

private:
	void DrawMenuBar(double deltaTime);
	void DrawEntityList();
	void DrawEntityInspector();


	bool IsEntityListActive = false;
	bool IsEntityInspectorActive = false;
	int SelectedEntity = 0;

	EntityInspector EI;
};