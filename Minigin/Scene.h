#ifndef SCENE
#define SCENE

#include "CoreIncludes.h"
#include "SceneData.h"

#include "ECS.h"
#include "glm/vec3.hpp"

#include "AchievementSystem.h"
#include "RenderSystem.h"
#include "TextSystem.h"
#include "WorldPositionSystem.h"
#include "MovementSystem.h"
#include "FPSSystem.h"
#include "AnimationSystem.h"
#include "CollisionSystem.h"
#include "UIDisplaySystem.h"

#include "SceneInfoPanel.h"
#include "InputInfoPanel.h"

#include <memory>
#include <filesystem>

namespace Pengin
{
	class Entity;
	class Scene final : public std::enable_shared_from_this<Scene>
	{
	public:
		~Scene();
		void Start();

		[[nodiscard]] Entity CreateEntity(const glm::vec3& position = { }, const glm::vec3& rotation = { }, const glm::vec3& scale = { 1, 1, 1 }, const UserIndex& = UUID{ true } );
		bool DestroyEntity(Entity entity, bool keepChildren = true);
		bool DestroyEntity(const EntityId entityId, bool keepChildren = true);

		//Entity AddEntityByFile(filepath);
			//DeserSceneEntity - Add entity to scene
			//Return to allow edits	

		void FixedUpdate();
		void Update();
		void Render() const;
		void RenderImGUI();

		[[nodiscard]] const UUID& GetUUID(const Entity entity) const;
		[[nodiscard]] const UUID& GetUUID(const EntityId id);
		[[nodiscard]] const EntityId GetEntityId(const UUID& uuid) const;
		[[nodiscard]] const Entity GetEntity(const UUID& uuid);

		[[nodiscard]] const std::string& GetName() const noexcept    { return m_SceneData.name; }
		[[nodiscard]] const SceneData& GetSceneData() const noexcept { return m_SceneData; }

		void SetPlayer(const UserIndex& userIdx, const UUID& uuid) noexcept;
		void SetPlayer(const UserIndex& userIdx, const EntityId id) noexcept;
		void SetPlayer(const UserIndex& userIdx, const Entity entity) noexcept;

		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) = delete;

	private:
		friend class SceneManager;
		Scene(const std::string& name, const SceneFileData& sceneFileData);

		bool SerializeScene() const noexcept;

		friend class Entity;
		ECS m_Ecs;
		std::unordered_map<UUID, EntityId> m_UUID_EntityIdMap;

		SceneData m_SceneData;

		//SYSTEMS----------------------------
		//Can this be moved to a more generic approach? System queue, manager, ... (base system?)
		std::unique_ptr<RenderSystem> m_RenderSystem{ std::make_unique<RenderSystem>(m_Ecs) };
		std::unique_ptr<TextSystem> m_TextSystem{ std::make_unique<TextSystem>(m_Ecs) };
		std::unique_ptr<FPSSystem> m_FPSSystem{ std::make_unique<FPSSystem>(m_Ecs) };
		std::unique_ptr<AnimationSystem> m_AnimationSystem{ std::make_unique<AnimationSystem>(m_Ecs) };
		std::unique_ptr<MovementSystem> m_MovementSystem{ std::make_unique<MovementSystem>(m_Ecs) };
		std::unique_ptr<CollisionSystem> m_CollSystem{ std::make_unique<CollisionSystem>(m_Ecs) };
		std::unique_ptr<WorldPositionSystem> m_WorldPosSystem{ std::make_unique<WorldPositionSystem>(m_Ecs) };

		std::unique_ptr<UIDisplaySystem> m_UIDisplaySystem{ std::make_unique<UIDisplaySystem>(m_Ecs, this) };

		std::unique_ptr<AchievementSystem> m_AchSys{ std::make_unique<AchievementSystem>() };
		//-----------------------------------

		//GUI--------------------------------
		std::unique_ptr<SceneInfoPanel> m_SceneInfoPanel{ std::make_unique<SceneInfoPanel>(this) };
		std::unique_ptr<InputInfoPanel> m_InputInfoPanel{ std::make_unique<InputInfoPanel>() };
		//-----------------------------------

		bool DeserializeScene() noexcept;
	};
}

#endif