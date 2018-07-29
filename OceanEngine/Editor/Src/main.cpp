//=============================================================================================================
//								OCEAN ENGINE EDITOR
//								  Colin Vaughan 
//
// The Editor is used to design and edit game scenes. The editor can be used to add and modify 
// entities/components/systems/assets within any given scene. The Editor can link with user created 
// code and assets at runtime, using DLL hotloading & asset loading. Exposed component & system parameters can be 
// modified in realtime using the editor.
//
//===============================================================================================================

#include <GL/glew.h>
#include <SDL\SDL.h>

#include <Application.h>
#include <glm/gtc/matrix_transform.hpp>
#include <PhysicsSystem.h>
#include <RenderSystem.h>
#include <PlayerControlSystem.h>
#include <Voxels.h>
#include <VoxelDestructionSystem.h>

#include <imgui.h>
#include "imgui_SDL.h"

#include <EditorUI.h>

Editor *editor = new Editor();

int main(int argc, char **argv)
{
	//-------------------------------------------------------------------------
	//		Init SDL
	//-------------------------------------------------------------------------
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		std::cout << "SDL was not Initalized.";
		return 1;
	}

	Uint32 WindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE/*| SDL_WINDOW_FULLSCREEN*/;
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


	editor->m_Window = SDL_CreateWindow("Ocean Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, WindowFlags);
	SDL_GLContext glContext = SDL_GL_CreateContext(editor->m_Window);

	//-----------------------------------------------------------
	//		Init GLEW
	//---------------------------------------------------------
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW was not initalized\n";
		system("pause");
		return 0;
	}

	//-----------------------------------------------------------
	//	Init IMGUI
	//-----------------------------------------------------------
	ImGui::CreateContext();
	ImGui_ImplSdlGL3_Init(editor->m_Window);

	auto& io = ImGui::GetIO();
	//io.Fonts->AddFontDefault();
	ImFontConfig config;
	config.OversampleH = 3;
	config.OversampleV = 3;


	ImFont* font = io.Fonts->AddFontFromFileTTF("./Assets/Roboto-Regular.ttf", 16.5f, &config);

	//-----------------------------------------------------------
	//	Init & Run Application
	//------------------------------------------------------------
	editor->Initalize();
	while (editor->Running)
	{
		editor->Update();

	}
	editor->Unload();


	ImGui_ImplSdlGL3_Shutdown();
	return 0;

}