#include "UIDisplaySystem.h"

#include "CoreIncludes.h"

#include "EventManager.h"
#include "Observer.h"

#include "Scene.h"
#include "ECS.h"

#include "HealthComponent.h"
#include "ScoreComponent.h"

#include "TextComponent.h"
#include "DisplayComponent.h"

namespace Pengin
{
	UIDisplaySystem::UIDisplaySystem(ECS& ecs, Scene* pScene) :
		m_ECS { ecs },
		m_pScene { pScene },
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
		
		std::vector<UUID> idsToErase; //Erase deleted displays from the vector
		for (const auto& entity : healthComp.m_HealthDisplayIds)
		{
			const auto displayId = m_pScene->GetEntityId(entity);
			assert(displayId != NULL_ENTITY_ID);

			if (!m_ECS.Exists(displayId))
			{
				idsToErase.emplace_back(entity);
				continue;
			}

			assert(m_ECS.HasComponent<TextComponent>(displayId));
			assert(m_ECS.HasComponent<TxtDisplayComponent>(displayId));

			auto& textComp = m_ECS.GetComponent<TextComponent>(displayId);
			auto& displayComp = m_ECS.GetComponent<TxtDisplayComponent>(displayId);

			const std::string newText{ displayComp.m_Prefix + std::to_string(healthComp.m_Health) + displayComp.m_Postfix };

			textComp.m_Text = newText; //TODO change (text system)
			textComp.needsTextureChange = true;
		}

		std::erase_if(healthComp.m_HealthDisplayIds, [&idsToErase](const UUID& id) 
			{
				return std::find(idsToErase.begin(), idsToErase.end(), id) != idsToErase.end();
			});
	}

	void UIDisplaySystem::OnScoreCollectEvent(const void* eventData)
	{
		const EntityId id{ *static_cast<const EntityId*>(eventData) };
		assert(m_ECS.HasComponent<ScoreComponent>(id));

		auto& scoreComp = m_ECS.GetComponent<ScoreComponent>(id);

		std::vector<UUID> idsToErase;
		for (const auto& entity : scoreComp.m_ScoreDisplays)
		{
			const auto displayId = m_pScene->GetEntityId(entity);

			if (!m_ECS.Exists(displayId))
			{
				idsToErase.emplace_back(entity);
				continue;
			}

			assert(m_ECS.HasComponent<TextComponent>(displayId));
			assert(m_ECS.HasComponent<TxtDisplayComponent>(displayId));

			auto& textComp = m_ECS.GetComponent<TextComponent>(displayId);
			auto& displayComp = m_ECS.GetComponent<TxtDisplayComponent>(displayId);

			const std::string newText{ displayComp.m_Prefix + std::to_string(scoreComp.m_Score) + displayComp.m_Postfix };

			textComp.m_Text = newText; //TODO change
			textComp.needsTextureChange = true;
		}

		std::erase_if(scoreComp.m_ScoreDisplays, [&idsToErase](const UUID& id)
			{
				return std::find(idsToErase.begin(), idsToErase.end(), id) != idsToErase.end();
			});
	}
}


