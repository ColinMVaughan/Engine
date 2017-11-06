#include "EditorUI.h"
#include "imgui.h"
#include "imgui_SDL.h"
#include <ECS.h>
#include<TestComponent.h>
#include <CameraSystem.h>
#include <CoreComponentRegistration.h>

Editor::Editor()
{
}

void Editor::DoInitalize()
{
	//---------------------------------------------------
	//  Some Manditory Scene Setup
	//-------------------------------------------------
	//Set Skybox for Scene
	m_Renderer->InitalizePBREnvironmentMaps("./Assets/Textures/Footprint_Court_2k.hdr");

	//Allows the user to move the camera in debug mode.
	m_Scene->AddSystem<DebugCameraControlSystem>();
	
	//Create Entity that will act as the editor camera
	auto EditorCamera = m_Scene->CreateEntity(); //create entity
	m_Renderer->SetCamera(EditorCamera.AddComponent<Camera>()); //add camera component and register it with the renderer
	EditorCamera.AddComponent<Transform>(); //Add transform for positioning camera

	//set the style to the daek theme
	ImGui::StyleColorsDark();
}

void Editor::DoUpdate(double deltaTime)
{
	ImGui_ImplSdlGL3_ProcessEvent(&InputEvent);
	ImGui_ImplSdlGL3_NewFrame(m_Window);
	
	//Draw Menu Bar
	DrawMenuBar(deltaTime);


	//Draw Optional Windows
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
		if (ImGui::MenuItem("EntityInspector", NULL, IsEntityInspectorActive)) { IsEntityInspectorActive = !IsEntityInspectorActive; }
		if (ImGui::MenuItem("SystemList", NULL, IsEntityListActive)) { IsEntityListActive = !IsEntityListActive; }
		if (ImGui::MenuItem("Resource Manager", NULL, IsEntityInspectorActive)) {}
		ImGui::EndMenu();
	}

	//Display FPS
	ImGui::Text("%i FPS (%f ms)", int(1 / deltaTime), deltaTime);

	//End Menu Bar
	ImGui::EndMainMenuBar();
	
}


//------------------------------------------------------
//					ENTITY INSPECTOR
//------------------------------------------------------
void Editor::DrawEntityInspector()
{
	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Entity Inspector", &IsEntityInspectorActive, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Add"))
			{
				if (ImGui::MenuItem("Entity"))
				{
					m_Scene->CreateEntity();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		//Enity View
		static int selected = 0;
		ImGui::BeginChild("Left Pane", ImVec2(150, 0), true);
		//**************
		// This is where you list the entities
		for (int i = 0; i < m_Scene->GetNumEntities(); ++i)
		{
			char label[128];
			sprintf(label, "Entity %d", i);
			if (ImGui::Selectable(label, selected == i))
				selected = i;
		}
		//**************
		ImGui::EndChild();
		ImGui::SameLine();


		//Component View
		ImGui::BeginGroup();

		ImGui::BeginChild("Item View", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()));
		ImGui::Text("MyObject %d", selected);
		ImGui::Separator();
		ImGui::TextWrapped("COMPONETNS WILL GO HERE");
		
		for (int i = 0; i < 10; i++)
		{
			ImGui::CollapsingHeader("Component 1"); 
		}

		ImGui::EndChild();

		ImGui::BeginChild("Buttons");
		if (ImGui::Button("Add Component")) { ImGui::OpenPopup("ComponentList"); std::cout<<"Bla"; }
		if (ImGui::BeginPopup("ComponentList"))
		{
			ImGui::Text("Components");
			ImGui::Separator();

			auto registry = ECS::detail::GetComponentRegistry();
			for (auto it = registry.begin(); it != registry.end(); ++it)
			{
				if (ImGui::Selectable(it->first.c_str()))
				{
					auto AddComponentFunc = it->second;
					AddComponentFunc(m_Scene, m_Scene->GetEntity(selected));
				}

			}

			ImGui::EndPopup();
		}


		ImGui::SameLine();
		if (ImGui::Button("Remove Component")) {}
		ImGui::EndChild();
		ImGui::EndGroup();



	}
	ImGui::End();
}
