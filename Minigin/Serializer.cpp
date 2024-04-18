#include "Serializer.h"

#include "ECS.h"
#include "Components.h"

#include <fstream>
#include <format>
#include <unordered_map>

#include <string>
#include <memory>
#include "JsonConversion.h"

#include "InputManager.h"
#include "InputCommands.h"

namespace Pengin
{
	bool Serializer::SerializeScene(const ECS& ecs, const SceneData& sceneData, const std::filesystem::path& scenePath) const noexcept
	{
		const auto extension{ scenePath.extension() };
		if (extension == ".json")
		{
			return SerializeScene_Json(ecs, sceneData, scenePath);
		}
		else
		{
			DEBUG_OUT("wrong file extension, only support .json");
			return false;
		}
	}

	bool Serializer::DeserializeScene(SceneData& sceneData, std::unordered_map<UUID, EntityId>& entityMap, ECS& ecs, const std::filesystem::path& scenePath) noexcept
	{
		const auto extension{ scenePath.extension() };
		if (extension == ".json")
		{
			return DeserializeScene_Json(sceneData, entityMap, ecs, scenePath);
		}
		else
		{
			DEBUG_OUT("wrong file extension, only support .json");
			return false;
		}
	}

	bool Serializer::SerializeInput(const std::filesystem::path& filePath) const noexcept
	{
		const auto extension{ filePath.extension() };
		if (extension == ".json")
		{
			return SerializeInput_Json(filePath);
		}
		else
		{
			DEBUG_OUT("wrong file extension, only support .json");
			return false;
		}
	}

	std::pair<bool, InputDataVec> Serializer::DeserializeInput(const std::filesystem::path& filePath) noexcept
	{
		const auto extension{ filePath.extension() };
		if (extension == ".json")
		{
			return DeserializeInput_Json(filePath);
		}
		else
		{
			DEBUG_OUT("wrong file extension, only support .json");
			return { false, {} };
		}
	}

	std::pair<bool, EntityId> Serializer::DerserializeSceneEntity(ECS& ecs, std::unordered_map<UUID, EntityId>& entityMap, const std::filesystem::path& filePath) noexcept
	{
		const auto extension{ filePath.extension() };
		if (extension == ".json")
		{
			json entityData;
			if (std::ifstream file{ filePath, std::ios::in }; file.is_open())
			{
				file >> entityData;
			}
			else
			{
				return { false, NULL_ENTITY_ID };
			}
			return DeserializeSceneEntity_Json(ecs, entityMap, entityData);
		}
		else
		{
			DEBUG_OUT("wrong file extension, only support .json");
			return { false, NULL_ENTITY_ID };
		}
	}

	bool Serializer::SerializeScene_Json(const ECS& ecs, const SceneData& sceneData, const std::filesystem::path& scenePath) const noexcept
	{
		json scene;

		scene["Scene Data"] = sceneData;

		const auto& allIds = ecs.GetAllEntities();

		for (const auto id : allIds)
		{
			json entityData;
			if (!SerializeSceneEntity_Json(ecs, id, entityData)) //async?
			{
				return false;
			}
			scene["Entities"].emplace_back(entityData);
		}

		if (std::ofstream file{ scenePath, std::ios::out }; file.is_open())
		{
			file << scene.dump(4);
			return true;
		}
		return false;
	}

	bool Serializer::DeserializeScene_Json(SceneData& sceneData, std::unordered_map<UUID, EntityId>& entityMap, ECS& ecs, const std::filesystem::path& scenePath) noexcept
	{
		using json = nlohmann::ordered_json;
		json scene;

		if (std::ifstream file{ scenePath, std::ios::in }; file.is_open())
		{
			file >> scene;
		}
		else
		{
			return false;
		}

		if (!scene.contains("Scene Data"))
		{
			DEBUG_OUT("Scene data instane is required in the json file to be able to deserialize");
			return false;
		}

		const auto& inpSceneData_Json = scene["Scene Data"];
		sceneData.name = inpSceneData_Json["SceneName"].get<std::string>();
		for (const auto& player : inpSceneData_Json["PlayerUUIDs"])
		{
			sceneData.playerUUIDs.emplace_back(UUID{ player.get<std::string>() });
		}
		for (const auto& user : inpSceneData_Json["UserIds"])
		{
			sceneData.user_UUIDVecIdxMap[UUID{ user[0].get<std::string>() }] = user[1].get<size_t>();
		}
		sceneData.isUUIDInit = inpSceneData_Json["IsUUIDInit"].get<bool>();
		assert(sceneData.isUUIDInit);
				
		sceneData.sceneFileData.inputFilePath = inpSceneData_Json["SceneFileData"]["InputFilePath"].get<std::string>();
		sceneData.sceneFileData.sceneLoadPath = inpSceneData_Json["SceneFileData"]["SceneLoadPath"].get<std::string>();
		sceneData.sceneFileData.sceneSavePath = inpSceneData_Json["SceneFileData"]["SceneSavePath"].get<std::string>();
		sceneData.sceneFileData.saveSceneOnDestroy = inpSceneData_Json["SceneFileData"]["SaveSceneOnDestroy"].get<bool>();
		sceneData.sceneFileData.autoSaveTime = inpSceneData_Json["SceneFileData"]["AutoSaveTime"].get<float>();
		sceneData.sceneFileData.keepPrevInput = inpSceneData_Json["SceneFileData"]["KeepPrevInput"].get<bool>();
			
		if (scene.contains("Entities"))
		{
			for (auto& entityData : scene["Entities"]) //First we need to get all the UUIDs and create the entities to ensure any dependencies can be converted
			{
				const auto entityId = ecs.CreateEntity();
				const auto& uuid = entityData["UUID"].get<std::string>();

				auto& uuidComp = ecs.AddComponent<UUIDComponent>(entityId, uuid);
					
				entityMap[uuidComp.uuid] = entityId;
			}

			for (const auto& entityData : scene["Entities"])
			{
				if (!DeserializeSceneEntity_Json(ecs, entityMap, entityData).first) //async ?
				{
					return false;
				}
			}
		}
		else
		{
			DEBUG_OUT("Scene " << sceneData.name << " does not contain entities");
		}
		return true;
	}

	bool Serializer::SerializeSceneEntity_Json(const ECS& ecs, const EntityId id, json& j) const noexcept
	{
		using json = nlohmann::ordered_json;

		assert(id != NULL_ENTITY_ID);
		assert(ecs.Exists(id));
		assert(ecs.HasComponent<UUIDComponent>(id));

		const auto& uuidComp = ecs.GetComponent<UUIDComponent>(id);
		j["UUID"] = uuidComp.uuid.GetUUID_PrettyStr();

		if (ecs.HasComponent<PlayerComponent>(id))
		{
			const auto& player = ecs.GetComponent<PlayerComponent>(id);
			j["Player Component"];
			j["Player Component"]["UserIdx"] = player.userIdx.GetUUID_PrettyStr();
			j["Player Component"]["MovementSpeed"] = player.movementSpeed;
		}

		if (ecs.HasComponent<TransformComponent>(id))
		{
			const auto& transform = ecs.GetComponent<TransformComponent>(id);
			to_json(j["Transform Component"], transform, ecs); //conversion to UUID is necessary so we need to pass the ecs
		}
		else
		{
			DEBUG_OUT("Must have transform component to exist in scene");
			return false;
		}

		if (ecs.HasComponent<SpriteComponent>(id))
		{
			const auto& sprite = ecs.GetComponent<SpriteComponent>(id);
			j["Sprite Component"] = sprite;
		}
		if (ecs.HasComponent<VelocityComponent>(id))
		{
			const auto& vel = ecs.GetComponent<VelocityComponent>(id);
			j["Velocity Component"] = vel;
		}
		if (ecs.HasComponent<TextComponent>(id))
		{
			const auto& text = ecs.GetComponent<TextComponent>(id);
			j["Text Component"] = text;
		}
		if (ecs.HasComponent<ScoreComponent>(id))
		{
			const auto& score = ecs.GetComponent<ScoreComponent>(id);
			to_json(j["Score Component"], score, ecs);
		}
		if (ecs.HasComponent<RectColliderComponent>(id))
		{
			const auto& rectColl = ecs.GetComponent<RectColliderComponent>(id);
			j["RectCollider Component"] = rectColl;
		}
		if (ecs.HasComponent<HealthComponent>(id))
		{
			const auto& health = ecs.GetComponent<HealthComponent>(id);
			to_json(j["Health Component"], health, ecs);
		}
		if (ecs.HasComponent<FPSCounterComponent>(id))
		{
			j["FPS Component"];
		}
		if (ecs.HasComponent<AnimationComponent>(id))
		{
			const auto& ani = ecs.GetComponent<AnimationComponent>(id);
			j["Animation Component"] = ani;
		}
		if (ecs.HasComponent<TxtDisplayComponent>(id))
		{
			const auto& dis = ecs.GetComponent<TxtDisplayComponent>(id);
			j["TextDisplay Component"] = dis;
		}

		return true;
	}

	std::pair<bool, EntityId> Serializer::DeserializeSceneEntity_Json(ECS& ecs, std::unordered_map<UUID, EntityId>& entityMap, const json& entityData) noexcept
	{
		using json = nlohmann::ordered_json;

		if (!entityData.contains("UUID"))
		{
			DEBUG_OUT("Need UUID in json file");
			return {false, NULL_ENTITY_ID};
		}
		
		const auto uuid_Str = entityData["UUID"].get<std::string>();
		assert(uuid_Str != "NULL_UUID");

		const auto uuid = UUID{ uuid_Str };
		const EntityId entity = entityMap[uuid];

		assert(entity != NULL_ENTITY_ID);

		if (entityData.contains("Player Component"))
		{
			const auto userIdx = UUID{ entityData["Player Component"]["UserIdx"].get<std::string>() };
			const auto movementSpeed = entityData["Player Component"]["MovementSpeed"].get<float>();
			ecs.AddComponent<PlayerComponent>(entity, userIdx, movementSpeed);
		}

		if (entityData.contains("Transform Component"))
		{
			TransformComponent transform;
			from_json(entityData["Transform Component"], transform, entityMap);
			ecs.AddComponent<TransformComponent>(entity, std::move(transform));
		}
		if (entityData.contains("Sprite Component"))
		{
			const auto& spriteData = entityData["Sprite Component"];

			const std::string texturePath = spriteData["path"];
			auto& sprite = (texturePath == "NO PATH" ?  ecs.AddComponent<SpriteComponent>(entity) : ecs.AddComponent<SpriteComponent>(entity, texturePath));

			sprite.m_SourceRect = UtilStructs::Rectu16{ spriteData["Source rect"][0].get<uint16_t>(),
														spriteData["Source rect"][1].get<uint16_t>(),
														spriteData["Source rect"][2].get<uint16_t>(),
														spriteData["Source rect"][3].get<uint16_t>() };

			sprite.isVisible = spriteData["is visible"];
		}
		if (entityData.contains("Velocity Component"))
		{
			VelocityComponent vel = entityData["Velocity Component"];
			ecs.AddComponent<VelocityComponent>(entity, std::move(vel));
		}
		if (entityData.contains("Text Component"))
		{
			const auto& textData = entityData["Text Component"];

			const std::string fontPath = textData["Path"];
			unsigned fontSize = static_cast<unsigned>(textData["FontSize"].get<uint64_t>());

			auto& textComp = ecs.AddComponent<TextComponent>(entity, fontPath, fontSize);
			textComp.m_Color = glm::u8vec4{ textData["Color"][0].get<uint8_t>(),
											textData["Color"][1].get<uint8_t>(),
											textData["Color"][2].get<uint8_t>(),
											textData["Color"][3].get<uint8_t>() };

			textComp.m_Text = textData["Text"].get<std::string>();

			textComp.needsTextureChange = true; //Always need to generate a texture upon deserializing
		}
		if (entityData.contains("Score Component"))
		{
			ScoreComponent score;
			from_json(entityData["Score Component"], score, entityMap);
			ecs.AddComponent<ScoreComponent>(entity, std::move(score));
		}
		if (entityData.contains("RectCollider Component"))
		{
			RectColliderComponent rectColl = entityData["RectCollider Component"];
			ecs.AddComponent<RectColliderComponent>(entity, std::move(rectColl));
		}
		if (entityData.contains("Health Component"))
		{
			HealthComponent health;
			from_json(entityData["Health Component"], health, entityMap);
			ecs.AddComponent<HealthComponent>(entity, std::move(health));
		}
		if (entityData.contains("Animation Component"))
		{
			AnimationComponent ani = entityData["Animation Component"];
			ecs.AddComponent<AnimationComponent>(entity, std::move(ani));
;		}
		if (entityData.contains("FPS Component"))
		{
			ecs.AddComponent<FPSCounterComponent>(entity);
		}
		if (entityData.contains("TextDisplay Component"))
		{
			TxtDisplayComponent dis = entityData["TextDisplay Component"];
			ecs.AddComponent<TxtDisplayComponent>(entity, std::move(dis));
		}
		return { true,  entity };
	}

	bool Serializer::SerializeInput_Json(const std::filesystem::path& filePath) const noexcept //TODO
	{
		using json = nlohmann::ordered_json;
		const auto& input = InputManager::GetInstance();

		const auto& userMap = input.m_UserIdx_VecIdxMap;
		const auto& users = input.m_RegisteredUsers;
		//const auto& combos = input.m_InputCombos; combos;

		json inputData; //Serialize using map?

		for (const auto& [userUUID, userIdx] : userMap)
		{
			json userInputData;

			userInputData["UserUUID"] = userUUID.GetUUID_PrettyStr();
			const UserType uType = users[userIdx].first;
			userInputData["UserType"] = static_cast<unsigned>(uType);

			inputData["Users"].emplace_back(userInputData);
		}

		if (std::ofstream file{ filePath, std::ios::out }; file.is_open())
		{
			file << inputData.dump(4);
			return true;
		}

		return false;
	}

	std::pair<bool, InputDataVec> Serializer::DeserializeInput_Json(const std::filesystem::path& filePath) noexcept //TOOD
	{
		using json = nlohmann::ordered_json;
		json inputData;

		if (std::ifstream file{ filePath, std::ios::in }; file.is_open())
		{
			file >> inputData;
		}
		else
		{
			return { false, {} };
		}

		InputDataVec inpVec{};

		for (const auto& user : inputData["Users"])
		{	
			const std::string uIdx_Str{ user["UserUUID"].get<std::string>() };	
			const UserIndex uIdx = UUID{ uIdx_Str };
			const unsigned uType = user["UserType"].get<unsigned>();

			inpVec.emplace_back(std::tuple{ uIdx, uType });
		}

		return { true, inpVec };
	}
}