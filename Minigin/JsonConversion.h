#ifndef JSONCONVERSION
#define JSONCONVERSION

#include "Components.h"
#include "SceneData.h"
#include <json.hpp>

namespace Pengin
{
	using json = nlohmann::ordered_json;

	//NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE();
	
	using UserIndex = GameUUID;

	//UUID
	void to_json(json& j, const GameUUID& uuid)
	{
		j = uuid.GetUUID_PrettyStr();
	}
	//------------

	//SceneFileData
	void to_json(json& j, const SceneFileData& sceneFileData)
	{
		j =
		{
			{"InputFilePath" ,sceneFileData.inputFilePath },
			{"SceneLoadPath" ,sceneFileData.sceneLoadPath },
			{"SceneSavePath" ,sceneFileData.sceneSavePath },
			{"SaveSceneOnDestroy" ,sceneFileData.saveSceneOnDestroy },
			{"AutoSaveTime" ,sceneFileData.autoSaveTime },
			{"KeepPrevInput" ,sceneFileData.keepPrevInput }
		};
	}
	//-------
	//SceneData
	void to_json(json& j, const SceneData& sceneData)
	{
		j =
		{
			{"SceneName", sceneData.name},

			{"PlayerUUIDs", sceneData.playerUUIDs },
			{"UserIds", sceneData.user_UUIDVecIdxMap },


			{"SceneFileData", sceneData.sceneFileData}
		};
	}
	//---------

	//Transforms
	void to_json(json& j, const TransformComponent& transform, const ECS& ecs)
	{
		j =
		{
			{"worldPos", {transform.worldPos.x, transform.worldPos.y, transform.worldPos.z} },
			{"localPos", {transform.localPos.x, transform.localPos.y, transform.localPos.z} },
			{"rotation", {transform.rotation.x, transform.rotation.y, transform.rotation.z} },
			{"scale",    {transform.scale.x, transform.scale.y, transform.scale.z} },
			{"relation",
				{
					{"children", transform.relation.children},
					{"firstChild", transform.relation.firstChild == NULL_ENTITY_ID ? "NULL_UUID" : ecs.GetComponent<UUIDComponent>(transform.relation.firstChild).uuid.GetUUID_PrettyStr() },
					{"prevSibling", transform.relation.prevSibling == NULL_ENTITY_ID ? "NULL_UUID" : ecs.GetComponent<UUIDComponent>(transform.relation.prevSibling).uuid.GetUUID_PrettyStr() },
					{"nextSibling", transform.relation.nextSibling == NULL_ENTITY_ID ? "NULL_UUID" : ecs.GetComponent<UUIDComponent>(transform.relation.nextSibling).uuid.GetUUID_PrettyStr() },
					{"parent", transform.relation.parent == NULL_ENTITY_ID ? "NULL_UUID" : ecs.GetComponent<UUIDComponent>(transform.relation.parent).uuid.GetUUID_PrettyStr() },
				}
			},
			{"isPosDirty", transform.isPosDirty}
		};
	}
	void from_json(const json& j, TransformComponent& transform, const std::unordered_map<GameUUID, EntityId>& entityMap)
	{
		transform.worldPos = {
			j["worldPos"][0].get<float>(),
			j["worldPos"][1].get<float>(),
			j["worldPos"][2].get<float>()
		};
		transform.localPos = {
			j["localPos"][0].get<float>(),
			j["localPos"][1].get<float>(),
			j["localPos"][2].get<float>()
		};
		transform.rotation = {
			j["rotation"][0].get<float>(),
			j["rotation"][1].get<float>(),
			j["rotation"][2].get<float>()
		};
		transform.scale = {
			j["scale"][0].get<float>(),
			j["scale"][1].get<float>(),
			j["scale"][2].get<float>()
		};

		transform.relation.children = j["relation"]["children"].get<uint32_t>();

		const auto fistChildStr = j["relation"]["firstChild"].get<std::string>();
		transform.relation.firstChild = (fistChildStr == "NULL_UUID" || "" ? NULL_ENTITY_ID : entityMap.at({fistChildStr}));

		const auto prevSibStr = j["relation"]["prevSibling"].get<std::string>();
		transform.relation.prevSibling = (prevSibStr == "NULL_UUID" || "" ? NULL_ENTITY_ID : entityMap.at({prevSibStr}));

		const auto nextSibStr = j["relation"]["nextSibling"].get<std::string>();
		transform.relation.nextSibling = (nextSibStr == "NULL_UUID" || "" ? NULL_ENTITY_ID : entityMap.at({nextSibStr}));

		const auto parentStr = j["relation"]["parent"].get<std::string>();
		transform.relation.parent = (parentStr == "NULL_UUID" || "" ? NULL_ENTITY_ID : entityMap.at({parentStr}));

		transform.isPosDirty = j["isPosDirty"].get<bool>();
	}
	//--------------------------
}

#endif