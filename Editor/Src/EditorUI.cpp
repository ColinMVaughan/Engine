#include "EditorUI.h"
#include "imgui.h"
#include "imgui_SDL.h"
#include <ECS.h>
#include<TestComponent.h>
#include <CameraSystem.h>
#include <CoreComponentRegistration.h>
#include <glm/gtc/matrix_transform.hpp>
#include <SceneLoader.h>

Editor::Editor()
{
}

void Editor::DoInitalize()
{
	//---------------------------------------------------
	//  Some Manditory Scene Setup
	//-------------------------------------------------
	//Set Skybox for Scene
	m_Renderer->InitalizePBREnvironmentMaps("./Assets/Textures/GCanyon_C_YumaPoint_3k.hdr");

	//Allows the user to move the camera in debug mode.
	RegisterKeyboardCallback(m_Scene->AddSystem<DebugCameraControlSystem>());
	m_Scene->AddSystem<EventSystemTest>();

	//Create Entity that will act as the editor camera
	auto EditorCamera = m_Scene->CreateEntity(); //create entity

	//add camera component and register it with the renderer
	auto cam = EditorCamera.AddComponent<Camera>();
	cam->m_Projection = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
	m_Renderer->SetCamera(cam); 

	EditorCamera.AddComponent<Transform>(); //Add transform for positioning camera
	EditorCamera.AddComponent<DebugControl>();

	//set the style to the dark theme
	ImGui::StyleColorsDark();
}

void Editor::PreUpdate(double deltaTime)
{
	NewEvent = false;
	while (SDL_PollEvent(&InputEvent))
	{
		NewEvent = true;
		ImGui_ImplSdlGL3_ProcessEvent(&InputEvent);
		switch (InputEvent.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			if (InputEvent.button.button == SDL_BUTTON_RIGHT)
				LookMode = true;
			break;
		case SDL_MOUSEBUTTONUP:
			if (InputEvent.button.button == SDL_BUTTON_RIGHT)
				LookMode = false;
			break;

		case SDL_KEYDOWN:
			if(LookMode)
				KeyDown(InputEvent.key);
			break;
		case SDL_KEYUP:
			if(LookMode)
				KeyUp(InputEvent.key);
			break;
		case SDL_MOUSEMOTION:
			if(LookMode)
				MouseMoved(InputEvent.motion);
			break;
		case SDL_QUIT:
			Running = false;
			break;
		}
	}
}

void Editor::DoUpdate(double deltaTime)
{
	ImGui_ImplSdlGL3_NewFrame(m_Window);
	
	//Draw Menu Bar
	DrawMenuBar(deltaTime);


	//Draw Optional Windows
	if (IsEntityInspectorActive)
		DrawEntityInspector();
	if (IsResourceManagerActive)
		DrawResourceManager();

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
		if (ImGui::MenuItem("Save Scene")) { m_Scene->SaveScene("./Assets/DemoScene.Scene"); }
		if (ImGui::MenuItem("Open Scene")) { m_Scene->LoadScene("./Assets/DemoScene.Scene"); }
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
		if (ImGui::MenuItem("Resource Manager", NULL, IsResourceManagerActive)) { IsResourceManagerActive = !IsResourceManagerActive; }
		ImGui::EndMenu();
	}

	//Display FPS
	ImGui::Button("Start");
	ImGui::Button("Stop");
	ImGui::Text("%f FPS (%f ms)", ceil(1.0 / deltaTime), deltaTime);

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
			if (ImGui::Selectable(m_Scene->GetEntity(i).GetName().c_str(), selected == i))
				selected = i;

			//I Know this nested if statement seems unessisary, but ImGUI throws an exception otherwise
			if(selected==i)
			{
				if (ImGui::BeginPopupContextItem("Rename"))
				{
					//Buffer used if the user wishes to rename an entity
					char renameBuffer[128] = "";
					if (ImGui::InputText("Rename Entity", renameBuffer, sizeof(renameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
						m_Scene->GetEntity(selected).SetName(std::string(renameBuffer));

					ImGui::EndPopup();
				}
			}

		}


		//**************
		ImGui::EndChild();
		ImGui::SameLine();


		//Component View
		auto registry = ECS::detail::GetComponentRegistry();

		ImGui::BeginGroup();

		ImGui::BeginChild("Item View", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()));
		ImGui::Text("MyObject %d", selected);
		ImGui::Separator();
		ImGui::TextWrapped("COMPONETNS WILL GO HERE");
		

		for (auto it = registry.begin(); it != registry.end(); ++it)
		{
			auto AddComponentFunc = it->second;
			if (AddComponentFunc(m_Scene, m_Scene->GetEntity(selected), ECS::detail::ComponentAction::Check))
			{
				if(ImGui::CollapsingHeader(it->first.c_str()))
				{
					ECS::DisplayComponentParameters(it->first, m_Scene, m_Scene->GetEntity(selected));
				}
			}

		}

		ImGui::EndChild();

		ImGui::BeginChild("Buttons");
		if (ImGui::Button("Add Component")) { ImGui::OpenPopup("ComponentList"); }
		if (ImGui::BeginPopup("ComponentList"))
		{
			ImGui::Text("Components");
			ImGui::Separator();

			for (auto it = registry.begin(); it != registry.end(); ++it)
			{
				if (ImGui::Selectable(it->first.c_str()))
				{
					auto AddComponentFunc = it->second;
					AddComponentFunc(m_Scene, m_Scene->GetEntity(selected), ECS::detail::ComponentAction::Add);
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

void Editor::DrawResourceManager()
{
	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Resource Manager", &IsResourceManagerActive, ImGuiWindowFlags_MenuBar))
	{

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Add Resource"))
				{

				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Text("Resources");
		ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
		ImGui::BeginChild("List", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 300), true, ImGuiWindowFlags_HorizontalScrollbar);
		m_AssetManager.DisplayDirectoryContents();
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("Info", ImVec2(0, 300), true);
		ImGui::Text("Resource Type: ");
		ImGui::Text("Path: ");
		ImGui::Text("Size: ");
		ImGui::EndChild();

		ImGui::PopStyleVar();
	}
	ImGui::End();
}
