#include <SDL.h>
#include "InputManager.h"

#include <backends/imgui_impl_sdl2.h>

#include <iostream>

#include "Windows.h"
#include "XInput.h"

namespace Pengin
{
	bool InputManager::ProcessInput() //Handles components TODO
	{
		DWORD userIdx{ };

		XINPUT_STATE previousState{};

		CopyMemory(&previousState, &m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		XInputGetState(userIdx, &m_CurrentState);

		auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
		m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);

		for (auto& command : m_InputActionMapping)
		{
			if (IsPressed(command.first))
			{
				command.second->Execute();
			}
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

	bool InputManager::IsDownThisFrame(InputCode button) const
	{
		return m_ButtonsPressedThisFrame & static_cast<unsigned>(m_InputCodeToConsoleButtonMapping[static_cast<size_t>(button)]);
	}

	bool InputManager::IsUpThisFrame(InputCode button) const
	{
		return m_ButtonsReleasedThisFrame & static_cast<unsigned>(m_InputCodeToConsoleButtonMapping[static_cast<size_t>(button)]);
	}

	bool InputManager::IsPressed(InputCode button) const
	{
		return m_CurrentState.Gamepad.wButtons & static_cast<unsigned>(m_InputCodeToConsoleButtonMapping[static_cast<size_t>(button)]);
	}
	void InputManager::RegisterActionMapping(InputCode button, std::unique_ptr<InputCommand> pAction)
	{
		m_InputActionMapping[button] = std::move(pAction);
	}
}


