#include "JsonSerializer.h"

#include "ECS.h"
#include "Components.h"

#include <fstream>
#include <format>
#include <unordered_map>
#include <string>

#include "JsonConversion.h"
#include "InputManager.h"

#include "json.hpp"

#include "FieldSerializer.h"
#include "SerializationRegistry.h"

namespace Pengin
{
	class JsonSerializer::JsonSerializerImpl
	{
	public:
		JsonSerializerImpl() = default;
		~JsonSerializerImpl() = default;

		using json = nlohmann::ordered_json;

		bool SerializeScene_Json(const ECS& ecs, const SceneData& sceneData, const std::filesystem::path& scenePath) const noexcept;
		bool DeserializeScene_Json(SceneData& sceneData, std::unordered_map<GameUUID, EntityId>& entityMap, ECS& ecs, const std::filesystem::path& scenePath) noexcept;

		bool SerializeInput_Json(const std::filesystem::path& filePath) const noexcept;
		std::pair<bool, InputDataVec> DeserializeInput_Json(const std::filesystem::path& filePath) noexcept;

		bool SerializeSceneEntity_Json(const ECS& ecs, const EntityId id, json& j, bool keepUUID) const noexcept;
		std::pair<bool, EntityId> DeserializeSceneEntity_Json(ECS& ecs, std::unordered_map<GameUUID, EntityId>& entityMap, const EntityId entity, const json& entityData) noexcept;

		FieldSerializer* GetFieldSerializer() noexcept;

		const std::unordered_map<std::string, std::vector<uint8_t>> ParseJsonStr(const std::string& json) const noexcept;

	private:
		FieldSerializer m_FieldSer{ FieldSerializer::SerializationType::Json };
	};


	JsonSerializer::JsonSerializer() :
		pImpl{std::make_unique<JsonSerializerImpl>() }
	{ }
	JsonSerializer::~JsonSerializer() = default;

	bool JsonSerializer::SerializeScene(const ECS& ecs, const SceneData& sceneData, const std::filesystem::path& scenePath) const noexcept
	{
		const auto extension{ scenePath.extension() };
		if (extension == ".json")
		{
			return pImpl->SerializeScene_Json(ecs, sceneData, scenePath);
		}
		else
		{
			DEBUG_OUT("wrong file extension, only support .json");
			return false;
		}
	}

	bool JsonSerializer::DeserializeScene(SceneData& sceneData, std::unordered_map<GameUUID, EntityId>& entityMap, ECS& ecs, const std::filesystem::path& scenePath) noexcept
	{
		const auto extension{ scenePath.extension() };
		if (extension == ".json")
		{
			return pImpl->DeserializeScene_Json(sceneData, entityMap, ecs, scenePath);
		}
		else
		{
			DEBUG_OUT("wrong file extension, only support .json");
			return false;
		}
	}

	bool JsonSerializer::SerializeInput(const std::filesystem::path& filePath) const noexcept
	{
		const auto extension{ filePath.extension() };
		if (extension == ".json")
		{
			return pImpl->SerializeInput_Json(filePath);
		}
		else
		{
			DEBUG_OUT("wrong file extension, only support .json");
			return false;
		}
	}

	std::pair<bool, InputDataVec> JsonSerializer::DeserializeInput(const std::filesystem::path& filePath) noexcept
	{
		const auto extension{ filePath.extension() };
		if (extension == ".json")
		{
			return pImpl->DeserializeInput_Json(filePath);
		}
		else
		{
			DEBUG_OUT("wrong file extension, only support .json");
			return { false, {} };
		}
	}

	bool JsonSerializer::SerializeSceneEntity(const ECS& ecs, const EntityId entityId, const std::filesystem::path& filePath, bool keepUUID) const noexcept
	{
		const auto extension{ filePath.extension() };
		if (extension == ".json")
		{
			json entity;
			if (!pImpl->SerializeSceneEntity_Json(ecs, entityId, entity, keepUUID))
			{
				return false;
			}
			if (std::ofstream file{ filePath, std::ios::out }; file.is_open())
			{
				file << entity.dump(4);
				return true;
			}
			return false;
		}
		else
		{
			DEBUG_OUT("wrong file extension, only support .json");
			return false;
		}
	}

	std::pair<bool, EntityId> JsonSerializer::DerserializeSceneEntity(ECS& ecs, std::unordered_map<GameUUID, EntityId>& entityMap, const std::filesystem::path& filePath, bool newUUID) noexcept
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

			const auto entityId = ecs.CreateEntity();

			const auto fileUUIDStr = entityData["UUID"].get<std::string>();
			assert(fileUUIDStr != "");
			const GameUUID fileUUID{ ( !GameUUID{ fileUUIDStr } || newUUID) ? GameUUID{ } : GameUUID{fileUUIDStr} }; //regen if null or need new UUID
			auto& uuidComp = ecs.AddComponent<UUIDComponent>(entityId, fileUUID);

			entityMap[uuidComp.uuid] = entityId;

			return pImpl->DeserializeSceneEntity_Json(ecs, entityMap, entityId, entityData);
		}
		else
		{
			DEBUG_OUT("wrong file extension, only support .json");
			return { false, NULL_ENTITY_ID };
		}
	}

	FieldSerializer* JsonSerializer::GetFieldSerializer() noexcept
	{
		return pImpl->GetFieldSerializer();
	}

	const std::unordered_map<std::string, std::vector<uint8_t>> JsonSerializer::ParseJsonStr(const std::string& json) const noexcept
	{
		return pImpl->ParseJsonStr(json);
	}

	bool JsonSerializer::JsonSerializerImpl::SerializeScene_Json(const ECS& ecs, const SceneData& sceneData, const std::filesystem::path& scenePath) const noexcept
	{
		json scene;

		scene["Scene Data"] = sceneData;

		const auto& allIds = ecs.GetAllEntities();

		for (const auto id : allIds)
		{
			json entityData;
			if (!SerializeSceneEntity_Json(ecs, id, entityData, true)) //async?
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

	bool JsonSerializer::JsonSerializerImpl::DeserializeScene_Json(SceneData& sceneData, std::unordered_map<GameUUID, EntityId>& entityMap, ECS& ecs, const std::filesystem::path& scenePath) noexcept
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
		if (sceneData.name.empty())
		{
			sceneData.name = inpSceneData_Json["SceneName"].get<std::string>();
		}

		for (const auto& player : inpSceneData_Json["PlayerUUIDs"])
		{
			sceneData.playerUUIDs.emplace_back(GameUUID{ player.get<std::string>() });
		}
		for (const auto& user : inpSceneData_Json["UserIds"])
		{
			sceneData.user_UUIDVecIdxMap[GameUUID{ user[0].get<std::string>() }] = user[1].get<size_t>();
		}
		
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

				assert(entityData.contains("UUID"));
				if (!entityData.contains("UUID"))
				{
					DEBUG_OUT("No UUID in json file");
					return false;
				}

				const auto& uuid = entityData["UUID"].get<std::string>();

				auto& uuidComp = ecs.AddComponent<UUIDComponent>(entityId, uuid);
					
				entityMap[uuidComp.uuid] = entityId;
			}

			for (const auto& entityData : scene["Entities"])
			{
				const auto& uuid = entityData["UUID"].get<std::string>();
				const auto entityId = entityMap.at(uuid);
				if (!DeserializeSceneEntity_Json(ecs, entityMap, entityId, entityData).first) //async ?
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

	bool JsonSerializer::JsonSerializerImpl::SerializeSceneEntity_Json(const ECS& ecs, const EntityId id, json& j, bool keepUUID) const noexcept
	{
		using json = nlohmann::ordered_json;

		assert(id != NULL_ENTITY_ID && "Invalid entityId");
		assert(ecs.Exists(id) && "Id doesnt eexist in the ecs");

		assert(ecs.HasComponent<UUIDComponent>(id) && "Must have UUID component to exist in scene");
		assert(ecs.HasComponent<TransformComponent>(id) && "Must have Transform component to exist in scene");


		if (ecs.HasComponent<TagComponent>(id))
		{
			const auto& tag = ecs.GetComponent<TagComponent>(id);
			j["Tag Component"] = tag.tag;
		}

		if (keepUUID)
		{
			const auto& uuidComp = ecs.GetComponent<UUIDComponent>(id);
			j["UUID"] = uuidComp.uuid.GetUUID_PrettyStr();
		}
		else
		{
			const auto uuidComp = UUIDComponent{ GameUUID{true} };
			j["UUID"] = uuidComp.uuid.GetUUID_PrettyStr();
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

		if (ecs.HasComponent<PlayerComponent>(id))
		{
			const auto& playerComp = ecs.GetComponent<PlayerComponent>(id);
			j["Player Component"] = playerComp.userIdx ? playerComp.userIdx.GetUUID_PrettyStr() : "NULL_UUID";
		}

		using namespace nlohmann::literals;
		auto& map = SerializationRegistry::GetInstance().m_SerMap;

		for (auto& it : map)
		{
			if (ecs.HasComponent(id, it.first))
			{
				j[it.first.name()];

				std::vector<uint8_t> fieldVector;
				fieldVector.emplace_back(static_cast<uint8_t>('{'));

				it.second(m_FieldSer, ecs, id, fieldVector);

				fieldVector.emplace_back(static_cast<uint8_t>('}'));
				
				auto fieldJson = json::parse(fieldVector.begin(), fieldVector.end());
				j[it.first.name()].merge_patch(fieldJson);
			}
		}		

		return true;
	}

	std::pair<bool, EntityId> JsonSerializer::JsonSerializerImpl::DeserializeSceneEntity_Json(ECS& ecs, std::unordered_map<GameUUID, EntityId>& entityMap, const EntityId entity, const json& entityData) noexcept
	{
		using json = nlohmann::ordered_json;

		assert(entity != NULL_ENTITY_ID);
		assert(entityData.contains("Transform Component"));

		if (entityData.contains("Tag Component"))
		{
			ecs.AddComponent<TagComponent>(entity, entityData["Tag Component"].get<std::string>());
		}

		if (entityData.contains("Transform Component"))
		{
			TransformComponent transform;
			from_json(entityData["Transform Component"], transform, entityMap);
			ecs.AddComponent<TransformComponent>(entity, std::move(transform));
		}
		else
		{
			DEBUG_OUT("No transform");
			return { false, entity };
		}

		if (entityData.contains("Player Component"))
		{
			PlayerComponent playerComp{};
			std::string userIdxStr{ entityData["Player Component"].get<std::string>() };
			playerComp.userIdx = userIdxStr == "NULL_UUID" ? GameUUID{ true } : GameUUID{ userIdxStr };

			ecs.AddComponent<PlayerComponent>(entity, std::move(playerComp));
		}

		auto& map{ SerializationRegistry::GetInstance().m_DeSerMap };
		for (auto& it : map)
		{
			if (entityData.contains(it.first.name()))
			{
				std::unordered_map<std::string, std::vector<uint8_t>> serializedFields;
				json serializedJson = entityData[it.first.name()];

				for (auto& [fieldName, fieldValue] : serializedJson.items())
				{
					const std::string field{ (fieldValue.is_string() ? fieldValue.get<std::string>() : fieldValue.dump()) };

					serializedFields[fieldName] = { field.begin(),  field.end() };
				}

				it.second(m_FieldSer, ecs, entity, serializedFields, entityMap);
			}
		}

		return { true,  entity };
	}

	FieldSerializer* JsonSerializer::JsonSerializerImpl::GetFieldSerializer() noexcept
	{
		return &m_FieldSer;
	}

	const std::unordered_map<std::string, std::vector<uint8_t>> JsonSerializer::JsonSerializerImpl::ParseJsonStr(const std::string& jsonStr) const noexcept
	{
		using json = nlohmann::ordered_json;
		json jsonField = json::parse(jsonStr);

		//DEBUG_OUT(jsonStr);
		//DEBUG_OUT(jsonField.dump());

		std::unordered_map<std::string, std::vector<uint8_t>> map;

		for (const auto& [key, value] : jsonField.items())
		{
			const std::string field{ (value.is_string() ? value.get<std::string>() : value.dump()) };
			
			map[key] = { field.begin(), field.end() };

			//DEBUG_OUT("key " << key);
			//DEBUG_OUT("Value " << value);
			//DEBUG_OUT("Field " << field);
		}

		return map;
	}

	bool JsonSerializer::JsonSerializerImpl::SerializeInput_Json(const std::filesystem::path& filePath) const noexcept //TODO
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

	std::pair<bool, InputDataVec> JsonSerializer::JsonSerializerImpl::DeserializeInput_Json(const std::filesystem::path& filePath) noexcept //TOOD
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
			const UserIndex uIdx = GameUUID{ uIdx_Str };
			const unsigned uType = user["UserType"].get<unsigned>();

			inpVec.emplace_back(std::tuple{ uIdx, uType });
		}

		return { true, inpVec };
	}
}