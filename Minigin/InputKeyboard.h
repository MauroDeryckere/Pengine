#ifndef INPUTKEYBOARD
#define INPUTKEYBOARD

#include <memory>
#include <vector>
#include <unordered_map>

#include "InputCommand.h"

#include "Windows.h"

namespace Pengin
{
	enum class KeyBoardKey : unsigned;
	enum class InputState;

	class InputKeyboard
	{
	public:
		InputKeyboard();

		void ProcessInputState();
		void ProcessMappedActions();

		void MapKeyboardAction(KeyBoardKey key, InputState inputState, std::unique_ptr<InputCommand> pInputAction);

	private:
		std::vector<std::unordered_map<KeyBoardKey, std::unique_ptr<InputCommand>>> m_KeyboardActionMapping;

		BYTE m_CurrentKBState[256];
		BYTE m_KBButtonsPressedThisFrame[256];
		BYTE m_KBButtonsReleasedThisFrame[256];

		[[nodiscard]] unsigned GetCodeFromEnum(KeyBoardKey key) const;

		[[nodiscard]] bool IsDownThisFrame(unsigned btn) const;
		[[nodiscard]] bool IsUpThisFrame(unsigned btn) const;
		[[nodiscard]] bool IsPressed(unsigned btn) const;
	};
}

#endif
