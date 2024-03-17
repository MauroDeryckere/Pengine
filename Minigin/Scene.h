#pragma once
#include "SceneManager.h"
#include "ECS.h"

namespace dae
{
	class GameObject;

	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);

	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		void FixedUpdate();
		void Update();
		void Render() const;

		void RenderGUI() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		Pengin::ECS& GetEcs() { return m_Ecs; };

	private: 
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector < std::shared_ptr<GameObject>> m_objects{};

		//scene == gameobj -> root of all objects - TOOD

		Pengin::ECS m_Ecs;

		static unsigned int m_idCounter; 
	};

}
