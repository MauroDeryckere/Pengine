#include "InputKeyboard.h"
#include "InputManager.h"
#include "InputBuffer.h"
#include "Windows.h"

#include <vector>
#include <unordered_map>

namespace Pengin
{
	class InputKeyboard::WindowsKeyboardImpl
	{
	public:
		WindowsKeyboardImpl();
		~WindowsKeyboardImpl() = default;

		void ProcessInputState();
		void ProcessMappedActions(InputBuffer* const inputBuffer, ExecActions& execActions);
		void MapActionToInput(unsigned key, InputState inputState, std::shared_ptr<InputCommand> pInputAction);
		void UnMapInputAction(unsigned key, InputState inputState);

		const std::vector<std::unordered_map<unsigned, std::shared_ptr<InputCommand>>>& GetMappedActions();

		void Clear() noexcept;

		//Pivate in InputKeyboard
		unsigned GetCodeFromKey(unsigned key) const;
		bool IsDownThisFrame(unsigned btn) const;
		bool IsUpThisFrame(unsigned btn) const;
		bool IsPressed(unsigned btn) const;

	private:
		std::vector<std::unordered_map<unsigned, std::shared_ptr<InputCommand>>> m_KeyboardActionMapping;

		BYTE m_CurrentKBState[256];
		BYTE m_KBButtonsPressedThisFrame[256];
		BYTE m_KBButtonsReleasedThisFrame[256];

	};

	InputKeyboard::WindowsKeyboardImpl::WindowsKeyboardImpl() :
		m_KeyboardActionMapping(static_cast<size_t>(InputState::STATE_COUNT))	
	{ }

	void InputKeyboard::WindowsKeyboardImpl::ProcessInputState()
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

	void InputKeyboard::WindowsKeyboardImpl::ProcessMappedActions(InputBuffer* const inputBuffer, ExecActions& execActions)
	{
		for (auto& pair : m_KeyboardActionMapping[static_cast<size_t>(InputState::DownThisFrame)]) {
			if (IsDownThisFrame(GetCodeFromKey(pair.first)))
			{
				pair.second->Execute();

				execActions[pair.second->GetActionName()] = pair.second;

				inputBuffer->RecordInput(pair.second);
			}
		}
		for (auto& pair : m_KeyboardActionMapping[static_cast<size_t>(InputState::UpThisFrame)]) {
			if (IsUpThisFrame(GetCodeFromKey(pair.first)))
			{
				pair.second->Execute();

				execActions[pair.second->GetActionName()] = pair.second;

				inputBuffer->RecordInput(pair.second);
			}			
		}
		for (auto& pair : m_KeyboardActionMapping[static_cast<size_t>(InputState::Pressed)]) {
			if (IsPressed(GetCodeFromKey(pair.first)))
			{
				pair.second->Execute();

				execActions[pair.second->GetActionName()] = pair.second;

				inputBuffer->RecordInput(pair.second);
			}
		}
	}

	void InputKeyboard::WindowsKeyboardImpl::MapActionToInput(unsigned key, InputState inputState, std::shared_ptr<InputCommand> pInputAction)
	{
		m_KeyboardActionMapping[static_cast<size_t>(inputState)][key] = std::move(pInputAction);
	}

	void InputKeyboard::WindowsKeyboardImpl::UnMapInputAction(unsigned key, InputState inputState)
	{
		auto it = m_KeyboardActionMapping[static_cast<size_t>(inputState)].find(key);

		if (it == end(m_KeyboardActionMapping[static_cast<size_t>(inputState)]))
		{
			DEBUG_OUT("trying to unmap action for a key/state that has no action bound to it: " << key);
			return;
		}

		m_KeyboardActionMapping[static_cast<size_t>(inputState)].erase(it);
	}

	const std::vector<std::unordered_map<unsigned, std::shared_ptr<InputCommand>>>& InputKeyboard::WindowsKeyboardImpl::GetMappedActions()
	{
		return m_KeyboardActionMapping;
	}

	void InputKeyboard::WindowsKeyboardImpl::Clear() noexcept
	{	
		m_KeyboardActionMapping.clear();
		m_KeyboardActionMapping.resize(static_cast<size_t>(InputState::STATE_COUNT));
	}

	unsigned InputKeyboard::WindowsKeyboardImpl::GetCodeFromKey(unsigned key) const
	{
		KeyBoardKey keyEnum{ static_cast<KeyBoardKey>(key) };
		
		switch (keyEnum)
		{
		case Pengin::KeyBoardKey::Backspace: return static_cast<unsigned>(VK_BACK);
			case Pengin::KeyBoardKey::Tab: return static_cast<unsigned>(VK_TAB);
			case Pengin::KeyBoardKey::Return: return static_cast<unsigned>(VK_RETURN);
			case Pengin::KeyBoardKey::Shift: return static_cast<unsigned>(VK_SHIFT);
			case Pengin::KeyBoardKey::LeftShift: return static_cast<unsigned>(VK_LSHIFT);
			case Pengin::KeyBoardKey::RightShift: return static_cast<unsigned>(VK_RSHIFT);
			case Pengin::KeyBoardKey::Ctrl: return static_cast<unsigned>(VK_CONTROL);
			case Pengin::KeyBoardKey::LeftCtrl: return static_cast<unsigned>(VK_LCONTROL);
			case Pengin::KeyBoardKey::RightCtrl: return static_cast<unsigned>(VK_RCONTROL);
			case Pengin::KeyBoardKey::Alt: return static_cast<unsigned>(VK_MENU);
			case Pengin::KeyBoardKey::LeftAlt: return static_cast<unsigned>(VK_LMENU);
			case Pengin::KeyBoardKey::RightAlt: return static_cast<unsigned>(VK_RMENU);
			case Pengin::KeyBoardKey::Pause: return static_cast<unsigned>(VK_PAUSE);
			case Pengin::KeyBoardKey::Capslock: return static_cast<unsigned>(VK_CAPITAL);
			case Pengin::KeyBoardKey::Escape: return static_cast<unsigned>(VK_ESCAPE);
			case Pengin::KeyBoardKey::PageUp: return static_cast<unsigned>(VK_PRIOR);
			case Pengin::KeyBoardKey::PageDown: return static_cast<unsigned>(VK_NEXT);
			case Pengin::KeyBoardKey::Home: return static_cast<unsigned>(VK_HOME);
			case Pengin::KeyBoardKey::End: return static_cast<unsigned>(VK_END);
			case Pengin::KeyBoardKey::Left: return static_cast<unsigned>(VK_LEFT);
			case Pengin::KeyBoardKey::Right: return static_cast<unsigned>(VK_RIGHT);
			case Pengin::KeyBoardKey::Up: return static_cast<unsigned>(VK_UP);
			case Pengin::KeyBoardKey::Down: return static_cast<unsigned>(VK_DOWN);
			case Pengin::KeyBoardKey::PrintScreen: return static_cast<unsigned>(VK_PRINT);
			case Pengin::KeyBoardKey::Insert: return static_cast<unsigned>(VK_INSERT);
			case Pengin::KeyBoardKey::Delete: return static_cast<unsigned>(VK_DELETE);
			case Pengin::KeyBoardKey::SpaceBar: return static_cast<unsigned>(VK_SPACE);
			case Pengin::KeyBoardKey::Key0: return static_cast<unsigned>(0x30);
			case Pengin::KeyBoardKey::Key1: return static_cast<unsigned>(0x31);
			case Pengin::KeyBoardKey::Key2: return static_cast<unsigned>(0x32);
			case Pengin::KeyBoardKey::Key3: return static_cast<unsigned>(0x33);
			case Pengin::KeyBoardKey::Key4: return static_cast<unsigned>(0x34);
			case Pengin::KeyBoardKey::Key5: return static_cast<unsigned>(0x35);
			case Pengin::KeyBoardKey::Key6: return static_cast<unsigned>(0x36);
			case Pengin::KeyBoardKey::Key7: return static_cast<unsigned>(0x37);
			case Pengin::KeyBoardKey::Key8: return static_cast<unsigned>(0x38);
			case Pengin::KeyBoardKey::Key9: return static_cast<unsigned>(0x39);
			case Pengin::KeyBoardKey::A: return static_cast<unsigned>(0x41);
			case Pengin::KeyBoardKey::B: return static_cast<unsigned>(0x42);
			case Pengin::KeyBoardKey::C: return static_cast<unsigned>(0x43);
			case Pengin::KeyBoardKey::D: return static_cast<unsigned>(0x44);
			case Pengin::KeyBoardKey::E: return static_cast<unsigned>(0x45);
			case Pengin::KeyBoardKey::F: return static_cast<unsigned>(0x46);
			case Pengin::KeyBoardKey::G: return static_cast<unsigned>(0x47);
			case Pengin::KeyBoardKey::H: return static_cast<unsigned>(0x48);
			case Pengin::KeyBoardKey::I: return static_cast<unsigned>(0x49);
			case Pengin::KeyBoardKey::J: return static_cast<unsigned>(0x4A);
			case Pengin::KeyBoardKey::K: return static_cast<unsigned>(0x4B);
			case Pengin::KeyBoardKey::L: return static_cast<unsigned>(0x4C);
			case Pengin::KeyBoardKey::M: return static_cast<unsigned>(0x4D);
			case Pengin::KeyBoardKey::N: return static_cast<unsigned>(0x4E);
			case Pengin::KeyBoardKey::O: return static_cast<unsigned>(0x4F);
			case Pengin::KeyBoardKey::P: return static_cast<unsigned>(0x50);
			case Pengin::KeyBoardKey::Q: return static_cast<unsigned>(0x51);
			case Pengin::KeyBoardKey::R: return static_cast<unsigned>(0x52);
			case Pengin::KeyBoardKey::S: return static_cast<unsigned>(0x53);
			case Pengin::KeyBoardKey::T: return static_cast<unsigned>(0x54);
			case Pengin::KeyBoardKey::U: return static_cast<unsigned>(0x55);
			case Pengin::KeyBoardKey::V: return static_cast<unsigned>(0x56);
			case Pengin::KeyBoardKey::W: return static_cast<unsigned>(0x57);
			case Pengin::KeyBoardKey::X: return static_cast<unsigned>(0x58);
			case Pengin::KeyBoardKey::Y: return static_cast<unsigned>(0x59);
			case Pengin::KeyBoardKey::Z: return static_cast<unsigned>(0x5A);
			case Pengin::KeyBoardKey::NumPad0: return static_cast<unsigned>(VK_NUMPAD0);
			case Pengin::KeyBoardKey::NumPad1: return static_cast<unsigned>(VK_NUMPAD1);
			case Pengin::KeyBoardKey::NumPad2: return static_cast<unsigned>(VK_NUMPAD2);
			case Pengin::KeyBoardKey::NumPad3: return static_cast<unsigned>(VK_NUMPAD3);
			case Pengin::KeyBoardKey::NumPad4: return static_cast<unsigned>(VK_NUMPAD4);
			case Pengin::KeyBoardKey::NumPad5: return static_cast<unsigned>(VK_NUMPAD5);
			case Pengin::KeyBoardKey::NumPad6: return static_cast<unsigned>(VK_NUMPAD6);
			case Pengin::KeyBoardKey::NumPad7: return static_cast<unsigned>(VK_NUMPAD7);
			case Pengin::KeyBoardKey::NumPad8: return static_cast<unsigned>(VK_NUMPAD8);
			case Pengin::KeyBoardKey::NumPad9: return static_cast<unsigned>(VK_NUMPAD9);
			case Pengin::KeyBoardKey::Multiply: return static_cast<unsigned>(VK_MULTIPLY);
			case Pengin::KeyBoardKey::Add: return static_cast<unsigned>(VK_ADD);
			case Pengin::KeyBoardKey::Subtract: return static_cast<unsigned>(VK_SUBTRACT);
			case Pengin::KeyBoardKey::Decimal: return static_cast<unsigned>(VK_DECIMAL);
			case Pengin::KeyBoardKey::Divide: return static_cast<unsigned>(VK_DIVIDE);
			case Pengin::KeyBoardKey::F1: return static_cast<unsigned>(VK_F1);
			case Pengin::KeyBoardKey::F2: return static_cast<unsigned>(VK_F2);
			case Pengin::KeyBoardKey::F3: return static_cast<unsigned>(VK_F3);
			case Pengin::KeyBoardKey::F4: return static_cast<unsigned>(VK_F4);
			case Pengin::KeyBoardKey::F5: return static_cast<unsigned>(VK_F5);
			case Pengin::KeyBoardKey::F6: return static_cast<unsigned>(VK_F6);
			case Pengin::KeyBoardKey::F7: return static_cast<unsigned>(VK_F7);
			case Pengin::KeyBoardKey::F8: return static_cast<unsigned>(VK_F8);
			case Pengin::KeyBoardKey::F9: return static_cast<unsigned>(VK_F9);
			case Pengin::KeyBoardKey::F10: return static_cast<unsigned>(VK_F10);
			case Pengin::KeyBoardKey::F11: return static_cast<unsigned>(VK_F11);
			case Pengin::KeyBoardKey::F12: return static_cast<unsigned>(VK_F12);
			case Pengin::KeyBoardKey::Numlock: return static_cast<unsigned>(VK_NUMLOCK);
			case Pengin::KeyBoardKey::Scrollock: return static_cast<unsigned>(VK_SCROLL);
			default: return 0x0000;
		}
	}

	bool InputKeyboard::WindowsKeyboardImpl::IsDownThisFrame(unsigned btn) const
	{
		return m_KBButtonsPressedThisFrame[btn] & 0x80;
	}

	bool InputKeyboard::WindowsKeyboardImpl::IsUpThisFrame(unsigned btn) const
	{
		return m_KBButtonsReleasedThisFrame[btn] & 0x80;
	}

	bool InputKeyboard::WindowsKeyboardImpl::IsPressed(unsigned btn) const
	{
		return m_CurrentKBState[btn] & 0x80;
	}

	InputKeyboard::InputKeyboard() :
		InputDevice{},
		m_WinImpl(std::make_unique<WindowsKeyboardImpl>())
	{
	}

	void InputKeyboard::ProcessInputState()
	{
		m_WinImpl->ProcessInputState();
	}

	void InputKeyboard::ProcessActions(InputBuffer* const inputBuffer, ExecActions& execActions)
	{
		m_WinImpl->ProcessMappedActions(inputBuffer, execActions);
	}

	void InputKeyboard::MapActionToInput(unsigned key, InputState inputState, std::shared_ptr<InputCommand> pInputAction)
	{
		m_WinImpl->MapActionToInput(key, inputState, pInputAction);
	}

	void InputKeyboard::UnMapInputAction(unsigned key, InputState inputState)
	{
		m_WinImpl->UnMapInputAction(key, inputState);
	}

	const std::vector<std::unordered_map<unsigned, std::shared_ptr<InputCommand>>>& InputKeyboard::GetMappedActions()
	{
		return m_WinImpl->GetMappedActions();
	}

	void InputKeyboard::Clear() noexcept
	{
		m_WinImpl->Clear();
	}

	unsigned InputKeyboard::GetCodeFromKey(unsigned key) const
	{
		return m_WinImpl->GetCodeFromKey(key);
	}

	bool InputKeyboard::IsDownThisFrame(unsigned btn) const
	{
		return m_WinImpl->IsDownThisFrame(btn);
	}

	bool InputKeyboard::IsUpThisFrame(unsigned btn) const
	{
		return m_WinImpl->IsUpThisFrame(btn);
	}

	bool InputKeyboard::IsPressed(unsigned btn) const
	{
		return m_WinImpl->IsPressed(btn);
	}
}


