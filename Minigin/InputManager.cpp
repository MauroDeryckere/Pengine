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
		m_KeyboardActionMapping(static_cast<size_t>(InputState::STATE_COUNT)),
		m_MouseActionMapping(static_cast<size_t>(InputState::STATE_COUNT)),
		m_ControllerActionMapping(static_cast<size_t>(InputState::STATE_COUNT)),

		m_CurrentState{},
		m_ButtonsPressedThisFrame{},
		m_ButtonsReleasedThisFrame{}

	{
		std::cout << "test\n";
	}

	bool InputManager::ProcessInput()
	{
		DWORD userIdx{ };

		XINPUT_STATE previousState{};

		CopyMemory(&previousState, &m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		XInputGetState(userIdx, &m_CurrentState);

		//if (result == ERROR_SUCCESS)
		//{
			//Controller connectted
		//}

		auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
		m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);

		//Keybaord
		BYTE previousKeyboardState[256];
		memcpy(previousKeyboardState, m_CurrentKBState, sizeof(previousKeyboardState));

		GetKeyboardState(m_CurrentKBState);

		// Determine which keys were pressed and released
		for (int i = 0; i < 256; ++i) {
			m_KBButtonsPressedThisFrame[i] = (m_CurrentKBState[i] & ~previousKeyboardState[i]) & 0x80;
			m_KBButtonsReleasedThisFrame[i] = (~m_CurrentKBState[i] & previousKeyboardState[i]) & 0x80;
		}

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
		m_ControllerActionMapping[static_cast<size_t>(inputState)][button] = std::move(pInputAction);
	}

	void InputManager::MapKeyboardAction(KeyBoardKey key, InputState inputState, std::unique_ptr<InputCommand> pInputAction)
	{
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

			for (auto& pair : m_ControllerActionMapping[i]) {
				if (stateFunctions[i](Devices::Controller, static_cast<unsigned>(pair.first)))
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
			case Pengin::InputManager::Devices::Keyboard: return false;
			case Pengin::InputManager::Devices::Mouse: return false;
			case Pengin::InputManager::Devices::Controller: return m_ButtonsPressedThisFrame & btn;

			default: return false;
		}
	}

	bool InputManager::IsUpThisFrame(Devices device, unsigned btn) const
	{
		switch (device)
		{
			case Pengin::InputManager::Devices::Keyboard: return false;
			case Pengin::InputManager::Devices::Mouse: return false;
			case Pengin::InputManager::Devices::Controller: return m_ButtonsPressedThisFrame & btn;

			default: return false;
		}
	}

	bool InputManager::IsPressed(Devices device, unsigned btn) const
	{
		switch (device)
		{
			case Pengin::InputManager::Devices::Keyboard: return m_CurrentKBState[static_cast<unsigned>(KeyBoardKey::A)] & 0x80;
			case Pengin::InputManager::Devices::Mouse: return false;
			case Pengin::InputManager::Devices::Controller: return m_CurrentState.Gamepad.wButtons & btn;

			default: return false;
		}
	}
}


