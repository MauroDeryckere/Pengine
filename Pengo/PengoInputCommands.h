#ifndef PENGO_INPUTCOMMANDS
#define PENGO_INPUTCOMMANDS

#include "EventManager.h"
#include "InputCommand.h"

#include "Entity.h"
#include "SceneManager.h"

#include "BodyComponent.h"
#include "PlayerComponent.h"

#include "PengoComponent.h"

#include "PengoBlockBreakEvent.h"

#include <memory>

namespace Pengo
{
	using namespace Pengin;

	class Movement final : public InputCommand
	{
	public:
		Movement(const UserIndex& user, const glm::vec3& direction) :
			InputCommand{ user, "PengoMovement" },
			m_Direction{ direction }
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
			Entity playerEntity{ entityId, pActiveScene.get() };

			if (playerEntity.HasComponent<PengoComponent>())
			{
				playerEntity.GetComponent<PengoComponent>().HandleInput( InputCommand::GetUserIdx(), InputCommand::GetActionName() );

				const auto movementSpeed = playerEntity.GetComponent<PlayerComponent>().movementSpeed;
				playerEntity.GetComponent<BodyComponent>().inputVelocity += (m_Direction * movementSpeed);
			}
		}

		virtual ~Movement() override = default;

		Movement(const Movement&) = delete;
		Movement& operator=(const Movement&) = delete;
		Movement(Movement&&) noexcept = delete;
		Movement& operator=(Movement&&) noexcept = delete;

	private:
		const glm::vec3 m_Direction;
	};

	class BreakBlock final : public InputCommand
	{
	public:
		BreakBlock(const UserIndex& user) :
			InputCommand{ user, "PengoBreakBlock" }
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
			Entity playerEntity{ entityId, pActiveScene.get() };

			if (playerEntity.HasComponent<PengoComponent>())
			{
				playerEntity.GetComponent<PengoComponent>().HandleInput(InputCommand::GetUserIdx(), InputCommand::GetActionName());
			}
		}

		virtual ~BreakBlock() override = default;

		BreakBlock(const BreakBlock&) = delete;
		BreakBlock& operator=(const BreakBlock&) = delete;
		BreakBlock(BreakBlock&&) noexcept = delete;
		BreakBlock& operator=(BreakBlock&&) noexcept = delete;
	};
}

#endif