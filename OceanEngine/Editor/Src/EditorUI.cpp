#include "EditorUI.h"
#include "imgui.h"
#include "imgui_SDL.h"
#include <ECS.h>
#include<TestComponent.h>
#include <CameraSystem.h>
#include <CoreComponentRegistration.h>
#include <glm/gtc/matrix_transform.hpp>
#include <SceneLoader.h>
#include "Asset.h"
#include <DynamicCodeDetail.h>
#include <imgui_internal.h>

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
	//m_Renderer->CreateUIBuffer(1920, 1080);

	SDL_GetWindowSize(m_Window, &WindowSizeX, &WindowSizeY);

	//Allows the user to move the camera in debug mode.
	RegisterKeyboardCallback(m_Scene->AddCoreSystem<DebugCameraControlSystem>());
	m_Scene->AddSystem<EventSystemTest>();
	m_Scene->AddSystem<BlockResetSystem>();

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

	//Load all assets in the asset directory
	m_AssetManager->LoadAllAssets();

	CodeReload.LoadDLL();
	auto reg = ECS::DynamicDetail::GetDynamicSystemRegistry();
	for (auto it = reg.begin(); it != reg.end(); it++)
	{
		it->second(m_Scene, ECS::detail::AddUserSystem);
	}

}

void Editor::PreUpdate(double deltaTime)
{
	m_DeltaTime = deltaTime;
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
	//DrawMenuBar(deltaTime);


	//Draw Optional Windows
	//if (IsEntityInspectorActive)
	//	DrawEntityInspector();
	//if (IsResourceManagerActive)
	//	DrawResourceManager();
	//if (IsRenderSettingsActive)
	//	DrawRenderSettings();
	if (!GameCompiling)
	{
		if (GameRunning)
			m_Scene->UpdateSystems(deltaTime);
		else
			m_Scene->UpdateCoreSystems(deltaTime);
	}




}

void Editor::PostUpdate(double deltaTime)
{
	DrawEditor();

	if (IsMaterialEditorActive)
		DrawMaterialWindow();

	//m_Renderer->UIBuffer.Bind();
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui::Render();
	ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(m_Window);
	//m_Renderer->UIBuffer.UnBind();

	//submit our final frame
	//m_Renderer->SubmitFrame();
}

//---------------------------------------------------
//	Menu Bar Controlls What Windows are active, and saving/Loading
//---------------------------------------------------
void Editor::DrawMenuBar(double deltaTime)
{
	//Create Menu Bar
	ImGui::BeginMenuBar();

	//File Menu Item
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Save Scene")) { m_Scene->SaveScene("./Assets/DemoScene.Scene"); }
		if (ImGui::MenuItem("Open Scene")) { m_Scene->LoadScene("./Assets/DemoScene.Scene"); }
		if (ImGui::MenuItem("Quit")) { Running = false; }
		ImGui::EndMenu();
	}

	//Edit Menu Item
	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Render Options", NULL, IsRenderSettingsActive)) { IsRenderSettingsActive = !IsRenderSettingsActive; }
			if (ImGui::MenuItem("Option2")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Create"))
	{
		if (ImGui::MenuItem("Entity"))
		{
			m_Scene->CreateEntity();
		}

		if (ImGui::MenuItem("Renderable"))
		{
			ECS::Entity& e = m_Scene->CreateEntity();

			ECS::AddComponentFromString("MeshFilter", m_Scene, e);
			ECS::AddComponentFromString("MaterailFilter", m_Scene, e);
			ECS::AddComponentFromString("Transform", m_Scene, e);
		}
		if (ImGui::MenuItem("PointLight"))
		{
			ECS::Entity& e = m_Scene->CreateEntity();

			ECS::AddComponentFromString("PointLightComponent", m_Scene, e);
			ECS::AddComponentFromString("Transform", m_Scene, e);
		}
		ImGui::EndMenu();
	}

	//View Menu Item
	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("Material Editor", NULL, IsMaterialEditorActive)) { IsMaterialEditorActive = !IsMaterialEditorActive; }

		ImGui::EndMenu();
	}




	//ImGui::Text("%f FPS (%f ms)", ceil(1.0 / deltaTime), deltaTime);

	//End Menu Bar
	ImGui::EndMenuBar();
	
}




void Editor::DrawEditor()
{

	//Set Some basic Style settings
	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 2.0);
	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.12, 0.12, 0.12, 1));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.12, 0.12, 0.12, 1));

	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0.75, 0.85, 1));
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0.75, 0.85, 1));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0, 0.75, 0.85, 1));
	
	//Set up window flags
	ImGuiWindowFlags wFlags = 0;
	wFlags |= ImGuiWindowFlags_NoCollapse;
	wFlags |= ImGuiWindowFlags_NoMove;
	wFlags |= ImGuiWindowFlags_NoTitleBar;
	wFlags |= ImGuiWindowFlags_MenuBar;
	wFlags |= ImGuiWindowFlags_NoResize;

	int w, h;
	SDL_GetWindowSize(m_Window, &w, &h);
	

	ImGui::SetNextWindowSize(ImVec2(w, h));
	ImGui::SetNextWindowPos(ImVec2(0, 0));

	//Setar Window
	ImGui::Begin("MyWindow", nullptr, wFlags);
	DrawMenuBar(0.0); //draw menu bar


	float windowHeight = ImGui::GetContentRegionMax().y - 27;
	float windowWidth = ImGui::GetContentRegionMax().x - 22;

	static float LeftPane = windowWidth * 0.75;
	static float Left_UpPane = windowHeight * 0.75;
	static float Left_DownPane = windowHeight * 0.25;

	static float RightPane = windowWidth * 0.25;
	static float Right_UpPane = windowHeight * 0.5;
	static float Right_DownPane = windowHeight * 0.5;

	//Reset window data if the window has been resized
	if (WindowSizeX != w || WindowSizeY != h)
	{
		LeftPane = windowWidth * 0.75;
		Left_UpPane = windowHeight * 0.75;
		Left_DownPane = windowHeight * 0.25;

		RightPane = windowWidth * 0.25;
		Right_UpPane = windowHeight * 0.5;
		Right_DownPane = windowHeight * 0.5;

		WindowSizeX = w;
		WindowSizeY = h;
	}



	DrawSplitter(true, 7.0f, &LeftPane, &RightPane, 8, 8, windowHeight);

	ImGui::BeginChild("LeftPane", ImVec2(LeftPane, windowHeight), true);
	DrawSplitter(false, 15.0f, &Left_UpPane, &Left_DownPane, 8, 8, LeftPane);

	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12, 0.12, 0.12, 1.0));

	ImGui::BeginChild("SceneView", ImVec2(LeftPane, Left_UpPane), true);
	DrawSceneWindow();
	ImGui::EndChild();

	ImGui::PopStyleColor();


	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.21, 0.21, 0.21, 1.0));
	ImGui::BeginChild("AssetView", ImVec2(LeftPane, Left_DownPane - 19), true);
	DrawAssetsWindow();
	ImGui::EndChild();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();

	ImGui::EndChild();



	ImGui::SameLine();




	ImGui::BeginChild("RightPane", ImVec2(windowWidth - LeftPane, windowHeight), true);
	DrawSplitter(false, 15.0f, &Right_UpPane, &Right_DownPane, 8, 8, windowWidth - LeftPane);

	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.21, 0.21, 0.21, 1.0));

	ImGui::BeginChild("EntityView", ImVec2(RightPane, Right_UpPane), true);
	DrawEntityListWindow();
	ImGui::EndChild();

	ImGui::BeginChild("InspectorView", ImVec2(RightPane, Right_DownPane - 19), true);
	DrawInspectorWindow();
	ImGui::EndChild();

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	ImGui::EndChild();



	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}



void Editor::DrawEntityListWindow()
{
	ImGui::Spacing();
	ImGui::Indent(5.0);
	ImGui::Text("Entity List");


	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.4, 0.4, 0.4, 1));
	ImGui::Separator();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.168, 0.168, 0.168, 1.0));
	ImGui::Indent(7.0);

	ImGui::BeginChild("EntiotyList", ImVec2(ImGui::GetContentRegionAvail().x - 7, ImGui::GetContentRegionAvail().y - 35));


	
	
	
	//**************
	// This is where you list the entities
	for (int i = 0; i < m_Scene->GetNumEntities(); ++i)
	{
		if (ImGui::Selectable(m_Scene->GetEntity(i).GetName().c_str(), SelectedEntity == i))
			SelectedEntity = i;

		//I Know this nested if statement seems unessisary, but ImGUI throws an exception otherwise
		if (SelectedEntity == i)
		{
			if (ImGui::BeginPopupContextItem("Rename"))
			{
				//Buffer used if the user wishes to rename an entity
				char renameBuffer[128] = "";
				if (ImGui::InputText("Rename Entity", renameBuffer, sizeof(renameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
					m_Scene->GetEntity(SelectedEntity).SetName(std::string(renameBuffer));

				ImGui::EndPopup();
			}
		}

	}


	//Draws any relevent gizmos of the selected entity
	if (!GameCompiling)
		m_SystemManager->UpdateGizmos(m_Scene->GetEntity(SelectedEntity));

	//**************
	//ImGui::EndChild();
	//If the user drags a prefab in, add it to the scene
	Asset<PrefabEntity> prefab;
	if (EditorRequestAsset<PrefabEntity>(prefab, "Prefab", ""))
	{
		m_Scene->LoadAndSpawnPrefabEntity(prefab.m_Asset.m_Filepath);
	}




	ImGui::EndChild();
	ImGui::PopStyleColor();

	ImGui::Separator();
	ImGui::Text("Entities: 4");

	ImGui::PopStyleColor();
}







void Editor::DrawInspectorWindow()
{

	auto registry = ECS::detail::GetComponentRegistry();
	auto usercodeRegistry = ECS::DynamicDetail::GetDynamicComponentRegistry();

	ImGui::Spacing();

	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.12, 0.12, 0.12, 1));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12, 0.12, 0.12, 1));

	ImGui::Indent(5.0);
	if (ImGui::BeginCombo("", "+ Add Component..."))
	{
		//LIST AVAILABLE COMPONENTS
		//Core components registry
		for (auto it = registry.begin(); it != registry.end(); ++it)
		{
			if (ImGui::Selectable(it->first.c_str()))
			{
				auto AddComponentFunc = it->second;
				AddComponentFunc(m_Scene, m_Scene->GetEntity(SelectedEntity), ECS::detail::ComponentAction::Add);
			}

		}

		//User Components Regsitry
		for (auto it = usercodeRegistry.begin(); it != usercodeRegistry.end(); it++)
		{
			if (ImGui::Selectable(it->first.c_str()))
			{
				auto AddComponentFunc = it->second;
				AddComponentFunc(m_Scene, m_Scene->GetEntity(SelectedEntity), ECS::detail::ComponentAction::Add);
			}
		}
		ImGui::EndCombo();
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();


	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.4, 0.4, 0.4, 1));
	ImGui::Separator();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.168, 0.168, 0.168, 1.0));
	ImGui::Indent(7.0);

	ImGui::BeginChild("EntityList", ImVec2(ImGui::GetContentRegionAvail().x - 7, ImGui::GetContentRegionAvail().y - 7));

	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.13, 0.13, 0.13, 1));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3, 0.3, 0.3, 1));


	if (!GameCompiling)
	{
	//LIST ATTACHED COMPONENTS
	for (auto it = registry.begin(); it != registry.end(); ++it)
	{
		auto AddComponentFunc = it->second;
		if (AddComponentFunc(m_Scene, m_Scene->GetEntity(SelectedEntity), ECS::detail::ComponentAction::Check))
		{
			if (ImGui::CollapsingHeader(it->first.c_str()))
			{
				ECS::DisplayComponentParameters(it->first, m_Scene, m_Scene->GetEntity(SelectedEntity));
			}
		}

	}


		for (auto it = usercodeRegistry.begin(); it != usercodeRegistry.end(); ++it)
		{
			auto AddComponentFunc = it->second;
			if (AddComponentFunc(m_Scene, m_Scene->GetEntity(SelectedEntity), ECS::detail::ComponentAction::Check))
			{
				if (ImGui::CollapsingHeader(it->first.c_str()))
				{
					ECS::DisplayComponentParameters(it->first, m_Scene, m_Scene->GetEntity(SelectedEntity));
				}
			}

		}
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::EndChild();
	ImGui::PopStyleColor();


	ImGui::PopStyleColor();
}




void Editor::DrawAssetsWindow()
{
	ImGui::BeginChild("Assets");
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.168, 0.168, 0.168, 1.0));

	ImGui::Text("Asset Directory");
	ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
	ImGui::BeginChild("List", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.2f, ImGui::GetContentRegionAvail().y - 5), true, ImGuiWindowFlags_HorizontalScrollbar);
	m_AssetManager->DisplayAssetDirectory();
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("Info", ImVec2(ImGui::GetContentRegionAvail().x - 5, ImGui::GetContentRegionAvail().y - 5), true);
	m_AssetManager->DisplayDirectoryContents();
	ImGui::EndChild();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
	ImGui::EndChild();
}

float aspectRatio(float imageX, float imageY, float screenX, float screenY)
{
	float screenAspect = screenX / screenY;
	float imageAspect = imageX / imageY;

	float scaleFactor;
	if (screenAspect > imageAspect)
		scaleFactor = screenY / imageY;
	else
		scaleFactor = screenX / imageX;

	return scaleFactor;
}


ImVec4 LerpColour(ImVec4& colour1, ImVec4& colour2, double t)
{
	ImVec4 returnColour;

	returnColour.x = colour1.x + t * (colour2.x - colour1.x);
	returnColour.y = colour1.y + t * (colour2.y - colour1.y);
	returnColour.z = colour1.z + t * (colour2.z - colour1.z);
	returnColour.w = colour1.w + t * (colour2.w - colour1.w);

	return returnColour;
}
ImVec4 PulseColour(ImVec4& colour1, ImVec4& colour2, double deltaTime, bool run)
{
	static double pulseTime = 0.0;

	if (!run)
		return colour1;

	pulseTime +=deltaTime;

	const float pi = 3.14;
	const float frequency = 1; // Frequency in Hz

	return LerpColour(colour1, colour2, 0.5*(1 + sin(2 * pi * frequency * pulseTime)));
}

ImVec4 SwitchColour( ImVec4 colour1, ImVec4 colour2, bool SwitchCol)
{
	if (SwitchCol)
	{
		return colour2;
	}
	return colour1;
}

void Editor::DrawSceneWindow()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.21, 0.21, 0.21, 1.0));

	//------------------------------------------------------------------------------------------
	//Actions
	ImGui::BeginChild("Actions", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, 40));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.21, 0.21, 0.21, 1.0));
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Indent(5.0);
	ImGui::PushStyleColor(ImGuiCol_Button, PulseColour(ImVec4(0.21, 0.21, 0.21, 1.0), ImVec4(0, 0.75, 0.85, 1),m_DeltaTime, GameRunning));
	if (ImGui::Button("Play", ImVec2(50, 20)))
	{
		GameRunning = true;
		m_SystemManager->StartSystems();
	}
	ImGui::PopStyleColor();
	ImGui::SameLine();


	ImGui::PushStyleColor(ImGuiCol_Button, SwitchColour(ImVec4(0.21, 0.21, 0.21, 1.0), ImVec4(0.96, 0.18, 0.18, 1), GameRunning));
	if (ImGui::Button("Stop", ImVec2(50, 20)))
	{
		GameRunning = false;
		m_SystemManager->StopSystems();
	}
	ImGui::PopStyleColor();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, PulseColour(ImVec4(0.21, 0.21, 0.21, 1.0), ImVec4(0, 0.75, 0.85, 1), m_DeltaTime, GameCompiling));
	if (ImGui::Button("Compile", ImVec2(60, 20)))
	{
		TriggerDLLUnload();
		TriggerDLLReload();
		//ImGui::OpenPopup("Reload...");
	}
	ImGui::PopStyleColor();

	if (ImGui::BeginPopupModal("Reload...", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Waiting for User Code to compile...");
		if (ImGui::Button("Reload", ImVec2(50, 0)))
		{
			//TriggerDLLReload();
			ImGui::CloseCurrentPopup();
		}


		ImGui::EndPopup();
	}

	ImGui::PopStyleColor();
	ImGui::EndChild();
	//---------------------------------------------------------------------------------

	ImGui::SameLine();

	//---------------------------------------------------------------------------------
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.12, 0.12, 0.12, 1));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12, 0.12, 0.12, 1));

	ImGui::BeginChild("SceneSelect", ImVec2(ImGui::GetContentRegionAvail().x, 40));
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Indent(8.0);
	ImGui::BeginCombo("", "Default Scene");
	ImGui::EndChild();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	//--------------------------------------------------------------------------------


	ImGui::Spacing();

	//Gat FrameBuffer


	ImTextureID frame = (void*)m_Renderer->FinalBuffer->GetColorHandle(0);

	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	contentRegion.y -= 2;

	ImVec2 image(1280, 720);

	image.x *= aspectRatio(1280, 720, contentRegion.x, contentRegion.y);
	image.y *= aspectRatio(1280, 720, contentRegion.x, contentRegion.y);

	ImVec2 position;
	position.y = (contentRegion.y - image.y) / 2;
	position.x = (contentRegion.x - image.x) / 2;

	ImGui::BeginChild("Scene");
	ImGui::Indent(position.x);
	ImGui::Image(frame, image, ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 0));
	ImGui::EndChild();

	ImGui::PopStyleColor();
}

bool Editor::DrawSplitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size)
{
	using namespace ImGui;
	ImGuiContext& g = *ImGui::GetCurrentContext();

	ImGuiWindow* window = g.CurrentWindow;
	ImGuiID id = window->GetID("##Splitter");
	ImRect bb;
	bb.Min.x = window->DC.CursorPos.x + ImVec2(split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1)).x;
	bb.Min.y = window->DC.CursorPos.y + ImVec2(split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1)).y;

	bb.Max.x = bb.Min.x + ImVec2(CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f)).x;
	bb.Max.y = bb.Min.y + ImVec2(CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f)).y;

	return SplitterBehavior(id, bb, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
}



void Editor::TriggerDLLReload()
{
	//Trigger a Rebuild of the User code project
	system("MSBuild ../UserCode.vcxproj /p:Configuration=Debug /p:BuildProjectReferences=false /t:Build");

	bool reloaded = CodeReload.LoadDLL();
	auto reg = ECS::DynamicDetail::GetDynamicSystemRegistry();
	for (auto it = reg.begin(); it != reg.end(); it++)
	{
		it->second(m_Scene, ECS::detail::AddUserSystem);
	}

	//Recreate Camera (This is temporary, we will find a better spot for this code)
	auto EditorCamera = m_Scene->CreateEntity(); //create entity

	//add camera component and register it with the renderer
	auto cam = EditorCamera.AddComponent<Camera>();
	cam->m_Projection = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
	m_Renderer->SetCamera(cam);

	EditorCamera.AddComponent<Transform>(); //Add transform for positioning camera
	EditorCamera.AddComponent<DebugControl>();

	m_Scene->LoadScene("./Assets/DemoScene.scene");
	GameCompiling = false;

	return;
}

void Editor::TriggerDLLUnload()
{
	m_Scene->SaveScene("./Assets/DemoScene.scene");
	GameCompiling = true;
	GameRunning = false;
	
	m_Scene->Clear();
	CodeReload.UnloadDLL();
}



void Editor::DrawMaterialWindow()
{
	ImGui::Begin("CreateMaterialWindow");
	
	ImGui::BeginChild("Material Preview",ImVec2(ImGui::GetContentRegionAvail().x * 0.33, ImGui::GetContentRegionAvail().y));
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("Material Settings",ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
	
	//If the user drags a prefab in, add it to the scene
	Asset<Texture> albedo;
	if (EditorRequestAsset<Texture>(albedo, "Albedo", ""))
	{
	}
	ImGui::Selectable("Albedo");


	//If the user drags a prefab in, add it to the scene
	Asset<Texture> normal;
	if (EditorRequestAsset<Texture>(normal, "Normal", ""))
	{
	}
	ImGui::Selectable("Normal");

	//If the user drags a prefab in, add it to the scene
	Asset<Texture> metallic;
	if (EditorRequestAsset<Texture>(metallic, "Normal", ""))
	{
	}
	ImGui::Selectable("Metallic");

	//If the user drags a prefab in, add it to the scene
	Asset<Texture> roughness;
	if (EditorRequestAsset<Texture>(roughness, "Roughness", ""))
	{
	}
	ImGui::Selectable("Roughness");

	//If the user drags a prefab in, add it to the scene
	Asset<Texture> emissive;
	if (EditorRequestAsset<Texture>(emissive, "Emissive", ""))
	{
	}
	ImGui::Selectable("Emissive");


	//If the user drags a prefab in, add it to the scene
	Asset<Texture> ao;
	if (EditorRequestAsset<Texture>(ao, "AO", ""))
	{
	}
	ImGui::Selectable("AO");


	ImGui::EndChild();

	
	ImGui::End();
}

void Editor::OpenScene()
{
	m_Scene->LoadScene("");
	auto ent = m_Scene->GetEntity(0); //get the camera entity (should only be transform)

	m_Renderer->SetCamera(ent.AddComponent<Camera>());
	ent.AddComponent<DebugControl>();
}
