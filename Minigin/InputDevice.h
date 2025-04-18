#ifndef INPUTDEVICE
#define INPUTDEVICE

#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

#include "InputCommand.h"

namespace Pengin
{
	enum class InputState;
	struct InputCombo;

	using ExecActions = std::unordered_map<std::string, std::shared_ptr<InputCommand>>;

	class InputBuffer;

	class InputDevice abstract
	{
	public:
		virtual ~InputDevice() = default;

		virtual void ProcessInputState() = 0;
		virtual void ProcessActions(InputBuffer* const inputbuffer, ExecActions& execActions) = 0;

		virtual void MapActionToInput(unsigned key, InputState inputState, std::shared_ptr<InputCommand> pInputAction) = 0;
		virtual void UnMapInputAction(unsigned key, InputState inputState) = 0;

		virtual const std::vector<std::unordered_map<unsigned, std::shared_ptr<InputCommand>>>& GetMappedActions() = 0;

		virtual void Clear() noexcept = 0;

	protected:
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

