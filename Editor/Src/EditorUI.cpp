#include "EditorUI.h"
#include "imgui.h"
#include "imgui_SDL.h"
#include <ECS.h>

Editor::Editor():EI(m_Scene)
{
}

void Editor::DoInitalize()
{
	auto ent = m_Scene->CreateEntity();
	//---------------------------------------------------
	//  Rendering Setup
	//-------------------------------------------------
	m_Renderer->InitalizePBREnvironmentMaps("./Assets/Textures/Footprint_Court_2k.hdr");

	EI.RegisterComponent<int>();
}

void Editor::DoUpdate(double deltaTime)
{
	ImGui_ImplSdlGL3_ProcessEvent(&InputEvent);
	ImGui_ImplSdlGL3_NewFrame(m_Window);
	
	//Draw Menu Bar
	DrawMenuBar(deltaTime);


	//Draw Optional Windows
	if(IsEntityListActive)
		DrawEntityList();
	if (IsEntityInspectorActive)
		DrawEntityInspector();


	m_Renderer->UIBuffer.Bind();
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui::Render();
	m_Renderer->UIBuffer.UnBind();
}

//---------------------------------------------------
//	Menu Bar Controlls What Windows are active, and saving/Loading
//---------------------------------------------------
void Editor::DrawMenuBar(double deltaTime)
{
	//Create Menu Bar
	ImGui::BeginMainMenuBar();

	//File Menu Item
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New Scene")) {}
		if (ImGui::MenuItem("Open Scene")) {}
		ImGui::EndMenu();
	}

	//Edit Menu Item
	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Render Options")) {}
			if (ImGui::MenuItem("Option2")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}

	//View Menu Item
	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("EntityList", NULL, IsEntityListActive)) { IsEntityListActive = !IsEntityListActive; }
		if (ImGui::MenuItem("EntityInspector", NULL, IsEntityInspectorActive)) { IsEntityInspectorActive = !IsEntityInspectorActive; }
		ImGui::EndMenu();
	}

	//Display FPS
	ImGui::Text("%i FPS (%f ms)", int(1 / deltaTime), deltaTime);

	//End Menu Bar
	ImGui::EndMainMenuBar();
	
}

void Editor::DrawEntityList()
{
	//Start Window
	ImGui::Begin("Entity List");

	//Button Creates entity
	if (ImGui::Button("Add Entity"))
	{
		m_Scene->CreateEntity();
		m_Scene->m_EntityNames.push_back("Entity");
	}

	//Convert Entity Names to const char* to be read by List
	std::vector<const char *> nameArray;
	nameArray.reserve(m_Scene->m_EntityNames.size());
	for (int i = 0; i < m_Scene->m_EntityNames.size(); ++i)
	{
		nameArray.push_back(m_Scene->m_EntityNames[i].c_str());
	}
	
	//Create Listbox containing the Entities belonging to current Scene.
	ImGui::ListBox("", &SelectedEntity, nameArray.data(), m_Scene->GetNumEntities(), 4);

	//End Window
	ImGui::End();
}

//This Is A Mockup for an entity inspector
void Editor::DrawEntityInspector()
{
	ImGui::Begin("Entity Inspector");
	ImGui::Text("Entity: Player");

	if (ImGui::CollapsingHeader("Transform")) {}
	if (ImGui::CollapsingHeader("Mesh")) {}
	if (ImGui::CollapsingHeader("Material")) {}

	if (ImGui::Button("Add Component")) {}


	ImGui::End();
}
