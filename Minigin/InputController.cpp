#include "InputController.h"

#include "GameTime.h"

#include "InputManager.h"
#include "InputBuffer.h"

#include "Windows.h"
#include "Xinput.h"

#include <unordered_map>
#include <ranges>
#include <vector>

namespace Pengin
{
	class WindowsInputControllerImpl
	{
	public:
		WindowsInputControllerImpl(size_t userIdx);
		~WindowsInputControllerImpl() = default;

		void ProcessInputState();
		void ProcessMappedActions(InputBuffer* const inputBuffer);
		void MapActionToInput(unsigned key, InputState inputState, std::shared_ptr<InputCommand> pInputAction);

		const std::vector<std::unordered_map<unsigned, std::shared_ptr<InputCommand>>>& GetMappedActions();

		void Clear() noexcept;

		//Pivate in InputController
		unsigned GetCodeFromKey(unsigned key) const;
		bool IsDownThisFrame(unsigned btn) const;
		bool IsUpThisFrame(unsigned btn) const;
		bool IsPressed(unsigned btn) const;

	private:
		void UpdateControllerIdx();

		std::pair<size_t, DWORD> m_UserIdxControllerIdxPair;

		XINPUT_STATE m_CurrentState;
		unsigned m_ButtonsPressedThisFrame;
		unsigned m_ButtonsReleasedThisFrame;
		std::vector<std::unordered_map<unsigned, std::shared_ptr<InputCommand>>> m_ControllerActionMapping;

		static constexpr DWORD INVALID_CONTROLLER_VALUE { 100 } ;
		
		static std::vector<DWORD> m_FreeControllerIdxes;

		float m_DisconnectedTime{ 0.f };
		static constexpr float MAX_ALLOWED_DISCONNECTED_TIME{ 10.f };
	};
	std::vector<DWORD> WindowsInputControllerImpl::m_FreeControllerIdxes = { 0, 1, 2, 3 }; //Windows defined available controller idxes

	WindowsInputControllerImpl::WindowsInputControllerImpl(size_t userIdx):
		m_UserIdxControllerIdxPair{userIdx, INVALID_CONTROLLER_VALUE },
		m_CurrentState{},

		m_ButtonsPressedThisFrame{},
		m_ButtonsReleasedThisFrame{},

		m_ControllerActionMapping(static_cast<size_t>(InputState::STATE_COUNT)) 
	
	{
		UpdateControllerIdx();
	}

	void WindowsInputControllerImpl::ProcessInputState()
	{
		if (m_UserIdxControllerIdxPair.second == INVALID_CONTROLLER_VALUE)
		{
			UpdateControllerIdx();
			return;
		}

		XINPUT_STATE previousState{};

		CopyMemory(&previousState, &m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		auto result = XInputGetState(m_UserIdxControllerIdxPair.second, &m_CurrentState);

		if (result == ERROR_SUCCESS)
		{
			auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
			m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
			m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
		}
		else
		{
			m_FreeControllerIdxes.emplace_back(m_UserIdxControllerIdxPair.second);
			m_UserIdxControllerIdxPair.second = INVALID_CONTROLLER_VALUE;

			UpdateControllerIdx();
		}
	}

	void WindowsInputControllerImpl::ProcessMappedActions(InputBuffer * const inputBuffer)
	{
		for (auto& pair : m_ControllerActionMapping[static_cast<size_t>(InputState::DownThisFrame)]) {
			if (IsDownThisFrame(GetCodeFromKey(pair.first)))
			{
				pair.second->Execute();
				inputBuffer->RecordInput(pair.second);
			}
		}
		for (auto& pair : m_ControllerActionMapping[static_cast<size_t>(InputState::UpThisFrame)]) {
			if (IsUpThisFrame(GetCodeFromKey(pair.first)))
			{
				pair.second->Execute();
				inputBuffer->RecordInput(pair.second);
			}
		}
		for (auto& pair : m_ControllerActionMapping[static_cast<size_t>(InputState::Pressed)]) {
			if (IsPressed(GetCodeFromKey(pair.first)))
			{
				pair.second->Execute();
				inputBuffer->RecordInput(pair.second);
			}
		}
	}

	void WindowsInputControllerImpl::MapActionToInput(unsigned key, InputState inputState, std::shared_ptr<InputCommand> pInputAction)
	{
		m_ControllerActionMapping[static_cast<size_t>(inputState)][key] = std::move(pInputAction);
	}

	const std::vector<std::unordered_map<unsigned, std::shared_ptr<InputCommand>>>& WindowsInputControllerImpl::GetMappedActions()
	{
		return m_ControllerActionMapping;
	}

	void WindowsInputControllerImpl::Clear() noexcept
	{
		m_ControllerActionMapping.clear();
		m_ControllerActionMapping.resize(static_cast<size_t>(InputState::STATE_COUNT));
	}

	unsigned WindowsInputControllerImpl::GetCodeFromKey(unsigned key) const
	{
		ControllerButton button{ static_cast<ControllerButton>(key) };

		switch (button)
		{
			case ControllerButton::DPadUp: return 0x0001;
			case ControllerButton::DPadDown: return 0x0002;
			case ControllerButton::DPadLeft: return 0x0004;
			case ControllerButton::DPadRight: return 0x0008;

			case ControllerButton::Start: return 0x0010;
			case ControllerButton::Back: return 0x0020;

			case ControllerButton::LeftThumb: return 0x0040;
			case ControllerButton::RightThumb: return 0x0080;
			case ControllerButton::LeftShoulder: return 0x0100;
			case ControllerButton::RightShoulder: return 0x0200;
			
			case ControllerButton::A: return 0x1000;
			case ControllerButton::B: return 0x2000;
			case ControllerButton::X: return 0x4000;
			case ControllerButton::Y: return 0x8000;

			default: return 0x0000;
		}
	}
	bool WindowsInputControllerImpl::IsDownThisFrame(unsigned btn) const
	{
		return m_ButtonsPressedThisFrame & btn;
	}
	bool WindowsInputControllerImpl::IsUpThisFrame(unsigned btn) const
	{
		return m_ButtonsReleasedThisFrame & btn;
	}
	bool WindowsInputControllerImpl::IsPressed(unsigned btn) const
	{
		return m_CurrentState.Gamepad.wButtons & btn;
	}

	void WindowsInputControllerImpl::UpdateControllerIdx()
	{
		static bool displayMessage = false;

		for (auto& controllerIdx : m_FreeControllerIdxes)
		{
			XINPUT_STATE state;
			DWORD result = XInputGetState(controllerIdx, &state);

			if (result == ERROR_SUCCESS)
			{
				m_UserIdxControllerIdxPair.second = controllerIdx;
				std::erase_if(m_FreeControllerIdxes, [&](DWORD idx) { return idx == controllerIdx; });

				m_DisconnectedTime = 0.f;
				displayMessage = false;
				return;
			}
		}
		
		m_DisconnectedTime += GameTime::GetInstance().GetElapsedSec();

		if (m_DisconnectedTime >= MAX_ALLOWED_DISCONNECTED_TIME && !displayMessage)
		{
			std::cerr << "MAX ALLOWED CONTROLLER DISCONNECT TIME REACHED FOR USERIDX: " << m_UserIdxControllerIdxPair.first << "\n";
			displayMessage = true;
		}
	}

	InputController::InputController(size_t userIdx) :
		InputDevice{},
		m_WinImpl(std::make_unique<WindowsInputControllerImpl>(userIdx))
	{ }

	void InputController::ProcessInputState()
	{
		m_WinImpl->ProcessInputState();
	}

	void InputController::ProcessMappedActions(InputBuffer* const inputBuffer)
	{
		m_WinImpl->ProcessMappedActions(inputBuffer);
	}

	void InputController::MapActionToInput(unsigned key, InputState inputState, std::shared_ptr<InputCommand> pInputAction)
	{
		m_WinImpl->MapActionToInput(key, inputState, pInputAction);
	}

	const std::vector<std::unordered_map<unsigned, std::shared_ptr<InputCommand>>>& InputController::GetMappedActions()
	{
		return m_WinImpl->GetMappedActions();
	}

	void InputController::Clear() noexcept
	{
		m_WinImpl->Clear();
	}

	unsigned InputController::GetCodeFromKey(unsigned key) const
	{
		return m_WinImpl->GetCodeFromKey(key);
	}

	bool InputController::IsDownThisFrame(unsigned btn) const
	{
		return m_WinImpl->IsDownThisFrame(btn);
	}

	bool InputController::IsUpThisFrame(unsigned btn) const
	{
		return m_WinImpl->IsUpThisFrame(btn);
	}

	bool InputController::IsPressed(unsigned btn) const
	{
		return m_WinImpl->IsPressed(btn);
	}

}