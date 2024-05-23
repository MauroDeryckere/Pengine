#ifndef PENGIN_TAGCOMPONENT
#define PENGIN_TAGCOMPONENT

#include <string>

namespace Pengin
{
	struct TagComponent final
	{
		std::string tag{ "NO TAG" };

		TagComponent(const std::string& t = { "NO TAG" }) :
			tag{ t }
		{}

		~TagComponent() = default;
	};
}

#endif 
