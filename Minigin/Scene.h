#pragma once
#include "SceneManager.h"
#include "ECS.h"

#include "AchievementSystem.h"

namespace dae
{
	class GameObject;

	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);

	public:
		void FixedUpdate();
		void Update();
		void Render() const;

		void RenderGUI() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;
	private: 
		explicit Scene(const std::string& name);

		std::unique_ptr<Pengin::AchievementSystem> m_AchSys{std::make_unique<Pengin::AchievementSystem>() };

		std::string m_name;

		static unsigned int m_idCounter; 
	};

}
