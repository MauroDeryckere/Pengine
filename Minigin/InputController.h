#ifndef INPUTCONTROLLER
#define INPUTCONTROLLER

#include <unordered_map>
#include <vector>
#include <memory>
#include "InputCommand.h"
#include "windows.h"
#include "Xinput.h"

namespace Pengin
{
	enum class ControllerButton : unsigned;
	enum class InputState;

	class InputController
	{
	public:
		InputController();

		void ProcessInputState();
		void ProcessMappedActions();

		void MapControllerAction(ControllerButton button, InputState inputState, std::unique_ptr<InputCommand> pInputAction);

	private:
		XINPUT_STATE m_CurrentState;

		unsigned m_ButtonsPressedThisFrame;
		unsigned m_ButtonsReleasedThisFrame;

		std::vector<std::unordered_map<ControllerButton, std::unique_ptr<InputCommand>>> m_ControllerActionMapping;

		[[nodiscard]] unsigned GetCodeFromEnum(ControllerButton button) const;

		[[nodiscard]] bool IsDownThisFrame(unsigned btn) const;
		[[nodiscard]] bool IsUpThisFrame(unsigned btn) const;
		[[nodiscard]] bool IsPressed(unsigned btn) const;
	};
}

#endif

