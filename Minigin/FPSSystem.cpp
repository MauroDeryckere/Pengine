#include "FPSSystem.h"

#include "ECS.h"
#include "FPSCounterComponent.h"
#include "TextComponent.h"

#include "Time.h"

#include <format>

namespace Pengin
{
	void FPSSystem::Update()
	{
		auto fpsComps{ m_ECS.GetComponents<FPSCounterComponent>() };

		for (auto it{ fpsComps.begin() }; auto & entity : fpsComps)
		{
			entity.m_AccumulatedTime += Time::GetInstance().GetElapsedSec();

			++entity.m_FrameCount;

			if (entity.m_AccumulatedTime >= 1.0f)
			{
				const auto txt{ std::format("{:.1f}", entity.m_FrameCount / entity.m_AccumulatedTime) };

				auto id = fpsComps.GetIdFromIterator(it);
				auto& textComp{ m_ECS.GetComponent<TextComponent>(id) };

				textComp.m_Text = txt; //TODO Change
				textComp.needsTextureChange = true;

				entity.m_FrameCount = 0;
				entity.m_AccumulatedTime -= 1.f;
			}

			++it;
		}


	}
}
