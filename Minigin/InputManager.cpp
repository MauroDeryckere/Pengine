#include "InputManager.h"

#include <SDL.h>

#include <iostream>
#include <algorithm>

#include "InputDevice.h"
#include "InputController.h"
#include "InputKeyboard.h"

#include "InputBuffer.h"

#include "Renderer.h" //Imgui

namespace Pengin
{
	bool InputManager::ProcessInput() noexcept
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				return false;
			}

			dae::Renderer::GetInstance().GetImGUIWindow().ProcessEvent(e);
		}

		const bool usedMouse = dae::Renderer::GetInstance().GetImGUIWindow().UsedMouse();
		const bool usedKeyboard = dae::Renderer::GetInstance().GetImGUIWindow().UsedKeyboard();

		if (usedMouse && usedKeyboard)
		{
			return true;
		}

		for (size_t userIdx{ 0 }; userIdx < m_InputBuffers.size(); ++userIdx)
		{
			for (auto idx = -1; auto& device : m_RegisteredUsers[userIdx].second)
			{
				++idx;

				if (m_RegisteredUsers[userIdx].first == UserType::Keyboard) 
				{
					if (static_cast<KeyboardDevices>(idx) == KeyboardDevices::Mouse && usedMouse)
					{
						continue;
					}
					else if (static_cast<KeyboardDevices>(idx) == KeyboardDevices::Keyboard && usedKeyboard)
					{
						continue;
					}
				}

				device->ProcessInputState();
				device->ProcessMappedActions(m_InputBuffers[userIdx].get());
			}
		}

		for (size_t userIdx{ 0 }; userIdx < m_InputBuffers.size(); ++userIdx)
		{
			auto& inputBuffer{ m_InputBuffers[userIdx] };

			for (const auto& combo : m_NewInputCombos[userIdx])
			{
				if (inputBuffer->CheckCombo(combo))
					//This takes the first possible combo, in future it might be necessary to handle all found combos and let a state machine decides which one(s) are valid and should be recorded to the buffe
				{
					inputBuffer->ClearBuffer();

					combo.pResultingAction->Execute();
					inputBuffer->RecordInput(combo.pResultingAction);
				}
			}
		}

		return true;
	}

	size_t InputManager::RegisterUser(UserType usertype) noexcept
	{
		const size_t userIdx{ m_RegisteredUsers.size() };

		m_RegisteredUsers.resize(userIdx + 1);
		m_RegisteredUsers.back().first = usertype;

		m_NewInputCombos.resize(userIdx + 1);
		m_InputBuffers.emplace_back( new InputBuffer{} );

		switch (usertype)
		{
		case Pengin::UserType::Keyboard:
			m_RegisteredUsers.back().second.emplace_back(new InputKeyboard{});
			//Mouse too in future
			break;
		case Pengin::UserType::Controller:
			m_RegisteredUsers.back().second.emplace_back(new InputController{ userIdx });
			break;

		default:
			break;
		}

		std::cout << "registered user: " << userIdx << "\n";

		return userIdx;
	}

	void InputManager::MapControllerAction(size_t userIdx, ControllerButton button, InputState inputState, std::shared_ptr<InputCommand> pInputAction) noexcept
	{
		assert(m_RegisteredUsers.size() >= userIdx && "Engine error: idx out of bounds!");
		assert(m_RegisteredUsers[userIdx].first == UserType::Controller && "User error: registered user does not have Usertype: controller");

		assert(m_RegisteredUsers[userIdx].second.size() == 1 && "Engine error : only one input device / controller user allowed.");

		assert(std::count_if(m_RegisteredUsers.begin(), m_RegisteredUsers.end(), [](const auto& user) { return user.first == UserType::Controller; }) <= MAX_ALLOWED_CONTROLLERS && "Exceeded max allowed controllers");

		//only has a controller right now
		m_RegisteredUsers[userIdx].second[0]->MapActionToInput(static_cast<unsigned>(button), inputState, std::move(pInputAction));
	}

	void InputManager::MapKeyboardAction(size_t userIdx, KeyBoardKey key, InputState inputState, std::shared_ptr<InputCommand> pInputAction) noexcept
	{
		assert(m_RegisteredUsers.size() >= userIdx && "Engine error: idx out of bounds!");
		assert(m_RegisteredUsers[userIdx].first == UserType::Keyboard && "User error: registered user does not have Usertype: keyboard");

		//only keyboard for now - Mouse :TODO
		m_RegisteredUsers[userIdx].second[0]->MapActionToInput(static_cast<unsigned>(key), inputState, std::move(pInputAction));
	}


	void InputManager::MapCombo(size_t userIdx, const InputCombo& combo) noexcept
	{
		assert(combo.pComboActions.size() > 1 && "User error: Need more than one combo action");
		assert(combo.pComboActions.size() >= combo.allowedDelay.size() && "User errror: Can not have more delays than actions");

		m_NewInputCombos[userIdx].emplace_back(combo);
	}
}


