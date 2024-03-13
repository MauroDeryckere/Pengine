#include <SDL.h>
#include "InputManager.h"

#include <backends/imgui_impl_sdl2.h>

#include <iostream>

#include "Windows.h"
#include "XInput.h"

#include "InputDevice.h"
#include "InputController.h"
#include "InputKeyboard.h"

namespace Pengin
{
	InputManager::InputManager() :
		m_InputDevices(static_cast<size_t>(Devices::DEVICE_COUNT))
	{
		m_InputDevices[static_cast<size_t>(Devices::Keyboard)].reset(new InputKeyboard{});
		m_InputDevices[static_cast<size_t>(Devices::Controller)].reset(new InputController{});
	}

	bool InputManager::ProcessInput()
	{
		for (auto& device : m_InputDevices)
		{
			device->ProcessInputState();
			device->ProcessMappedActions();
		}

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				return false;
			}

			ImGui_ImplSDL2_ProcessEvent(&e);
		}
		return true;
	}

	void InputManager::MapControllerAction(ControllerButton button, InputState inputState, std::unique_ptr<InputCommand> pInputAction)
	{
		m_InputDevices[static_cast<size_t>(Devices::Controller)]->MapActionToInput(static_cast<unsigned>(button), inputState, std::move(pInputAction));
	}

	void InputManager::MapKeyboardAction(KeyBoardKey key, InputState inputState, std::unique_ptr<InputCommand> pInputAction)
	{
		m_InputDevices[static_cast<size_t>(Devices::Keyboard)]->MapActionToInput(static_cast<unsigned>(key), inputState, std::move(pInputAction));
	}

	//void InputManager::MapMouseAction(MouseButton button, InputState inputState, std::unique_ptr<InputCommand> pInputAction)
	//{
		//m_InputDevices[static_cast<size_t>(Devices::Mouse)]->MapActionToInput(static_cast<unsigned>(button), inputState, std::move(pInputAction));
	//}
}


