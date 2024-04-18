#include "InputManager.h"

#include <SDL.h>

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
			if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(dae::Renderer::GetInstance().GetSDLWindow()))
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

			for (const auto& combo : m_InputCombos[userIdx])
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

	const UserIndex InputManager::RegisterUser(UserType usertype) noexcept
	{
		const size_t userVecIdx{ m_RegisteredUsers.size() };

		m_RegisteredUsers.resize(userVecIdx + 1);
		m_RegisteredUsers.back().first = usertype;

		m_InputCombos.resize(userVecIdx + 1);
		m_InputBuffers.emplace_back( new InputBuffer{} );

		UserIndex uIdx{};
		m_UserIdx_VecIdxMap[uIdx] = userVecIdx;

		switch (usertype)
		{
		case Pengin::UserType::Keyboard:
			m_RegisteredUsers.back().second.emplace_back(new InputKeyboard{});
			//Mouse too in future
			break;
		case Pengin::UserType::Controller:
			m_RegisteredUsers.back().second.emplace_back(new InputController{ userVecIdx });
			break;

		default:
			break;
		}

		DEBUG_OUT("registered user UservecIdx: " << userVecIdx << " user uuid: " << uIdx.GetUUID_PrettyStr());

		return uIdx;
	}

	void InputManager::RegisterUser(const UserIndex& index, UserType usertype) noexcept
	{	
		const auto it = m_UserIdx_VecIdxMap.find(index);

		if (it != end(m_UserIdx_VecIdxMap))
		{
			DEBUG_OUT("User already exists " << index.GetUUID_PrettyStr());
			return;
		}

		const size_t userVecIdx{ m_RegisteredUsers.size() };

		m_RegisteredUsers.resize(userVecIdx + 1);
		m_RegisteredUsers.back().first = usertype;

		m_InputCombos.resize(userVecIdx + 1);
		m_InputBuffers.emplace_back(new InputBuffer{});

		m_UserIdx_VecIdxMap[index] = userVecIdx;

		switch (usertype)
		{
		case Pengin::UserType::Keyboard:
			m_RegisteredUsers.back().second.emplace_back(new InputKeyboard{});
			//Mouse too in future
			break;
		case Pengin::UserType::Controller:
			m_RegisteredUsers.back().second.emplace_back(new InputController{ userVecIdx });
			break;

		default:
			break;
		}

		DEBUG_OUT("registered user UservecIdx: " << userVecIdx << " user uuid: " << index.GetUUID_PrettyStr());
	}

	void InputManager::MapControllerAction(const UserIndex& userIdx, ControllerButton button, InputState inputState, std::shared_ptr<InputCommand> pInputAction) noexcept
	{
		auto it = m_UserIdx_VecIdxMap.find(userIdx);
		assert(it != end(m_UserIdx_VecIdxMap) && "user not added");

		assert(m_RegisteredUsers.size() >= it->second && "Engine error: idx out of bounds!");
		assert(m_RegisteredUsers[it->second].first == UserType::Controller && "User error: registered user does not have Usertype: controller");

		assert(m_RegisteredUsers[it->second].second.size() == 1 && "Engine error : only one input device / controller user allowed.");

		assert(std::count_if(m_RegisteredUsers.begin(), m_RegisteredUsers.end(), [](const auto& user) { return user.first == UserType::Controller; }) <= MAX_ALLOWED_CONTROLLERS && "Exceeded max allowed controllers");

		//only has a controller right now
		m_RegisteredUsers[it->second].second[0]->MapActionToInput(static_cast<unsigned>(button), inputState, std::move(pInputAction));
	}

	void InputManager::MapKeyboardAction(const UserIndex& userIdx, KeyBoardKey key, InputState inputState, std::shared_ptr<InputCommand> pInputAction) noexcept
	{
		auto it = m_UserIdx_VecIdxMap.find(userIdx);

		assert(it != end(m_UserIdx_VecIdxMap) && "user not added");

		assert(m_RegisteredUsers.size() >= it->second && "Engine error: idx out of bounds!");
		assert(m_RegisteredUsers[it->second].first == UserType::Keyboard && "User error: registered user does not have Usertype: keyboard");

		//only keyboard for now 
		m_RegisteredUsers[it->second].second[0]->MapActionToInput(static_cast<unsigned>(key), inputState, std::move(pInputAction));
	}

	void InputManager::Clear() noexcept
	{
		m_RegisteredUsers.clear();
		m_InputCombos.clear();
		m_InputBuffers.clear();
		m_UserIdx_VecIdxMap.clear();
	}

	void InputManager::Reset() noexcept
	{
		for (auto& user : m_RegisteredUsers)
		{
			for (auto& device : user.second)
			{
				device->Clear();
			}
		}

		for (auto& inputCombo : m_InputCombos)
		{
			inputCombo.clear();
		}

		for (auto& inputbuffer : m_InputBuffers)
		{
			inputbuffer->ClearBuffer();
		}
	}

	void InputManager::MapCombo(const UserIndex& userIdx, const InputCombo& combo) noexcept
	{
		auto it = m_UserIdx_VecIdxMap.find(userIdx);

		assert(it != end(m_UserIdx_VecIdxMap) && "user not added");

		assert(combo.pComboActions.size() > 1 && "User error: Need more than one combo action");
		assert(combo.pComboActions.size() >= combo.allowedDelay.size() && "User errror: Can not have more delays than actions");

		m_InputCombos[it->second].emplace_back(combo);
	}
}


