#include <SDL.h>
#include "InputManager.h"

#include <backends/imgui_impl_sdl2.h>

#include <iostream>

#include "Windows.h"
#include "XInput.h"

/*
- InputManager
	Enum device
	Enum inputcode

	- WindowsInputManager
	- <OS>InputManager

	Interacts with: 
	- Mouse
	- Keyboard
	- Controler
	- ...

	- InputCommand
		- InputCommands
*/

namespace Pengin
{
	InputManager::InputManager() :
		m_Controller{ std::make_unique<InputController>() },
		m_Keyboard{ std::make_unique<InputKeyboard>() },

		m_MouseActionMapping(static_cast<size_t>(InputState::STATE_COUNT))

	{
		std::cout << "test\n";
	}

	bool InputManager::ProcessInput()
	{
		m_Controller->ProcessInputState();
		m_Keyboard->ProcessInputState();

		m_Controller->ProcessMappedActions();
		m_Keyboard->ProcessMappedActions();

		ProcessInputActions();

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
		m_Controller->MapControllerAction(button, inputState, std::move(pInputAction));
	}

	void InputManager::MapKeyboardAction(KeyBoardKey key, InputState inputState, std::unique_ptr<InputCommand> pInputAction)
	{
		m_Keyboard->MapKeyboardAction(key, inputState, std::move(pInputAction));
	}

	void InputManager::MapMouseAction(MouseButton button, InputState inputState, std::unique_ptr<InputCommand> pInputAction)
	{
		m_MouseActionMapping[static_cast<size_t>(inputState)][button] = std::move(pInputAction);
	}

	void InputManager::ProcessInputActions()
	{
		for (size_t i{0}; i < static_cast<size_t>(InputState::STATE_COUNT); ++i)
		{
			for (auto& pair : m_MouseActionMapping[i]) {
				if (stateFunctions[i](Devices::Mouse, static_cast<unsigned>(pair.first)))
				{
					pair.second->Execute();
				}
			}
		}
	}

	bool InputManager::IsDownThisFrame(Devices device, unsigned btn) const
	{
		switch (device)
		{
			btn;
			case Pengin::InputManager::Devices::Mouse: return false;

			default: return false;
		}
	}

	bool InputManager::IsUpThisFrame(Devices device, unsigned btn) const
	{
		switch (device)
		{
			btn;
			case Pengin::InputManager::Devices::Mouse: return false;

			default: return false;
		}
	}

	bool InputManager::IsPressed(Devices device, unsigned btn) const
	{
		switch (device)
		{
			btn;
			case Pengin::InputManager::Devices::Mouse: return false;

			default: return false;
		}
	}
}


