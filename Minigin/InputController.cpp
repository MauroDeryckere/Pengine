#include "InputController.h"

#include "InputManager.h"
#include "InputBuffer.h"

#include "Windows.h"
#include "Xinput.h"

#include <unordered_map>
#include <vector>

namespace Pengin
{
	class WindowsInputControllerImpl
	{
	public:
		WindowsInputControllerImpl();
		~WindowsInputControllerImpl() = default;

		void ProcessInputState();
		void ProcessMappedActions();
		void MapActionToInput(unsigned key, InputState inputState, std::shared_ptr<InputCommand> pInputAction);

		//Pivate in InputController
		unsigned GetCodeFromKey(unsigned key) const;
		bool IsDownThisFrame(unsigned btn) const;
		bool IsUpThisFrame(unsigned btn) const;
		bool IsPressed(unsigned btn) const;
	private:
		XINPUT_STATE m_CurrentState;
		unsigned m_ButtonsPressedThisFrame;
		unsigned m_ButtonsReleasedThisFrame;
		std::vector<std::unordered_map<ControllerButton, std::shared_ptr<InputCommand>>> m_ControllerActionMapping;

	};

	WindowsInputControllerImpl::WindowsInputControllerImpl():
		m_CurrentState{},

		m_ButtonsPressedThisFrame{},
		m_ButtonsReleasedThisFrame{},

		m_ControllerActionMapping(static_cast<size_t>(InputState::STATE_COUNT)) {}

	void WindowsInputControllerImpl::ProcessInputState()
	{
		DWORD userIdx{ };

		XINPUT_STATE previousState{};

		CopyMemory(&previousState, &m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		XInputGetState(userIdx, &m_CurrentState);

		//if (result == ERROR_SUCCESS)
		//{
			//Controller connected
		//}

		auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
		m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
	}

	void WindowsInputControllerImpl::ProcessMappedActions()
	{
		for (auto& pair : m_ControllerActionMapping[static_cast<size_t>(InputState::DownThisFrame)]) {
			if (IsDownThisFrame(GetCodeFromKey(static_cast<unsigned>(pair.first))))
			{
				pair.second->Execute();
				InputBuffer::GetInstance().RecordInput(pair.second);
			}
		}
		for (auto& pair : m_ControllerActionMapping[static_cast<size_t>(InputState::UpThisFrame)]) {
			if (IsUpThisFrame(GetCodeFromKey(static_cast<unsigned>(pair.first))))
			{
				pair.second->Execute();
				InputBuffer::GetInstance().RecordInput(pair.second);
			}
		}
		for (auto& pair : m_ControllerActionMapping[static_cast<size_t>(InputState::Pressed)]) {
			if (IsPressed(GetCodeFromKey(static_cast<unsigned>(pair.first))))
			{
				pair.second->Execute();
				InputBuffer::GetInstance().RecordInput(pair.second);
			}
		}
	}

	void WindowsInputControllerImpl::MapActionToInput(unsigned key, InputState inputState, std::shared_ptr<InputCommand> pInputAction)
	{
		m_ControllerActionMapping[static_cast<size_t>(inputState)][static_cast<ControllerButton>(key)] = std::move(pInputAction);
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

	InputController::InputController() :
		InputDevice{},
		m_WinImpl(std::make_unique<WindowsInputControllerImpl>())
	{

	}

	void InputController::ProcessInputState()
	{
		m_WinImpl->ProcessInputState();
	}

	void InputController::ProcessMappedActions()
	{
		m_WinImpl->ProcessMappedActions();
	}

	void InputController::MapActionToInput(unsigned key, InputState inputState, std::shared_ptr<InputCommand> pInputAction)
	{
		m_WinImpl->MapActionToInput(key, inputState, pInputAction);
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