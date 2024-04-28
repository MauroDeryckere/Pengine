#include "SceneManager.h"
#include "Scene.h"

#include "EventManager.h"
#include "ServiceLocator.h"

#include <algorithm>

namespace Pengin
{
	std::shared_ptr<Scene> SceneManager::GetScene(const std::string& sceneName) noexcept
	{
		return m_Scenes[m_SceneName_IdMap.at(sceneName)];
	}

	void SceneManager::SetSceneActive(const std::string& sceneName, bool destroyActive) noexcept
	{
		auto it = m_SceneName_IdMap.find(sceneName);

		if (it == end(m_SceneName_IdMap))
		{
			DEBUG_OUT("Invalid scene: " << sceneName);
			return;
		}

		const auto idx = (*it).second;;

		if (destroyActive)
		{
			DEBUG_OUT("Destroying scene: " << m_ActiveSceneIdx << "Name: " << m_Scenes[m_ActiveSceneIdx]->GetName());
			m_SceneName_IdMap.erase(m_Scenes[m_ActiveSceneIdx]->GetName());
			std::erase(m_Scenes, m_Scenes[m_ActiveSceneIdx]);
		}

		m_ActiveSceneIdx = idx;
		m_Scenes[idx]->Start();
	}

	void SceneManager::DestroyScene(const std::string& sceneName) noexcept
	{
		auto it = m_SceneName_IdMap.find(sceneName);

		if (it == end(m_SceneName_IdMap))
		{
			DEBUG_OUT(sceneName << " does not exist");
			return;
		}

		DEBUG_OUT("Destroying scene: " << sceneName);
		std::erase(m_Scenes, m_Scenes[(*it).second]);
		m_SceneName_IdMap.erase(it);
	}

	bool SceneManager::SwitchToNextScene() noexcept
	{
		++m_ActiveSceneIdx;

		m_SceneName_IdMap.erase(m_Scenes[m_ActiveSceneIdx - 1]->GetName());
		std::erase(m_Scenes, m_Scenes[m_ActiveSceneIdx - 1]);

		if (m_ActiveSceneIdx >= m_SceneCounter)
		{
			DEBUG_OUT("Can't switch to next scene, last scene reached in scene vector.");
			return false;
		}


		m_Scenes[m_ActiveSceneIdx]->Start();
		return true;
	}
	void SceneManager::Update()
	{
		ServiceLocator::GetSoundSystem().Update();
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

	std::shared_ptr<Scene> SceneManager::CreateScene(const SceneData& sceneData, bool swapToNext) noexcept
	{
		const auto& scene = std::shared_ptr<Scene>(new Scene{ sceneData });
		m_Scenes.emplace_back(scene);

		auto it = m_SceneName_IdMap.find(sceneData.name);
		assert(it == end(m_SceneName_IdMap) && "Don't have 2 scenes with same name.");

		++m_SceneCounter;
		m_SceneName_IdMap.insert({ sceneData.name, m_SceneCounter });

		if (swapToNext)
		{
			m_ActiveSceneIdx = m_SceneCounter - 1;
			m_Scenes[m_ActiveSceneIdx]->Start();
		}

		return scene;
	}
}