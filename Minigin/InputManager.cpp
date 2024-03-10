#include <SDL.h>
#include "InputManager.h"

#include <backends/imgui_impl_sdl2.h>

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}

		ImGui_ImplSDL2_ProcessEvent(&e);

		/*if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}*/
	}

	return true;
}
