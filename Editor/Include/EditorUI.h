#pragma once
#include <Application.h>

class Editor : public Application
{
public:
	Editor();
	void DoInitalize() override;
	void DoUpdate(double deltaTime) override;

private:
	void DrawMenuBar();
	void DrawEntityList();


	bool IsEntityListActive = false;
};