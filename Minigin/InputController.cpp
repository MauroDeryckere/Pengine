#include "InputController.h"

#include "InputManager.h"

namespace Pengin
{
	InputController::InputController():
		m_ControllerActionMapping(static_cast<size_t>(InputState::STATE_COUNT))
	{
	}
	void InputController::ProcessInputState()
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
	}

	void InputController::ProcessMappedActions()
	{
		for (auto& pair : m_ControllerActionMapping[0]) {
			if (IsUpThisFrame(GetCodeFromEnum(pair.first))) pair.second->Execute();
		}
		for (auto& pair : m_ControllerActionMapping[1]) {
			if (IsDownThisFrame(GetCodeFromEnum(pair.first))) pair.second->Execute();
		}
		for (auto& pair : m_ControllerActionMapping[2]) {
			if (IsPressed(GetCodeFromEnum(pair.first))) pair.second->Execute();
		}
	}

	void InputController::MapControllerAction(ControllerButton button, InputState inputState, std::unique_ptr<InputCommand> pInputAction)
	{
		m_ControllerActionMapping[static_cast<size_t>(inputState)][button] = std::move(pInputAction);
	}

	unsigned InputController::GetCodeFromEnum(ControllerButton button) const
	{
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
	bool InputController::IsDownThisFrame(unsigned btn) const
	{
		return m_ButtonsPressedThisFrame & btn;
	}
	bool InputController::IsUpThisFrame(unsigned btn) const
	{
		return m_ButtonsReleasedThisFrame & btn;
	}
	bool InputController::IsPressed(unsigned btn) const
	{
		return m_CurrentState.Gamepad.wButtons & btn;
	}
}

