#include "InputManager.h"

#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>

#include <iostream>
#include <algorithm>

#include "InputDevice.h"
#include "InputController.h"
#include "InputKeyboard.h"

#include "InputBuffer.h"

namespace Pengin
{
	InputManager::InputManager() :
		m_InputDevices(static_cast<size_t>(Devices::DEVICE_COUNT))
	{
		m_InputDevices[static_cast<size_t>(Devices::Keyboard)].reset(new InputKeyboard{});
		m_InputDevices[static_cast<size_t>(Devices::Controller)].reset(new InputController{});
	}

	bool InputManager::ProcessInput() noexcept
	{
		for (auto& device : m_InputDevices)
		{
			device->ProcessInputState();
			device->ProcessMappedActions();
		}

		auto& inputBuffer{ InputBuffer::GetInstance() };

		for (const auto& combo : m_Combos)
		{
			if (inputBuffer.CheckCombo(combo))
				//This takes the first possible combo, in future it might be necessary to handle all found combos and let a state machine decides which one(s) are valid and should be recorded to the buffe
			{
				inputBuffer.ClearBuffer();

				combo.pResultingAction->Execute();
				inputBuffer.RecordInput(combo.pResultingAction);
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

	void InputManager::MapControllerAction(ControllerButton button, InputState inputState, std::shared_ptr<InputCommand> pInputAction) noexcept
	{
		m_InputDevices[static_cast<size_t>(Devices::Controller)]->MapActionToInput(static_cast<unsigned>(button), inputState, std::move(pInputAction));
	}

	void InputManager::MapKeyboardAction(KeyBoardKey key, InputState inputState, std::shared_ptr<InputCommand> pInputAction) noexcept
	{
		m_InputDevices[static_cast<size_t>(Devices::Keyboard)]->MapActionToInput(static_cast<unsigned>(key), inputState, std::move(pInputAction));
	}

	void InputManager::MapCombo(const InputCombo& combo) noexcept
	{
		assert(combo.pComboActions.size() > 1);
		assert(combo.pComboActions.size() >= combo.allowedDelay.size());

		m_Combos.emplace_back(combo);
	}
}


