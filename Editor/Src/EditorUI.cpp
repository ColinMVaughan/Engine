#include "EditorUI.h"
#include "imgui.h"
#include "imgui_SDL.h"

Editor::Editor()
{
}

void Editor::DoInitalize()
{
	//---------------------------------------------------
	//  Rendering Setup
	//-------------------------------------------------
	m_Renderer->InitalizePBREnvironmentMaps("./Assets/Textures/Footprint_Court_2k.hdr");
}

void Editor::DoUpdate(double deltaTime)
{
	ImGui_ImplSdlGL3_ProcessEvent(&InputEvent);
	ImGui_ImplSdlGL3_NewFrame(m_Window);

	DrawMenuBar();
	DrawEntityList();

	m_Renderer->UIBuffer.Bind();
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui::Render();
	m_Renderer->UIBuffer.UnBind();
}

void Editor::DrawMenuBar()
{

	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New Scene")) {}
		if (ImGui::MenuItem("Open Scene")) {}
		ImGui::EndMenu();
	}


	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Option1")) {}
			if (ImGui::MenuItem("Option2")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("EntityList",NULL,true)) {}
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
	
}

void Editor::DrawEntityList()
{
	ImGui::Begin("Entity List");
	for (int i = 0; i < 10; i++)
	{
		ImGui::Text("Entity %i", i);
	}
	ImGui::End();
}
