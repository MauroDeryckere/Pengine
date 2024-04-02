#include "ImGUIWindow.h"

#include <SDL.h>

#include "imgui.h"
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl2.h>

namespace Pengin
{
	void ImGUIWindow::Create(SDL_Window* pWindow)
	{
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();

		auto& io = ImGui::GetIO();
		io.ConfigWindowsMoveFromTitleBarOnly = true;
		//io.ConfigFlags |= ImGuiConfigFlags_;

		ImGui::GetMainViewport();

		ImGui_ImplSDL2_InitForOpenGL(pWindow, SDL_GL_GetCurrentContext());
		ImGui_ImplOpenGL2_Init();
	}

	void ImGUIWindow::Destroy()
	{
		ImGui_ImplOpenGL2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGUIWindow::ProcessEvent(SDL_Event& event)
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
	}

	void ImGUIWindow::BeginRender() const
	{
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
	}

	void ImGUIWindow::EndRender() const
	{
		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	}

	bool ImGUIWindow::UsedMouse() const noexcept
	{
		return ImGui::GetIO().WantCaptureMouse;
	}

	bool ImGUIWindow::UsedKeyboard() const noexcept
	{
		return ImGui::GetIO().WantCaptureKeyboard;
	}
}
