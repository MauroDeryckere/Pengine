#ifndef DISPLAYCOMPONENT
#define DISPLAYCOMPONENT

#include <string>

namespace Pengin
{
	struct TxtDisplayComponent final
	{
		TxtDisplayComponent() = default;

		TxtDisplayComponent(const std::string& prefix, const std::string& postfix = {}) :
			prefix{ prefix },
			postfix{ postfix }
		{}

		~TxtDisplayComponent() = default;

		std::string prefix{};
		std::string postfix{};
	};
}

#endif