#include "UIDisplaySystem.h"

#include "CoreIncludes.h"

#include "EventManager.h"
#include "Observer.h"

#include "Scene.h"
#include "ECS.h"

#include "HealthComponent.h"
#include "ScoreComponent.h"

#include "TextComponent.h"
#include "TxtDisplayComponent.h"

#include "DeathEvent.h"
#include "ScoreCollectEvent.h"

namespace Pengo
{
	UIDisplaySystem::UIDisplaySystem(Pengin::ECS& ecs, Pengin::Scene* pScene) :
		BaseSystem{ },
		m_ECS { ecs },
		m_pScene { pScene },
		m_pObserver{ Pengin::EventManager::GetInstance().CreateObserver() }
	{
		m_pObserver->RegisterForEvent(m_pObserver, "PengoDeath", [this](const Pengin::BaseEvent& event) { OnHealthChangeEvent(event); } );
		m_pObserver->RegisterForEvent(m_pObserver, "ScoreCollect", [this](const Pengin::BaseEvent& event) { OnScoreCollectEvent(event); } );
	}

	void UIDisplaySystem::OnHealthChangeEvent(const Pengin::BaseEvent& event)
	{
		using namespace Pengin;
		const DeathEvent& healthEv{ static_cast<const DeathEvent&>(event) };
				
		const EntityId id{ healthEv.GetEntityId() };
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
			const auto& displayComp = m_ECS.GetComponent<TxtDisplayComponent>(entity);
			textComp.SetText(displayComp.prefix + std::to_string(healthComp.health) + displayComp.postfix);
		}

		std::erase_if(healthComp.healthDisplayIds, [&idsToErase](const EntityId id)
			{
				return std::find(idsToErase.begin(), idsToErase.end(), id) != idsToErase.end();
			});
	}

	void UIDisplaySystem::OnScoreCollectEvent(const Pengin::BaseEvent& event)
	{
		using namespace Pengin;
		const ScoreCollectEvent& scoreEv{ static_cast<const ScoreCollectEvent&>(event) };

		const EntityId id{ scoreEv.GetEntityId() };
		assert(m_ECS.HasComponent<ScoreComponent>(id));

		auto& scoreComp = m_ECS.GetComponent<ScoreComponent>(id);
		scoreComp.score += scoreEv.GetScore();

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
			const auto& displayComp = m_ECS.GetComponent<TxtDisplayComponent>(entity);
			textComp.SetText(displayComp.prefix + std::to_string(scoreComp.score) + displayComp.postfix);
		}

		std::erase_if(scoreComp.scoreDisplays, [&idsToErase](const EntityId id)
			{
				return std::find(idsToErase.begin(), idsToErase.end(), id) != idsToErase.end();
			});
	}
}


