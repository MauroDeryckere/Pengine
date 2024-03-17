#ifndef INPUTKEYBOARD
#define INPUTKEYBOARD

#include <vector>
#include <unordered_map>

#include "InputDevice.h"
#include "InputCommand.h"

#include "Windows.h"

namespace Pengin
{
	struct InputCombo;
	enum class KeyBoardKey : unsigned;
	enum class InputState;

	class InputKeyboard final : public InputDevice
	{
	public:
		InputKeyboard();
		virtual ~InputKeyboard() override = default;

		virtual void ProcessInputState() override;
		virtual void ProcessMappedActions() override;

		virtual void MapActionToInput(unsigned key, InputState inputState, std::shared_ptr<InputCommand> pInputAction) override;

		virtual void MapCombo(const InputCombo& combo) override { combo; };

	private:
		std::vector<std::unordered_map<KeyBoardKey, std::shared_ptr<InputCommand>>> m_KeyboardActionMapping;

		BYTE m_CurrentKBState[256];
		BYTE m_KBButtonsPressedThisFrame[256];
		BYTE m_KBButtonsReleasedThisFrame[256];

		virtual [[nodiscard]] unsigned GetCodeFromKey(unsigned key) const override;

		virtual [[nodiscard]] bool IsDownThisFrame(unsigned btn) const override;
		virtual [[nodiscard]] bool IsUpThisFrame(unsigned btn) const override;
		virtual [[nodiscard]] bool IsPressed(unsigned btn) const override;
	};
}

#endif
