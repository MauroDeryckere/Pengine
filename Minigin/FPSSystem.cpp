#include "FPSSystem.h"

#include "ECS.h"
#include "FPSCounterComponent.h"
#include "TextComponent.h"

#include "GameTime.h"

#include <format>

namespace Pengin
{
	void FPSSystem::Update()
	{
		auto fpsComps{ m_ECS.GetComponents<FPSCounterComponent>() };

		for (auto it{ fpsComps.begin() }; auto & entity : fpsComps)
		{
			entity.accumulatedTime += GameTime::GetInstance().GetElapsedSec();

			++entity.frameCount;

			if (entity.accumulatedTime >= 1.0f)
			{
				const auto txt{ std::format("{:.1f}", entity.frameCount / entity.accumulatedTime) };

				auto id = fpsComps.GetIdFromIterator(it);
				auto& textComp{ m_ECS.GetComponent<TextComponent>(id) };

				textComp.text = txt; //TODO Change
				textComp.needsTextureChange = true;

				entity.frameCount = 0;
				entity.accumulatedTime -= 1.f;
			}

			++it;
		}


	}
}
