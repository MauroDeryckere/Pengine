#ifndef INPUTDEVICE
#define INPUTDEVICE

#include <memory>
#include <vector>

#include "InputCommand.h"

namespace Pengin
{
	enum class InputState;
	struct InputCombo;

	class InputDevice abstract
	{
	public:
		virtual ~InputDevice() = default;

		virtual void ProcessInputState() = 0;
		virtual void ProcessMappedActions() = 0;

		virtual void MapActionToInput(unsigned key, InputState inputState, std::shared_ptr<InputCommand> pInputAction) = 0;
		virtual void MapCombo(const InputCombo& combo) = 0;

		[[nodiscard]] bool IsTriggered(InputCommand* pAction)
		{
			pAction;
			return false; //TODO implement
		}
		/*
			bool InputController::IsTriggered(InputCommand* command)
			{
				const auto it{ m_TriggeredCommands.find(command) };

				if (it != m_TriggeredCommands.end())
				{
					return true;
				}

				return false;
			}
		*/

	protected:

		//m_Triggered ...

		virtual [[nodiscard]] unsigned GetCodeFromKey(unsigned key) const = 0;

		virtual [[nodiscard]] bool IsDownThisFrame(unsigned btn) const = 0;
		virtual [[nodiscard]] bool IsUpThisFrame(unsigned btn) const = 0;
		virtual [[nodiscard]] bool IsPressed(unsigned btn) const = 0;

		InputDevice() = default;

		InputDevice(const InputDevice&) = delete;
		InputDevice(InputDevice&&) = delete;
		InputDevice& operator=(const InputDevice&) = delete;
		InputDevice& operator=(const InputDevice&&) = delete;
	};
}

#endif

