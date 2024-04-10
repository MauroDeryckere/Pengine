#include "Scene.h"

#include "Serializer.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "UUIDComponent.h"

namespace Pengin
{
	Scene::Scene(const std::string& name, const path& sceneLoadPath, const path& sceneSavePath, bool saveOnDestroy) :
		m_Name{ name },
		m_SaveOnDestroy{ saveOnDestroy },
		m_SceneSavePath{ sceneSavePath }
	{
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
			DEBUG_OUT("Saving scene " << m_Name);
			SerializeScene();
		}
	}

	bool Scene::DeserializeScene(const std::filesystem::path& scenePath) noexcept
	{
		return Serializer::GetInstance().DeserializeScene(m_Name, m_UUID_EntityIdMap, m_Ecs, scenePath);
	}

	Entity Scene::CreateEntity(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	{
		const auto id{ m_Ecs.CreateEntity() };

		Entity entity{ id, weak_from_this() };
		entity.AddComponent<TransformComponent>(position, rotation, scale);
		const auto& uuidComp = entity.AddComponent<UUIDComponent>();

		m_UUID_EntityIdMap[uuidComp.uuid] = id;

		return entity;
	}

	bool Scene::DestroyEntity(Entity entity, bool keepChildren)
	{
		entity.SetParent({ NULL_ENTITY_ID, shared_from_this() }, keepChildren);

		const auto id = entity.GetEntityId();
		const auto& uuid = entity.GetComponent<UUIDComponent>().uuid;

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
		return Serializer::GetInstance().SerializeScene(m_Ecs, m_Name, m_SceneSavePath);
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