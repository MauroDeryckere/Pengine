#ifndef INPUTCOMMANDS
#define INPUTCOMMANDS

#include "SceneManager.h"

#include "InputCommand.h"
#include "EventManager.h"

#include "ECS.h"
#include "Entity.h"
#include "Components.h"

#include "glm/vec3.hpp"

#include <iostream>
#include <string>

namespace Pengin
{
	class Movement final : public InputCommand
	{
	public:
		
		Movement(const UserIndex& user, const glm::vec3& direction) :
			InputCommand{ user },
			m_Direction{direction}
		{ }

		virtual void Execute() override 
		{ 
			auto pActiveScene = SceneManager::GetInstance().GetActiveScene();

			auto it = pActiveScene->GetSceneData().user_UUIDVecIdxMap.find(InputCommand::GetUserIdx());

			if (it == end(pActiveScene->GetSceneData().user_UUIDVecIdxMap))
			{
				DEBUG_OUT("movement for a deleted playerIdx");
				return;
			}

			const auto& playerUUID = pActiveScene->GetSceneData().playerUUIDs[it->second];
			 
			const EntityId entityId = pActiveScene->GetEntityId(playerUUID);
			Entity playerEntity{ entityId, pActiveScene };

			const auto movementSpeed = playerEntity.GetComponent<PlayerComponent>().movementSpeed;
			playerEntity.GetComponent<VelocityComponent>().m_Velocity += (m_Direction * movementSpeed);
		}

		virtual ~Movement() override = default;

		Movement(const Movement&) = delete;
		Movement& operator=(const Movement&) = delete;
		Movement(Movement&&) noexcept = delete;
		Movement& operator=(Movement&&) noexcept = delete;

	private:
		const glm::vec3 m_Direction;
	};

	class AttackPlayer final : public InputCommand //bound to input for now - need to use UUID if want to use this input bound action again
	{
	public:
		AttackPlayer(const EntityId id) :
			InputCommand{ {} },
			m_Id{ id }
		{
			assert(SceneManager::GetInstance().GetActiveScene()->GetECS().HasComponent<HealthComponent>(m_Id));
		}

		virtual void Execute() override
		{
			auto& ecs = SceneManager::GetInstance().GetActiveScene()->GetECS();
			ecs.GetComponent<HealthComponent>(m_Id).m_Health--;

			Event healthchangeEvent{"OnHealthChangeEvent", &m_Id};
			EventManager::GetInstance().BroadcoastEvent(healthchangeEvent);
		}

		virtual ~AttackPlayer() override = default;

		AttackPlayer(const AttackPlayer&) = delete;
		AttackPlayer& operator=(const AttackPlayer&) = delete;
		AttackPlayer(AttackPlayer&&) noexcept = delete;
		AttackPlayer& operator=(AttackPlayer&&) noexcept = delete;

	private:
		const EntityId m_Id;
	};

	class CollectScore final : public InputCommand //bound to input for now - need to use UUID if want to use this input bound action again
	{
	public:
		CollectScore(const EntityId id, unsigned score = 10) :
			InputCommand{ {} },
			m_EntityId { id },
			m_ScoreVal{ score }
		{ }

		virtual void Execute() override
		{
			auto& ecs = SceneManager::GetInstance().GetActiveScene()->GetECS();
			ecs.GetComponent<ScoreComponent>(m_EntityId).m_Score += m_ScoreVal;

			Event scoreEvent{ "OnScoreCollectEvent" , &m_EntityId };
			EventManager::GetInstance().BroadcoastEvent(scoreEvent);
		}

		virtual ~CollectScore() override = default;

		CollectScore(const CollectScore&) = delete;
		CollectScore& operator=(const CollectScore&) = delete;
		CollectScore(CollectScore&&) noexcept = delete;
		CollectScore& operator=(CollectScore&&) noexcept = delete;

	private:
		const EntityId m_EntityId;

		const unsigned m_ScoreVal;
	};

}

#endif