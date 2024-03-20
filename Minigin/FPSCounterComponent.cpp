#include "FPSCounterComponent.h"

#include "Time.h"
#include "TextComponent.h"

namespace Pengin
{
	void FPSCounterComponent::Update()
	{
		m_AccumulatedTime += Time::GetInstance().GetElapsedSec();

		++m_FrameCount;

		if (m_AccumulatedTime >= 1.0f)
		{
			const auto txt{ std::format("{:.1f}", m_FrameCount / m_AccumulatedTime) };

			auto& textComp{ ECS::GetInstance().GetComponent<TextComponent>(m_Id) };
			textComp.SetText(txt);

			m_FrameCount = 0;
			m_AccumulatedTime -= 1.f;
		}
	}
}