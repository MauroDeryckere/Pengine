#ifndef INPUTKEYBOARD
#define INPUTKEYBOARD

#include "InputDevice.h"
#include "InputCommand.h"

namespace Pengin
{
	class InputKeyboard final : public InputDevice
	{
	public:
		InputKeyboard();
		virtual ~InputKeyboard() override = default;

		virtual void ProcessInputState() override;
		virtual void ProcessActions(InputBuffer* const inputBuffer, ExecActions& execActions) override;

		virtual void MapActionToInput(unsigned key, InputState inputState, std::shared_ptr<InputCommand> pInputAction) override;
		virtual void UnMapInputAction(unsigned key, InputState inputState) override;
		virtual const std::vector<std::unordered_map<unsigned, std::shared_ptr<InputCommand>>>& GetMappedActions() override;

		virtual void Clear() noexcept override;

	private:
		class WindowsKeyboardImpl;
		std::unique_ptr<WindowsKeyboardImpl> m_WinImpl;

		virtual [[nodiscard]] unsigned GetCodeFromKey(unsigned key) const override;

		virtual [[nodiscard]] bool IsDownThisFrame(unsigned btn) const override;
		virtual [[nodiscard]] bool IsUpThisFrame(unsigned btn) const override;
		virtual [[nodiscard]] bool IsPressed(unsigned btn) const override;
	};
}

#endif
