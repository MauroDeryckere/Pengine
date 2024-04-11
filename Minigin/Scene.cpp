#include "Scene.h"

#include "Serializer.h"
#include "Entity.h"
#include "PlayerComponent.h"
#include "TransformComponent.h"
#include "UUIDComponent.h"

namespace Pengin
{
	Scene::Scene(const std::string& name, const path& sceneLoadPath, const path& sceneSavePath, bool saveOnDestroy) :
		m_SaveOnDestroy{ saveOnDestroy },
		m_SceneSavePath{ sceneSavePath }
	{
		m_SceneData.name = name;

		if (!sceneLoadPath.empty())
		{
			if (!DeserializeScene(sceneLoadPath))
			{
				throw std::runtime_error("Failed to deserialize scene at: " + sceneLoadPath.string());
			}
		}
	}

	Scene::~Scene()
	{
		if (m_SaveOnDestroy)
		{
			DEBUG_OUT("Saving scene " << m_SceneData.name);
			SerializeScene();
		}
	}

	bool Scene::DeserializeScene(const std::filesystem::path& scenePath) noexcept
	{
		return Serializer::GetInstance().DeserializeScene(m_SceneData, m_UUID_EntityIdMap, m_Ecs, scenePath);
	}

	Entity Scene::CreateEntity(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, size_t userIdx)
	{
		const auto id{ m_Ecs.CreateEntity() };

		Entity entity{ id, weak_from_this() };
		entity.AddComponent<TransformComponent>(position, rotation, scale);
		const auto& uuidComp = entity.AddComponent<UUIDComponent>();

		m_UUID_EntityIdMap[uuidComp.uuid] = id;

		if (userIdx != SIZE_MAX)
		{
			entity.AddComponent<PlayerComponent>(userIdx);
			SetPlayer(userIdx, uuidComp.uuid);
		}

		return entity;
	}

	bool Scene::DestroyEntity(Entity entity, bool keepChildren)
	{
		entity.SetParent({ NULL_ENTITY_ID, shared_from_this() }, keepChildren);

		const auto id = entity.GetEntityId();
		const auto& uuid = entity.GetComponent<UUIDComponent>().uuid;

		if (entity.HasComponent<PlayerComponent>())
		{
			const auto& playerComp = entity.GetComponent<PlayerComponent>();
			const auto userIdx = playerComp.userIdx;

			auto it = m_SceneData.user_UUIDVecIdxMap.find(userIdx);

			if (it != end(m_SceneData.user_UUIDVecIdxMap))
			{
				const auto vecIdx = it->second;
				assert(vecIdx < m_SceneData.playerUUIDs.size());
				
				if (m_SceneData.playerUUIDs.size() > 1) //Could possibly also delete the inputUser if necessary (/ the player specific input in future)
				{
					const EntityId lastEntityId = m_UUID_EntityIdMap.at(m_SceneData.playerUUIDs.back());
					const auto lastPlayerUserIdx = m_Ecs.GetComponent<PlayerComponent>(lastEntityId).userIdx;

					m_SceneData.playerUUIDs[vecIdx] = std::move(m_SceneData.playerUUIDs.back());
					m_SceneData.user_UUIDVecIdxMap.at(lastPlayerUserIdx) = vecIdx;
				}

				m_SceneData.user_UUIDVecIdxMap.erase(it);
				m_SceneData.playerUUIDs.pop_back();

			}
			else
			{
				DEBUG_OUT("player component was never set as a player, ensure this is intened behaviour");
			}
		}

		auto it = m_UUID_EntityIdMap.find(uuid);
		assert(it != end(m_UUID_EntityIdMap));

		m_UUID_EntityIdMap.erase(it);

		return m_Ecs.DestroyEntity(id);
	}

	bool Scene::DestroyEntity(const EntityId entityId, bool keepChildren)
	{
		return DestroyEntity({entityId, shared_from_this() }, keepChildren);
	}

	const UUID& Scene::GetUUID(const Entity entity) const
	{
		return entity.GetUUID();
	}
	const UUID& Scene::GetUUID(const EntityId id)
	{
		return Entity{ id, shared_from_this() }.GetUUID();
	}

	const EntityId Scene::GetEntityId(const UUID& uuid) const
	{
		auto it = m_UUID_EntityIdMap.find(uuid);

		if (it == end(m_UUID_EntityIdMap))
		{
			return NULL_ENTITY_ID;
		}
		return (*it).second;
	}

	void Scene::SetPlayer(const size_t userIdx, const UUID& uuid) noexcept
	{
		m_SceneData.isUUIDInit = true;

		auto it = m_SceneData.user_UUIDVecIdxMap.find(userIdx);

		if (it == end(m_SceneData.user_UUIDVecIdxMap))
		{
			m_SceneData.playerUUIDs.emplace_back(uuid);
			m_SceneData.user_UUIDVecIdxMap[userIdx] = m_SceneData.playerUUIDs.size() - 1;
		}
		else
		{
			DEBUG_OUT("Setting uuid for useridx that already exists, ensure this is the intended behaviour");
			m_SceneData.playerUUIDs[(*it).second] = uuid;
		}
	}

	void Scene::SetPlayer(const size_t userIdx, const EntityId id) noexcept
	{
		assert(m_Ecs.HasComponent<UUIDComponent>(id));
		const auto uuid = m_Ecs.GetComponent<UUIDComponent>(id).uuid;
		SetPlayer(userIdx, uuid);
	}

	void Scene::SetPlayer(const size_t userIdx, const Entity entity) noexcept
	{
		assert(entity.GetEntityId() != NULL_ENTITY_ID);
		SetPlayer(userIdx, entity.GetEntityId());
	}

	void Scene::FixedUpdate()
	{
	}

	void Scene::Update()
	{
		//Order doesnt matter
		m_TextSystem->Update();
		m_FPSSystem->Update();
		m_AnimationSystem->Update(); 
		//-------------------
		
		//Detect collision (and respond) - might adjust velocity so update the positions | no complex phjysics so not required to be in fixed update rn
		m_CollSystem->Update(); 
		//Update the character positions based on velocity - might set dirty
		m_MovementSystem->Update();
		//Update the dirty world positions that are left
		m_WorldPosSystem->Update();
	}

	void Scene::Render() const
	{
		m_RenderSystem->Render();
	}

	void Scene::RenderImGUI()
	{
		m_SceneInfoPanel->Render(m_Ecs);
		m_InputInfoPanel->Render();
	}

	bool Scene::SerializeScene() const noexcept
	{
		assert(m_SceneData.isUUIDInit && "Must init a player to serialize the scene (SetPlayer or scene data in load file)");
		return Serializer::GetInstance().SerializeScene(m_Ecs, m_SceneData, m_SceneSavePath);
	}

	//void Scene::SerializeEntity(const EntityId id) noexcept
	//{
	//	Serializer::GetInstance().SerializeEntity(m_Ecs, id, "../Data/jsonTestSerEntity.json");
	//}

	//void Scene::DeserializeEntity() noexcept
	//{
	//	Serializer::GetInstance().DeserializeEntity(m_Ecs, "../Data/jsonTestSerEntity.json");
	//}

}