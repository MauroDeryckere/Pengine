#include "InputManager.h"

#include "Serializer.h"
#include "Entity.h"
#include "PlayerComponent.h"
#include "TransformComponent.h"
#include "RectColliderComponent.h"
#include "UUIDComponent.h"
#include "BodyComponent.h"

#include "SystemManager.h"
#include "EventManager.h"

#include "BaseSystem.h"

#include "RenderSystem.h"
#include "TextSystem.h"
#include "FPSSystem.h"
#include "DebugDrawSystem.h"
#include "AnimationSystem.h"
#include "GridSystem.h"

#include "PhysicsSystem.h"

#include "GameTime.h"
#include "ServiceLocator.h"

#include "SceneInfoPanel.h"
#include "InputInfoPanel.h"

namespace Pengin
{
	Scene::Scene(const SceneData& sceneData) :
		m_SceneData{ sceneData },
		m_SceneInfoPanel{ std::make_unique<SceneInfoPanel>(this) },
		m_InputInfoPanel{ std::make_unique<InputInfoPanel>() } 
	{
		RegisterEngineSystems();

		if (!m_SceneData.sceneFileData.sceneLoadPath.empty())
		{
			if (!DeserializeScene())
			{
				throw std::runtime_error("Failed to deserialize scene at: " + m_SceneData.sceneFileData.sceneLoadPath.string());
			}
		}
	}

	void Scene::RegisterEngineSystems()
	{
		m_SysManager.RegisterSystem<FPSSystem>(std::make_shared<FPSSystem>(m_Ecs));
		m_SysManager.RegisterSystem<TextSystem>(std::make_shared<TextSystem>(m_Ecs));
		m_SysManager.RegisterSystem<AnimationSystem>(std::make_shared<AnimationSystem>(m_Ecs));
		
		m_SysManager.RegisterSystem<PhysicsSystem>(std::make_shared<PhysicsSystem>(m_Ecs));

		m_SysManager.RegisterSystem<RenderSystem>(std::make_shared<RenderSystem>(m_Ecs));

		m_SysManager.RegisterSystem<GridSystem>(std::make_shared<GridSystem>(m_Ecs));
	}

	Scene::~Scene()
	{
		if (m_SceneData.sceneFileData.saveSceneOnDestroy && !m_SceneData.sceneFileData.sceneSavePath.empty())
		{
			DEBUG_OUT("Saving scene " << m_SceneData.name);
			SerializeScene();
		}
	}

	void Scene::Start()
	{
		if (m_SceneData.sceneFileData.keepPrevInput)
		{
			return;
		}

		const bool isKeyboarsFunc{ m_SceneData.sceneFileData.f_RegKeyboardInput };
		const bool isControllerFunc{ m_SceneData.sceneFileData.f_RegControllerInput };

		if (!isKeyboarsFunc && !isControllerFunc)
		{
			return;
		}


		if (m_SceneData.sceneFileData.inputFilePath.empty()) //no path but still provided a function (empty inpData)
		{
			if (isKeyboarsFunc)
			{
				m_SceneData.sceneFileData.f_RegKeyboardInput({});
			}
			if (isControllerFunc)
			{
				m_SceneData.sceneFileData.f_RegControllerInput({});
			}
		}
		else
		{
			const auto inp = ServiceLocator::GetSerializer().DeserializeInput(m_SceneData.sceneFileData.inputFilePath);

			if (!inp.first)
			{
				DEBUG_OUT("Failed to deserialize"); //likely have to throw here in future
				return;
			}

			auto& input = InputManager::GetInstance();
			input.Clear();

			for (const auto& user : inp.second)
			{
				const auto& userIndex = std::get<0>(user);
				const auto uType = static_cast<UserType>(std::get<1>(user));

				input.RegisterUser(userIndex, uType);

				if (uType == UserType::Keyboard && isKeyboarsFunc)
				{
					m_SceneData.sceneFileData.f_RegKeyboardInput(user);
				}
				else if (uType == UserType::Controller && isControllerFunc)
				{
					m_SceneData.sceneFileData.f_RegControllerInput(user);
				}
			}
		}
	}

	bool Scene::DeserializeScene() noexcept
	{
		return ServiceLocator::GetSerializer().DeserializeScene(m_SceneData, m_UUID_EntityIdMap, m_Ecs, m_SceneData.sceneFileData.sceneLoadPath);
	}

	Entity Scene::CreateEntity(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const UserIndex& userIdx)
	{
		const auto id{ m_Ecs.CreateEntity() };

		Entity entity{ id, this };
		entity.AddComponent<TransformComponent>(position, rotation, scale);
		const auto& uuidComp = entity.AddComponent<UUIDComponent>();

		m_UUID_EntityIdMap[uuidComp.uuid] = id;

		if (userIdx)
		{
			entity.AddComponent<PlayerComponent>(userIdx); //remove
			SetPlayer(userIdx, uuidComp.uuid);
		}

		//2 Options: 
		//Only use a playerComp and inp command usesthe entityID (not best, what if we switch to diff entity)
		//Only use the sceneData for players, playercomponent is redundant

		return entity;
	}

	Entity Scene::CreatePhysicsEntity(const UtilStructs::Rectu16& collRect, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const UserIndex& userIdx)
	{
		Entity ent = CreateEntity(position, rotation, scale, userIdx);
		auto& body = ent.AddComponent<BodyComponent>();

		body.currentPosition = position;
		body.lastPosition = position;

		ent.AddComponent<RectColliderComponent>(collRect);

		return ent;
	}

	void Scene::DestroyEntity(Entity entity, bool keepChildren) noexcept
	{
		entity.SetParent({ NULL_ENTITY_ID, this }, keepChildren);

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

		m_Ecs.DestroyEntity(id);
	}

	void Scene::DestroyEntity(const EntityId entityId, bool keepChildren) noexcept
	{
		DestroyEntity({entityId, this }, keepChildren);
	}

	const GameUUID& Scene::GetUUID(const EntityId id) const noexcept
	{
		assert(m_Ecs.HasComponent<UUIDComponent>(id));

		return m_Ecs.GetComponent<UUIDComponent>(id).uuid;
	}

	const EntityId Scene::GetEntityId(const GameUUID& uuid) const noexcept
	{
		auto it = m_UUID_EntityIdMap.find(uuid);

		if (it == end(m_UUID_EntityIdMap))
		{
			return NULL_ENTITY_ID;
		}
		return (*it).second;
	}

	Entity Scene::GetEntity(const GameUUID& uuid) noexcept
	{
		return Entity{ GetEntityId(uuid), this };
	}

	void Scene::SetPlayer(const UserIndex& userIdx, const GameUUID& uuid) noexcept
	{
		m_SceneData.SetPlayerUUID(userIdx, uuid);
	}

	void Scene::SetPlayer(const UserIndex& userIdx, const EntityId id) noexcept
	{
		assert(m_Ecs.HasComponent<UUIDComponent>(id));
		const auto uuid = m_Ecs.GetComponent<UUIDComponent>(id).uuid;
		SetPlayer(userIdx, uuid);
	}

	void Scene::SetPlayer(const UserIndex& userIdx, const Entity entity) noexcept
	{
		assert(entity.GetEntityId() != NULL_ENTITY_ID);
		SetPlayer(userIdx, entity.GetEntityId());
	}

	Entity Scene::GetPlayer(const UserIndex& userIdx) noexcept
	{
		const GameUUID& playerUUID{ m_SceneData.GetPlayerUUID(userIdx) };

		const EntityId id = GetEntityId(playerUUID);

		return Entity { id, this };
	}

	Entity Scene::AddEntityFromFile(const std::filesystem::path& entityLoadPath, bool newUUID)
	{
		const auto& deserVal = ServiceLocator::GetSerializer().DerserializeSceneEntity(m_Ecs, m_UUID_EntityIdMap, entityLoadPath, newUUID);
		if (!deserVal.first)
		{
			throw std::runtime_error("Failed to deserialize entity from file");
		}

		return Entity{ deserVal.second, this };
	}

	bool Scene::SerializeEntity(const Entity entity, const std::filesystem::path& entitySavePath, bool keepUUID) const noexcept
	{
		return ServiceLocator::GetSerializer().SerializeSceneEntity(m_Ecs, entity.GetEntityId(), entitySavePath, keepUUID);
	}

	void Scene::FixedUpdate()
	{
		m_SysManager.FixedUpdate();
	}

	void Scene::Update()
	{	
		m_SysManager.Update();

		if (m_SceneData.sceneFileData.autoSaveTime > 0.f) //this should possibly be sent to a separate thread if very large save file
		{
			static float currTime{ 0.f };
			currTime += GameTime::GetInstance().ElapsedSec();

			if (currTime >= m_SceneData.sceneFileData.autoSaveTime)
			{
				DEBUG_OUT("Auto saving scene: " << m_SceneData.name);
				SerializeScene();
				currTime = 0.f;
			}
		}
	}

	void Scene::Render() const
	{
		m_SysManager.Render();
	}

	void Scene::RenderImGUI()
	{
		m_SceneInfoPanel->Render(m_Ecs);
		m_InputInfoPanel->Render();
	}

	void Scene::RegisterSystems(const std::function<void(SystemManager&, ECS&)>& fRegisterSystems) noexcept
	{
		assert(fRegisterSystems);

		fRegisterSystems(m_SysManager, m_Ecs);
	}

	bool Scene::SerializeScene() const noexcept
	{
		assert(!m_SceneData.sceneFileData.sceneSavePath.empty());

		return ServiceLocator::GetSerializer().SerializeScene(m_Ecs, m_SceneData, m_SceneData.sceneFileData.sceneSavePath);
	}
}