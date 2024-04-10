#include "SceneManager.h"
#include "Scene.h"

#include "EventManager.h"

#include <ranges>
#include <algorithm>

namespace Pengin
{
	std::shared_ptr<Scene> SceneManager::GetScene(const std::string& sceneName)
	{
		return m_Scenes[m_SceneName_IdMap.at(sceneName)];
	}

	void SceneManager::SetSceneActive(const std::string& sceneName, bool destroyActive)
	{
		auto it = m_SceneName_IdMap.find(sceneName);

		if (it == end(m_SceneName_IdMap))
		{
			DEBUG_OUT("Invalid scene");
			return;
		}

		const auto idx = (*it).second;;

		if (destroyActive)
		{
			DEBUG_OUT("Destroying scene: " << m_ActiveSceneIdx);
			std::erase(m_Scenes, m_Scenes[m_ActiveSceneIdx]);
		}

		m_ActiveSceneIdx = idx;
	}

	bool SceneManager::SwitchToNextScene() noexcept
	{
		++m_ActiveSceneIdx;

		if (m_ActiveSceneIdx >= m_SceneCounter)
		{
			DEBUG_OUT("Can't switch to next scene, last scene reached in scene vector.");
			std::erase(m_Scenes, m_Scenes[m_ActiveSceneIdx - 1]);
			return false;
		}

		std::erase(m_Scenes, m_Scenes[m_ActiveSceneIdx - 1]);
		return true;
	}
	void SceneManager::Update()
	{
		EventManager::GetInstance().ProcessEventQueue();

		if (m_SceneCounter > 0)
		{
			GetActiveScene()->Update();
		}
	}

	void SceneManager::FixedUpdate()
	{
		if (m_SceneCounter > 0)
		{
			GetActiveScene()->FixedUpdate();
		}
	}

	void SceneManager::Render() const
	{
		if (m_SceneCounter > 0)
		{
			m_Scenes[m_ActiveSceneIdx]->Render();
		}
	}

	void SceneManager::RenderImGUI()
	{
		if (m_SceneCounter > 0)
		{
			GetActiveScene()->RenderImGUI();
		}
	}

	std::shared_ptr<Scene> SceneManager::CreateScene(const std::string& name, const path& sceneLoadPath, const path& sceneSavePath, bool saveOnDestroy, bool swapToNext)
	{
		const auto& scene = std::shared_ptr<Scene>( new Scene{ name, sceneLoadPath, sceneSavePath, saveOnDestroy,  swapToNext } );
		m_Scenes.emplace_back(scene);

		auto it = m_SceneName_IdMap.find(name);
		assert(it == end(m_SceneName_IdMap) && "Don't have 2 scenes with same name.");

		++m_SceneCounter;
		m_SceneName_IdMap.insert(it, { name, m_SceneCounter} );

		if (swapToNext)
		{
			m_ActiveSceneIdx = m_SceneCounter - 1;
		}

		return scene;
	}
}