#ifndef RECTCOLLIDERCOMPONENT
#define RECTCOLLIDERCOMPONENT

#include "UtilStructs.h"

namespace Pengin
{
	struct RectColliderComponent final
	{
		RectColliderComponent(const UtilStructs::Rectu16 collRect = UtilStructs::Rectu16{0 ,0, 1, 1}) :
			m_CollRect{collRect}
		{
			assert(collRect.width > 0 && collRect.height > 0);
		}
		~RectColliderComponent() = default;

		UtilStructs::Rectu16 m_CollRect;
	};
}

#endif