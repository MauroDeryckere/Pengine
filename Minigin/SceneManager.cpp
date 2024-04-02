#include "SceneManager.h"
#include "Scene.h"

#include "EventManager.h"

namespace Pengin
{
	void SceneManager::Update()
	{
		EventManager::GetInstance().ProcessEventQueue();

		m_ActiveScene->Update();
	}

	void SceneManager::FixedUpdate()
	{
		m_ActiveScene->FixedUpdate();
	}

	void SceneManager::Render() const
	{
		m_ActiveScene->Render();
	}

	void SceneManager::RenderImGUI()
	{
		m_ActiveScene->RenderImGUI();
	}

	std::shared_ptr<Scene> SceneManager::CreateScene(const std::string& name)
	{
		const auto& scene = std::shared_ptr<Scene>(new Scene{ name });
		m_Scenes.push_back(scene);

		m_ActiveScene = scene;

		return scene;
	}
}