#include "SceneManager.h"
#include "Scene.h"

#include "EventManager.h"
#include "ServiceLocator.h"

#include <algorithm>

namespace Pengin
{
	Scene* SceneManager::CreateScene(const SceneData& sceneData, bool setAsActive, bool destroyActive) noexcept
	{
		m_Scenes.emplace_back(std::make_unique<Scene>(sceneData));

		auto it{ m_SceneName_IdMap.find(sceneData.name) };
		assert(it == end(m_SceneName_IdMap) && "Can't have 2 scenes with same name.");

		if (it == end(m_SceneName_IdMap))
		{
			m_SceneName_IdMap[sceneData.name] = m_Scenes.size() - 1;
			
			if (m_ActiveSceneIdx >= 0 && destroyActive)
			{
				DEBUG_OUT("Destroying scene: " << m_ActiveSceneIdx << "Name: " << m_Scenes[m_ActiveSceneIdx]->GetName());
				m_SceneName_IdMap.erase(m_Scenes[m_ActiveSceneIdx]->GetName());
				std::erase(m_Scenes, m_Scenes[m_ActiveSceneIdx]);
			}

			if (setAsActive)
			{
				m_ActiveSceneIdx = static_cast<int32_t>(m_Scenes.size() - 1);
				m_Scenes[m_ActiveSceneIdx]->Start();
			}

			return m_Scenes.back().get();
		}

		return nullptr;
	}

	Scene* SceneManager::GetScene(const std::string& sceneName) noexcept
	{
		auto it{ m_SceneName_IdMap.find(sceneName) };
		assert(it == end(m_SceneName_IdMap) && "Scenename not found in map");

		if (it != end(m_SceneName_IdMap))
		{
			return m_Scenes[it->second].get();
		}

		return nullptr;
	}

	void SceneManager::SetSceneActive(const std::string& sceneName, bool destroyActive) noexcept
	{
		auto it = m_SceneName_IdMap.find(sceneName);

		if (it == end(m_SceneName_IdMap))
		{
			DEBUG_OUT("Invalid scene: " << sceneName);
			return;
		}

		const auto idx{ (*it).second };

		if (destroyActive)
		{
			DEBUG_OUT("Destroying scene: " << m_ActiveSceneIdx << "Name: " << m_Scenes[m_ActiveSceneIdx]->GetName());
			m_SceneName_IdMap.erase(m_Scenes[m_ActiveSceneIdx]->GetName());
			std::erase(m_Scenes, m_Scenes[m_ActiveSceneIdx]);
		}

		m_ActiveSceneIdx = static_cast<int32_t>(idx);
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

		if (static_cast<int32_t>((*it).second) == m_ActiveSceneIdx)
		{
			m_ActiveSceneIdx = -1;
		}

		std::erase(m_Scenes, m_Scenes[(*it).second]);
		m_SceneName_IdMap.erase(it);
	}

	void SceneManager::Update()
	{
		EventManager::GetInstance().ProcessEventQueue();
		ServiceLocator::GetSoundSystem().Update();

		if (m_ActiveSceneIdx >= 0)
		{
			m_Scenes[m_ActiveSceneIdx]->m_Ecs.CleanUpDestroys(); //any component removes / entity destroys should happen here and not before an event or during the update.
			m_Scenes[m_ActiveSceneIdx]->Update();
		}
	}

	void SceneManager::FixedUpdate()
	{
		if (m_ActiveSceneIdx >= 0)
		{
			m_Scenes[m_ActiveSceneIdx]->FixedUpdate();
		}
	}

	void SceneManager::Render() const
	{
		if (m_ActiveSceneIdx >= 0)
		{
			m_Scenes[m_ActiveSceneIdx]->Render();
		}
	}

	void SceneManager::RenderImGUI()
	{
	#ifdef USE_IMGUI
		if (m_ActiveSceneIdx >= 0)
		{
			m_Scenes[m_ActiveSceneIdx]->RenderImGUI();
		}
	#endif
	}
}