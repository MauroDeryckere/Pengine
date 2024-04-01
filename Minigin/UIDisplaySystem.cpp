#include "UIDisplaySystem.h"

#include "EventManager.h"
#include "Observer.h"

#include "ECS.h"

#include "HealthComponent.h"
#include "ScoreComponent.h"

#include "TextComponent.h"
#include "DisplayComponent.h"

namespace Pengin
{
	UIDisplaySystem::UIDisplaySystem(ECS& ecs):
		m_ECS{ecs},
		m_pObserver{ EventManager::GetInstance().CreateObserver() }
	{
		m_pObserver->RegisterForEvent(m_pObserver, "OnHealthChangeEvent", [this](const void* eventData) { OnHealthChangeEvent(eventData); } );
		m_pObserver->RegisterForEvent(m_pObserver, "OnScoreCollectEvent", [this](const void* eventData) { OnScoreCollectEvent(eventData); } );
	}

	void UIDisplaySystem::OnHealthChangeEvent(const void* eventData)
	{
		const EntityId id{ *static_cast<const EntityId*>(eventData) };
	
		assert(m_ECS.HasComponent<HealthComponent>(id));

		auto& healthComp = m_ECS.GetComponent<HealthComponent>(id);
		
		for (const auto entity : healthComp.m_HealthDisplayIds)
		{
			assert(m_ECS.HasComponent<TextComponent>(entity));
			assert(m_ECS.HasComponent<DisplayComponent>(entity));

			auto& textComp = m_ECS.GetComponent<TextComponent>(entity);
			auto& displayComp = m_ECS.GetComponent<DisplayComponent>(entity);

			const std::string newText{ displayComp.m_Prefix + std::to_string(healthComp.m_Health) + displayComp.m_Postfix };

			textComp.m_Text = newText; //TODO change
			textComp.needsTextureChange = true;
		}
	}

	void UIDisplaySystem::OnScoreCollectEvent(const void* eventData)
	{
		const EntityId id{ *static_cast<const EntityId*>(eventData) };

		assert(m_ECS.HasComponent<ScoreComponent>(id));

		auto& scoreComp = m_ECS.GetComponent<ScoreComponent>(id);

		for (const auto entity : scoreComp.m_ScoreDisplays)
		{
			assert(m_ECS.HasComponent<TextComponent>(entity));
			assert(m_ECS.HasComponent<DisplayComponent>(entity));

			auto& textComp = m_ECS.GetComponent<TextComponent>(entity);
			auto& displayComp = m_ECS.GetComponent<DisplayComponent>(entity);

			const std::string newText{ displayComp.m_Prefix + std::to_string(scoreComp.m_Score) + displayComp.m_Postfix };

			textComp.m_Text = newText; //TODO change
			textComp.needsTextureChange = true;
		}
	}
}


