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

		m_KeyboardActionMapping(static_cast<size_t>(InputState::STATE_COUNT)),
		m_MouseActionMapping(static_cast<size_t>(InputState::STATE_COUNT))

	{
		std::cout << "test\n";
	}

	bool InputManager::ProcessInput()
	{
		//m_Keyboard->GetState();

		m_Controller->ProcessInputState();

		BYTE previousKeyboardState[256];
		memcpy(previousKeyboardState, m_CurrentKBState, sizeof(previousKeyboardState));

		GetKeyboardState(m_CurrentKBState);

		// Determine which keys were pressed and released
		for (int i = 0; i < 256; ++i) {
			m_KBButtonsPressedThisFrame[i] = (m_CurrentKBState[i] & ~previousKeyboardState[i]) & 0x80;
			m_KBButtonsReleasedThisFrame[i] = (~m_CurrentKBState[i] & previousKeyboardState[i]) & 0x80;
		}


		m_Controller->ProcessMappedActions();

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
		//m_Keyboard->MapAction
		m_KeyboardActionMapping[static_cast<size_t>(inputState)][key] = std::move(pInputAction);
	}

	void InputManager::MapMouseAction(MouseButton button, InputState inputState, std::unique_ptr<InputCommand> pInputAction)
	{
		m_MouseActionMapping[static_cast<size_t>(inputState)][button] = std::move(pInputAction);
	}

	void InputManager::ProcessInputActions()
	{
		for (size_t i{0}; i < static_cast<size_t>(InputState::STATE_COUNT); ++i)
		{
			for (auto& pair : m_KeyboardActionMapping[i]) {
				
				if (stateFunctions[i](Devices::Keyboard, static_cast<unsigned>(pair.first)))
				{
					pair.second->Execute();
				}
			}

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
			case Pengin::InputManager::Devices::Keyboard: return false;
			case Pengin::InputManager::Devices::Mouse: return false;

			default: return false;
		}
	}

	bool InputManager::IsUpThisFrame(Devices device, unsigned btn) const
	{
		switch (device)
		{
			btn;
			case Pengin::InputManager::Devices::Keyboard: return false;
			case Pengin::InputManager::Devices::Mouse: return false;

			default: return false;
		}
	}

	bool InputManager::IsPressed(Devices device, unsigned btn) const
	{
		switch (device)
		{
			btn;
			case Pengin::InputManager::Devices::Keyboard: return m_CurrentKBState[static_cast<unsigned>(KeyBoardKey::A)] & 0x80;
			case Pengin::InputManager::Devices::Mouse: return false;

			default: return false;
		}
	}
}


