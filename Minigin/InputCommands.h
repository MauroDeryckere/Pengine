#ifndef INPUTCOMMANDS
#define INPUTCOMMANDS

#include "InputCommand.h"

#include <iostream>

namespace Pengin
{
	class PrintTest final : public InputCommand
	{
	public:
		PrintTest() : InputCommand{} {}

		virtual void Execute() override { std::cout << "executed \n"; };
		virtual ~PrintTest() override = default;

		PrintTest(const PrintTest&) = delete;
		PrintTest& operator=(const PrintTest&) = delete;
		PrintTest(PrintTest&&) noexcept = delete;
		PrintTest& operator=(PrintTest&&) noexcept = delete;
	};
}

#endif