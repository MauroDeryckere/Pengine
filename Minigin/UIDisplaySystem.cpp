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
		BaseSystem{ },
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
		
		std::vector<EntityId> idsToErase; //Erase deleted displays from the vector
		for (const auto& entity : healthComp.healthDisplayIds)
		{
			assert(entity != NULL_ENTITY_ID);

			if (!m_ECS.Exists(entity))
			{
				idsToErase.emplace_back(entity);
				continue;
			}

			assert(m_ECS.HasComponent<TextComponent>(entity));
			assert(m_ECS.HasComponent<TxtDisplayComponent>(entity));

			auto& textComp = m_ECS.GetComponent<TextComponent>(entity);
			auto& displayComp = m_ECS.GetComponent<TxtDisplayComponent>(entity);

			const std::string newText{ displayComp.prefix + std::to_string(healthComp.health) + displayComp.postfix };

			textComp.text = newText; //TODO change (text system)
			textComp.needsTextureChange = true;
		}

		std::erase_if(healthComp.healthDisplayIds, [&idsToErase](const EntityId id)
			{
				return std::find(idsToErase.begin(), idsToErase.end(), id) != idsToErase.end();
			});
	}

	void UIDisplaySystem::OnScoreCollectEvent(const void* eventData)
	{
		const EntityId id{ *static_cast<const EntityId*>(eventData) };
		assert(m_ECS.HasComponent<ScoreComponent>(id));

		auto& scoreComp = m_ECS.GetComponent<ScoreComponent>(id);

		std::vector<EntityId> idsToErase;
		for (const auto& entity : scoreComp.scoreDisplays)
		{
			assert(entity != NULL_ENTITY_ID);

			if (!m_ECS.Exists(entity))
			{
				idsToErase.emplace_back(entity);
				continue;
			}

			assert(m_ECS.HasComponent<TextComponent>(entity));
			assert(m_ECS.HasComponent<TxtDisplayComponent>(entity));

			auto& textComp = m_ECS.GetComponent<TextComponent>(entity);
			auto& displayComp = m_ECS.GetComponent<TxtDisplayComponent>(entity);

			const std::string newText{ displayComp.prefix + std::to_string(scoreComp.score) + displayComp.postfix };

			textComp.text = newText; //TODO change
			textComp.needsTextureChange = true;
		}

		std::erase_if(scoreComp.scoreDisplays, [&idsToErase](const EntityId id)
			{
				return std::find(idsToErase.begin(), idsToErase.end(), id) != idsToErase.end();
			});
	}
}


