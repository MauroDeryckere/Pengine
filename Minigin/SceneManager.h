#ifndef SCENEMANAGER
#define SCENEMANAGER

#include "Singleton.h"

#include <vector>
#include <string>
#include <memory>

namespace Pengin
{
	class Scene;
	class SceneManager final : public dae::Singleton<SceneManager>
	{
	public:
		std::shared_ptr<Scene> CreateScene(const std::string& name);
		std::shared_ptr<Scene> GetActiveScene() { return m_ActiveScene; }

		void Update();
		void FixedUpdate();
		void Render() const;

		void RenderImGUI();

	private:
		friend class dae::Singleton<SceneManager>;
		SceneManager() = default;

		std::shared_ptr<Scene> m_ActiveScene;

		std::vector<std::shared_ptr<Scene>> m_Scenes;

		//TODO swpaping scenes
	};
}

#endif