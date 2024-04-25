#ifndef SCENE
#define SCENE

#include "CoreIncludes.h"
#include "SceneData.h"

#include "ECS.h"
#include "glm/vec3.hpp"

#include "SceneInfoPanel.h"
#include "InputInfoPanel.h"

#include "SystemManager.h"
#include "AchievementSystem.h"

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

		//Entities
		[[nodiscard]] Entity CreateEntity(const glm::vec3& position = { }, const glm::vec3& rotation = { }, const glm::vec3& scale = { 1, 1, 1 }, const UserIndex& = GameUUID{ true } );
		bool DestroyEntity(Entity entity, bool keepChildren = true);
		bool DestroyEntity(const EntityId entityId, bool keepChildren = true);

		Entity AddEntityFromFile(const std::filesystem::path& entityLoadPath, bool newUUID = true); //Load a specific entity from file into the scene
		bool SerializeEntity(const Entity entity, const std::filesystem::path& entitySavePath, bool keepUUID = false) const noexcept; //Save a specific entity to a file
		
		[[nodiscard]] const GameUUID& GetUUID(const Entity entity) const;
		[[nodiscard]] const GameUUID& GetUUID(const EntityId id);
		[[nodiscard]] const EntityId GetEntityId(const GameUUID& uuid) const;
		[[nodiscard]] const Entity GetEntity(const GameUUID& uuid);
		//--------

		//Player
		void SetPlayer(const UserIndex& userIdx, const GameUUID& uuid) noexcept;
		void SetPlayer(const UserIndex& userIdx, const EntityId id) noexcept;
		void SetPlayer(const UserIndex& userIdx, const Entity entity) noexcept;
		//------

		//Loop
		void FixedUpdate();
		void Update();
		void Render() const;
		void RenderImGUI();
		//----

		//Data
		[[nodiscard]] const std::string& GetName() const noexcept    { return m_SceneData.name; }
		[[nodiscard]] const SceneData& GetSceneData() const noexcept { return m_SceneData; }
		//----

		//Systems
		void RegisterSystems(const std::function<void(SystemManager&, ECS&)>& fRegisterSystems) noexcept; //Register any user defined systems for the scene
		//-------

		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) = delete;

	private:
		friend class SceneManager;
		Scene(const SceneData& sceneData);
		void RegisterEngineSystems();
 
		bool SerializeScene() const noexcept;

		friend class Entity;
		ECS m_Ecs;
		std::unordered_map<GameUUID, EntityId> m_UUID_EntityIdMap;

		SceneData m_SceneData;
		SystemManager m_SysManager{};
		
		//Achievements-------------------------------
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