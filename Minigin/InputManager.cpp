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

		for (auto& event : m_InputActionMapping)
		{
			if ((event.first.type == InputCommandType::Pressed && IsPressed(event.first.code)) ||
				(event.first.type == InputCommandType::UpThisFrame && IsUpThisFrame(event.first.code)) || 
				(event.first.type == InputCommandType::DownThisFrame && IsDownThisFrame(event.first.code)))
			{
				event.second->Execute();
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
	void InputManager::RegisterActionMapping(InputCode button, InputCommandType type, std::unique_ptr<InputCommand> pAction)
	{
		
		m_InputActionMapping[InputCommandInfo{button, type}] = std::move(pAction);
	}
}


