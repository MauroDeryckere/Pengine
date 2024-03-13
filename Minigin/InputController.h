#ifndef INPUTCONTROLLER
#define INPUTCONTROLLER

#include "InputDevice.h"

#include <unordered_map>
#include <vector>

#include "windows.h"
#include "Xinput.h"

namespace Pengin
{
	enum class ControllerButton : unsigned;
	enum class InputState;

	class InputController final : public InputDevice
	{
	public:
		InputController();
		virtual ~InputController() override = default;

		virtual void ProcessInputState() override;
		virtual void ProcessMappedActions() override;

		virtual void MapActionToInput(unsigned key, InputState inputState, std::unique_ptr<InputCommand> pInputAction) override;

	private:
		XINPUT_STATE m_CurrentState;

		unsigned m_ButtonsPressedThisFrame;
		unsigned m_ButtonsReleasedThisFrame;

		std::vector<std::unordered_map<ControllerButton, std::unique_ptr<InputCommand>>> m_ControllerActionMapping;

		virtual [[nodiscard]] unsigned GetCodeFromKey(unsigned key) const override;

		virtual [[nodiscard]] bool IsDownThisFrame(unsigned btn) const override;
		virtual [[nodiscard]] bool IsUpThisFrame(unsigned btn) const override;
		virtual [[nodiscard]] bool IsPressed(unsigned btn) const override;
	};
}

#endif

