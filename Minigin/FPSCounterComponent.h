#ifndef FPSCOUNTERCOMPONENT
#define FPSCOUNTERCOMPONENT

#include "Time.h"
#include "ECS.h"
#include "TextComponent.h"

namespace Pengin
{
	class FPSCounterComponent
	{
	public:
		FPSCounterComponent(EntityId id) :
			m_Id{ id }
		{}

		~FPSCounterComponent() = default;

		void Update()
		{
			m_AccumulatedTime += Pengin::Time::GetInstance().GetElapsedSec();

			++m_FrameCount;

			if (m_AccumulatedTime >= 1.0f)
			{
				const auto txt{ std::format("{:.1f}", m_FrameCount / m_AccumulatedTime) };

				auto& textComp{ Pengin::ECS::GetInstance().GetComponent<TextComponent>(m_Id) };
				textComp.SetText(txt);

				m_FrameCount = 0;
				m_AccumulatedTime -= 1.f;
			}
		}

	private:
		unsigned m_FrameCount{ 0 };
		float m_AccumulatedTime{ 0.f };

		const EntityId m_Id;
	};
}

#endif