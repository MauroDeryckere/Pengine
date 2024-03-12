#ifndef INPUTCOMMAND
#define INPUTCOMMAND

namespace Pengin
{
	class InputCommand abstract
	{
	public:
		virtual void Execute() = 0;
		virtual ~InputCommand() = default;

		InputCommand(const InputCommand&) = delete;
		InputCommand& operator=(const InputCommand&) = delete;
		InputCommand(InputCommand&&) noexcept = delete;
		InputCommand& operator=(InputCommand&&) noexcept = delete;

	protected:
		InputCommand() = default;
	};
}
#endif