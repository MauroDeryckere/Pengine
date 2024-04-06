#ifndef INPUTINFOPANEL
#define INPUTINFOPANEL

#include "CoreIncludes.h"

namespace Pengin
{
	class InputInfoPanel final
	{
	public:
		InputInfoPanel() = default;

		~InputInfoPanel() = default;
		
		void Render();

		InputInfoPanel(const InputInfoPanel&) = delete;
		InputInfoPanel(InputInfoPanel&&) = delete;
		InputInfoPanel& operator=(const InputInfoPanel&) = delete;
		InputInfoPanel& operator=(const InputInfoPanel&&) = delete;

	private:
		void RenderUsers();
	};
}

#endif
