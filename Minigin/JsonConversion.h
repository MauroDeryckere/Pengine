#ifndef JSONCONVERSION
#define JSONCONVERSION

//provide the json specialization functions to convert data structures

#include "Components.h"
#include "SceneData.h"
#include <json.hpp>

namespace Pengin
{
	using json = nlohmann::ordered_json;

	//NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE();
	
	using UserIndex = UUID;

	//UUID
	void to_json(json& j, const UUID& uuid)
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
		assert(sceneData.isUUIDInit);
		j =
		{
			{"SceneName", sceneData.name},

			{"PlayerUUIDs", sceneData.playerUUIDs },
			{"UserIds", sceneData.user_UUIDVecIdxMap },

			{"IsUUIDInit", sceneData.isUUIDInit },

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
	void from_json(const json& j, TransformComponent& transform, const std::unordered_map<UUID, EntityId>& entityMap)
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

		transform.relation.children = j["relation"]["children"].get<size_t>();

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
	//----------
	
	//Sprites
	void to_json(json& j, const SpriteComponent& sprite)
	{
		j =
		{
			{"Source rect", { sprite.sourceRect.x, sprite.sourceRect.y, sprite.sourceRect.width, sprite.sourceRect.height } },
			{"is visible", sprite.isVisible },
			{"path", sprite.pTexture ? sprite.pTexture->GetPath() : "NO PATH" },

		};
	}
	//--------------

	//Velocity
	void to_json(json& j, const VelocityComponent& velocity)
	{
		j =
		{
			{"Velocity", { velocity.velocity.x, velocity.velocity.y, velocity.velocity.z } }
		};
	}
	void from_json(const json& j, VelocityComponent& velocity)
	{
		velocity.velocity = {
			j["Velocity"][0].get<float>(),
			j["Velocity"][1].get<float>(),
			j["Velocity"][2].get<float>()
		};
	}
	//--------------

	//Text
	void to_json(json& j, const TextComponent& text)
	{
		j =
		{
			{"Text", text.text},
			{"Path", text.pFont->GetPath() },
			{"FontSize", text.pFont->GetFontSize() },
			{"Color", { text.color.r, text.color.g, text.color.b, text.color.a } }
		};
	}
	//---------------

	//Score
	void to_json(json& j, const ScoreComponent& score, const ECS& ecs)
	{
		json displays;
		for (const auto& e : score.scoreDisplays)
		{
			displays.emplace_back(e == NULL_ENTITY_ID ? "NULL_UUID" : ecs.GetComponent<UUIDComponent>(e).uuid.GetUUID_PrettyStr());
		}
		
		j =
		{
			{"Score", score.score},
			{"ScoreDisplayUUIDS", displays }
		};
	}
	void from_json(const json& j, ScoreComponent& score, const std::unordered_map<UUID, EntityId>& entityMap)
	{
		score.score = j["Score"].get<unsigned>();

		const std::vector<std::string> id_Str = j["ScoreDisplayUUIDS"].get<std::vector<std::string>>();
		for (const auto& uuid : id_Str)
		{
			score.scoreDisplays.emplace_back(entityMap.at({ uuid }));
		}
	}
	//-------------

	//Rect colliider
	void to_json(json& j, const RectColliderComponent& rectColl)
	{
		j =
		{
			{"CollRect",{ rectColl.collRect.x,  rectColl.collRect.y,  rectColl.collRect.width,  rectColl.collRect.height }}
		};
	}
	void from_json(const json& j, RectColliderComponent& rectColl)
	{
		rectColl.collRect = UtilStructs::Rectu16{ 
			j["CollRect"][0].get<uint16_t>(),
			j["CollRect"][1].get<uint16_t>(),
			j["CollRect"][2].get<uint16_t>(),
			j["CollRect"][3].get<uint16_t>()
		};
	}
	//---------------

	//Health
	void to_json(json& j, const HealthComponent& health, const ECS& ecs)
	{
		json displays;
		for (const auto& e : health.healthDisplayIds)
		{
			displays.emplace_back( e == NULL_ENTITY_ID ? "NULL_UUID" : ecs.GetComponent<UUIDComponent>(e).uuid.GetUUID_PrettyStr());
		}

		j =
		{
			{"Health", health.health},
			{"HealthDisplayUUIDS", displays }
		};
	}
	void from_json(const json& j, HealthComponent& health, const std::unordered_map<UUID, EntityId>& entityMap)
	{
		health.health = j["Health"].get<unsigned>();

		const std::vector<std::string> id_Str = j["HealthDisplayUUIDS"].get<std::vector<std::string>>();
		for (const auto& uuid : id_Str)
		{
			health.healthDisplayIds.emplace_back(entityMap.at({ uuid }));
		}
	}
	//-------------

	//Animation
	void to_json(json& j, const AnimationData& aniData)
	{
		j =
		{
			{"Frame0SourceRect", {aniData.frame0sourceRect.x, aniData.frame0sourceRect.y, aniData.frame0sourceRect.width, aniData.frame0sourceRect.height} },
			{"FrameCount", aniData.frameCt},
			{"FrameDuration", aniData.frameDuration}
		};
	}
	void from_json(const json& j, AnimationData& aniData)
	{
		aniData.frame0sourceRect.x = j["Frame0SourceRect"][0].get<uint16_t>();
		aniData.frame0sourceRect.y = j["Frame0SourceRect"][1].get<uint16_t>();
		aniData.frame0sourceRect.width = j["Frame0SourceRect"][2].get<uint16_t>();
		aniData.frame0sourceRect.height = j["Frame0SourceRect"][3].get<uint16_t>();
		aniData.frameCt = j["FrameCount"].get<uint8_t>();
		aniData.frameDuration = j["FrameDuration"].get<float>();
	}

	void to_json(json& j, const AnimationComponent& ani)
	{
		j =
		{
			{"Animations", ani.animations },
			{"CurrentAnimationIndex", ani.currAnimationIdx},
			{"CurrentAnimationFrame",ani.currFrame},
			{"CurrentFrameTime", ani.m_FrameTimer},
			{"IsPlaying", ani.isPlaying}
		};
	}
	void from_json(const json& j, AnimationComponent& ani)
	{
		ani.animations = j["Animations"].get<std::vector<AnimationData>>();
		ani.currAnimationIdx = j["CurrentAnimationIndex"].get<uint8_t>();
		ani.currFrame = j["CurrentAnimationFrame"].get<uint8_t>();
		ani.m_FrameTimer = j["CurrentFrameTime"].get<float>();
		ani.isPlaying = j["IsPlaying"].get<bool>();
	}
	//---------------

	//TextDisplay
	void to_json(json& j, const TxtDisplayComponent& dis)
	{
		j =
		{
			{"Prefix", dis.prefix},
			{"Postfix", dis.postfix}
		};
	}
	void from_json(const json& j, TxtDisplayComponent& dis)
	{
		dis.prefix = j["Prefix"].get<std::string>();
		dis.postfix = j["Postfix"].get<std::string>();
	}
	//-----------
}

#endif