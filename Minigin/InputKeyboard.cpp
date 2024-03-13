#include "InputKeyboard.h"
#include "InputManager.h"

namespace  Pengin
{
	InputKeyboard::InputKeyboard() :
		m_KeyboardActionMapping(static_cast<size_t>(InputState::STATE_COUNT))
	{

	}

	void InputKeyboard::ProcessInputState()
	{
		BYTE previousKeyboardState[256];
		memcpy(previousKeyboardState, m_CurrentKBState, sizeof(previousKeyboardState));

		GetKeyboardState(m_CurrentKBState);

		for (int i = 0; i < 256; ++i)
		{
			m_KBButtonsPressedThisFrame[i] = (m_CurrentKBState[i] & ~previousKeyboardState[i]) & 0x80;
			m_KBButtonsReleasedThisFrame[i] = (~m_CurrentKBState[i] & previousKeyboardState[i]) & 0x80;
		}

	}

	void InputKeyboard::ProcessMappedActions()
	{
		for (auto& pair : m_KeyboardActionMapping[0]) {
			if (IsUpThisFrame(GetCodeFromEnum(pair.first))) pair.second->Execute();
		}
		for (auto& pair : m_KeyboardActionMapping[1]) {
			if (IsDownThisFrame(GetCodeFromEnum(pair.first))) pair.second->Execute();
		}
		for (auto& pair : m_KeyboardActionMapping[2]) {
			if (IsPressed(GetCodeFromEnum(pair.first))) pair.second->Execute();
		}
	}

	void InputKeyboard::MapKeyboardAction(KeyBoardKey key, InputState inputState, std::unique_ptr<InputCommand> pInputAction)
	{
		m_KeyboardActionMapping[static_cast<size_t>(inputState)][key] = std::move(pInputAction);

	}

	unsigned InputKeyboard::GetCodeFromEnum(KeyBoardKey key) const
	{
		switch (key)
		{
			case KeyBoardKey::A: return 0x41;
			default: return 0;
		}
	}

	bool InputKeyboard::IsDownThisFrame(unsigned btn) const
	{
		btn;
		return false;
	}

	bool InputKeyboard::IsUpThisFrame(unsigned btn) const
	{
		btn;
		return false;
	}

	bool InputKeyboard::IsPressed(unsigned btn) const
	{
		return m_CurrentKBState[btn] & 0x80;
	}
}


