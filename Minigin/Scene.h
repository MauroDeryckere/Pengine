#ifndef PENGIN_SCENE
#define PENGIN_SCENE

#include "CoreIncludes.h"
#include "SceneData.h"

#include "ECS.h"
#include "glm/vec3.hpp"
#include "UtilStructs.h"

#include "SystemManager.h"
#include "AchievementSystem.h"

#include <memory>
#include <filesystem>

namespace Pengin
{
	class Entity;

	class SceneInfoPanel;
	class InputInfoPanel;

	class Scene final 
	{
	public:
		~Scene();
		void Start();

		//Entities
		[[nodiscard]] Entity CreateEntity(const glm::vec3& position = { }, const glm::vec3& rotation = { }, const glm::vec3& scale = { 1, 1, 1 }, const UserIndex& userIdx  = GameUUID{ true } );
		[[nodiscard]] Entity CreatePhysicsEntity(const UtilStructs::Rectu16& collRect = UtilStructs::Rectu16{ 0, 0 ,1, 1 }, const glm::vec3& position = {}, const glm::vec3& rotation = {}, const glm::vec3& scale = { 1, 1, 1 }, const UserIndex& userIdx = GameUUID{ true });

		void DestroyEntity(Entity entity, bool keepChildren = true) noexcept;
		void DestroyEntity(const EntityId entityId, bool keepChildren = true) noexcept;

		Entity AddEntityFromFile(const std::filesystem::path& entityLoadPath, bool newUUID = true); //Load a specific entity from file into the scene
		bool SerializeEntity(const Entity entity, const std::filesystem::path& entitySavePath, bool keepUUID = false) const noexcept; //Save a specific entity to a file
		
		[[nodiscard]] const GameUUID& GetUUID(const EntityId id) const noexcept;
		[[nodiscard]] const EntityId GetEntityId(const GameUUID& uuid) const noexcept;
		[[nodiscard]] Entity GetEntity(const GameUUID& uuid) noexcept;
		//--------

		//Player
		void SetPlayer(const UserIndex& userIdx, const GameUUID& uuid) noexcept;
		void SetPlayer(const UserIndex& userIdx, const EntityId id) noexcept;
		void SetPlayer(const UserIndex& userIdx, const Entity entity) noexcept;

		[[nodiscard]] Entity GetPlayer(const UserIndex& userIdx) noexcept;
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
		template<typename DerivedSystem>
		[[nodiscard]] DerivedSystem* GetSystem() const noexcept { return static_cast<DerivedSystem*>(m_SysManager.GetSystem<DerivedSystem>().get()); }
		//-------

		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) = delete;

	private:
		friend class SceneManager;
		Scene(const SceneData& sceneData);

		friend class Entity; //to allow accessing the ecs
		ECS m_Ecs;
		std::unordered_map<GameUUID, EntityId> m_UUID_EntityIdMap;

		SceneData m_SceneData;
		SystemManager m_SysManager{};
		
		//Achievements-------------------------------
		std::unique_ptr<AchievementSystem> m_AchSys{ std::make_unique<AchievementSystem>() };
		//-----------------------------------

		//GUI--------------------------------
		std::unique_ptr<SceneInfoPanel> m_SceneInfoPanel{};
		std::unique_ptr<InputInfoPanel> m_InputInfoPanel{};
		//-----------------------------------

		bool SerializeScene() const noexcept;
		bool DeserializeScene() noexcept;
		void RegisterEngineSystems();
	};
}

#endif