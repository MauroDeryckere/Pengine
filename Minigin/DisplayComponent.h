#ifndef DISPLAYCOMPONENT
#define DISPLAYCOMPONENT

#include <string>

namespace Pengin
{
	struct DisplayComponent final
	{
		DisplayComponent() = default;

		DisplayComponent(const std::string& prefix, const std::string& postfix = {}) :
			m_Prefix{ prefix },
			m_Postfix{ postfix }
		{}

		~DisplayComponent() = default;

		const std::string m_Prefix{};
		const std::string m_Postfix{};
	};
}

#endif