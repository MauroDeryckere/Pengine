#ifndef INPUTKEYBOARD
#define INPUTKEYBOARD

#include "InputDevice.h"
#include "InputCommand.h"

namespace Pengin
{
	//TODO pimpl in the class 'space'
	class WindowsKeyboardImpl;

	class InputKeyboard final : public InputDevice
	{
	public:
		InputKeyboard();
		virtual ~InputKeyboard() override = default;

		virtual void ProcessInputState() override;
		virtual void ProcessMappedActions() override;

		virtual void MapActionToInput(unsigned key, InputState inputState, std::shared_ptr<InputCommand> pInputAction) override;

	private:
		std::unique_ptr<WindowsKeyboardImpl> m_WinImpl;

		virtual [[nodiscard]] unsigned GetCodeFromKey(unsigned key) const override;

		virtual [[nodiscard]] bool IsDownThisFrame(unsigned btn) const override;
		virtual [[nodiscard]] bool IsUpThisFrame(unsigned btn) const override;
		virtual [[nodiscard]] bool IsPressed(unsigned btn) const override;
	};
}

#endif
