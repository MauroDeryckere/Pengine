#ifndef RECTCOLLIDERCOMPONENT
#define RECTCOLLIDERCOMPONENT

#include "UtilStructs.h"

namespace Pengin
{
	struct RectColliderComponent final
	{
		RectColliderComponent(const UtilStructs::Rectu16 collRect) :
			m_CollRect{collRect}
		{
			assert(collRect.width > 0 && collRect.height > 0);
		}
		~RectColliderComponent() = default;

		UtilStructs::Rectu16 m_CollRect;
	};
}

#endif