#ifndef ANIMATIONCOMPONENT
#define ANIMATIONCOMPONENT

#include "UtilStructs.h"
#include <vector>

namespace Pengin
{ 
	struct AnimationData final
	{
		UtilStructs::Rectu16 frame0sourceRect;

		float frameDuration;

		uint8_t frameCt;
	};

	struct AnimationComponent final
	{
		AnimationComponent(const std::vector<AnimationData>& animations, uint8_t initAnimationIdx = 0, bool playOnInit = true) :
			m_Animations{ animations },
			m_IsPlaying{ playOnInit },
			m_CurrAnimationIdx{ initAnimationIdx }
		{}
		
		~AnimationComponent() = default;

		std::vector<AnimationData> m_Animations { };
		
		float m_FrameTimer { 0.f };

		uint8_t m_CurrAnimationIdx { 0 };
		uint8_t m_CurrFrame { 0 };

		bool m_IsPlaying { true };
	};
}


#endif