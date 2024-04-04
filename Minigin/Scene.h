#ifndef SCENE
#define SCENE

#include "SceneManager.h"

#include "ECS.h"
#include "EntityId.h"
#include "glm/vec3.hpp"

#include "AchievementSystem.h"
#include "RenderSystem.h"
#include "TextSystem.h"
#include "WorldPositionSystem.h"
#include "MovementSystem.h"
#include "FPSSystem.h"
#include "UIDisplaySystem.h"

#include <memory>

namespace Pengin
{
	class Entity;
	
	class Scene final : public std::enable_shared_from_this<Scene>
	{
	public:
		~Scene() = default;

		[[nodiscard]] Entity CreateEntity(const glm::vec3& position = { 0, 0, 0 }, const glm::vec3& rotation = { 0, 0, 0 }, const glm::vec3 & scale = { 1, 1, 1 });
		bool DestroyEntity(Entity entity, bool keepChildren = true);
		bool DestroyEntity(const EntityId entityId, bool keepChildren = true);

		[[nodiscard]] ECS& GetECS() { return m_Ecs; }

		void FixedUpdate();
		void Update();
		void Render() const;

		void RenderImGUI();

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		friend std::shared_ptr<Scene> SceneManager::CreateScene(const std::string& name);

		explicit Scene(const std::string& name);
		
		friend class Entity;
		ECS m_Ecs;

		std::string m_Name;
		static unsigned m_IdCounter; 

		//Can this be moved to a more generic approach? System queue, manager, ... (base system?)
		std::unique_ptr<RenderSystem> m_RenderSystem{ std::make_unique<RenderSystem>(m_Ecs) };
		std::unique_ptr<TextSystem> m_TextSystem{ std::make_unique<TextSystem>(m_Ecs) };
		std::unique_ptr<FPSSystem> m_FPSSystem{ std::make_unique<FPSSystem>(m_Ecs) };
		std::unique_ptr<MovementSystem> m_MovementSystem{ std::make_unique<MovementSystem>(m_Ecs) };
		std::unique_ptr<WorldPositionSystem> m_WorldPosSystem{ std::make_unique<WorldPositionSystem>(m_Ecs) };

		std::unique_ptr<UIDisplaySystem> m_UIDisplaySystem{ std::make_unique<UIDisplaySystem>(m_Ecs) };


		std::unique_ptr<AchievementSystem> m_AchSys{ std::make_unique<AchievementSystem>() };
	};
}

#endif