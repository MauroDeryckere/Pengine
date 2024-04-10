#ifndef DISPLAYCOMPONENT
#define DISPLAYCOMPONENT

#include <string>

namespace Pengin
{
	struct TxtDisplayComponent final
	{
		TxtDisplayComponent() = default;

		TxtDisplayComponent(const std::string& prefix, const std::string& postfix = {}) :
			m_Prefix{ prefix },
			m_Postfix{ postfix }
		{}

		~TxtDisplayComponent() = default;

		std::string m_Prefix{};
		std::string m_Postfix{};
	};
}

#endif