#include "Serializer.h"

#include "ECS.h"
#include "Components.h"

#include <fstream>
#include <format>
#include <unordered_map>

#include <string>
#include "JsonConversion.h"

namespace Pengin
{
	bool Serializer::SerializeScene(const ECS& ecs, const std::string& sceneName, const std::filesystem::path& scenePath) const noexcept
	{
		const auto extension{ scenePath.extension() };
		if (extension == ".json")
		{
			return SerializeScene_Json(ecs, sceneName, scenePath);
		}
		else
		{
			DEBUG_OUT("wrong file extension, only support .json");
			return false;
		}
	}

	bool Serializer::DeserializeScene(std::string& sceneName, std::unordered_map<UUID, EntityId>& entityMap, ECS& ecs, const std::filesystem::path& scenePath) noexcept
	{
		const auto extension{ scenePath.extension() };
		if (extension == ".json")
		{
			return DeserializeScene_Json(sceneName, entityMap, ecs, scenePath);
		}
		else
		{
			DEBUG_OUT("wrong file extension, only support .json");
			return false;
		}
	}

	bool Serializer::SerializeEntity(const ECS& ecs, const EntityId id, const std::filesystem::path& path) const noexcept
	{
		ecs;
		id;
		path;
		return false;
		//assert(id != NULL_ENTITY_ID);

		//const auto extension{ path.extension() };		
		//if (extension == ".json")
		//{
		//	if (!SerializeEntity_Json(ecs, id, path))
		//	{
		//		return false;
		//	}
		//}
		//else
		//{
		//	DEBUG_OUT("wrong file extension, only support .json");
		//	return false;
		//}
		//return true;
	}

	bool Serializer::DeserializeEntity(ECS& ecs, const std::filesystem::path& path) noexcept
	{
		ecs;
		path;
		return false;
		//const auto extension{ path.extension() };
		//if (extension == ".json")
		//{
		//	if (!DeserializeEntity_Json(ecs, path))
		//	{
		//		return false;
		//	}
		//}
		//else
		//{
		//	DEBUG_OUT("wrong file extension, only support .json");
		//	return false;
		//}
		//return true;
	}

	bool Serializer::SerializeScene_Json(const ECS& ecs, const std::string& sceneName, const std::filesystem::path& scenePath) const noexcept
	{
		json scene;
		scene["Scene name"] = sceneName;

		const auto& allIds = ecs.GetAllEntities();

		for (const auto id : allIds)
		{
			json entityData;
			if (!SerializeSceneEntity_Json(ecs, id, entityData))
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

	bool Serializer::DeserializeScene_Json(std::string& sceneName, std::unordered_map<UUID, EntityId>& entityMap, ECS& ecs, const std::filesystem::path& scenePath) noexcept
	{
		using json = nlohmann::ordered_json;

		if (std::ifstream file{ scenePath, std::ios::in }; file.is_open())
		{
			json scene;
			file >> scene;
			if (!scene.contains("Scene name"))
			{
				return false;
			}

			sceneName = scene["Scene name"].get<std::string>();

			if (scene.contains("Entities"))
			{
				for (const auto& entityData : scene["Entities"])
				{
					if (!DeserializeSceneEntity_Json(ecs, entityMap, entityData))
					{
						return false;
					}
				}
			}
			else
			{
				DEBUG_OUT("Scene " << sceneName << " does not contain entities");
			}

			return true;
		}
		return false;
	}

	bool Serializer::SerializeSceneEntity_Json(const ECS& ecs, const EntityId id, json& j) const noexcept
	{
		using json = nlohmann::ordered_json;

		assert(id != NULL_ENTITY_ID);
		assert(ecs.Exists(id));
		assert(ecs.HasComponent<UUIDComponent>(id));

		j["Entity id"] = static_cast<uint64_t>(id);
		const auto& uuidComp = ecs.GetComponent<UUIDComponent>(id);
		j["UUID"] = uuidComp.uuid.GetUUID_PrettyStr();

		if (ecs.HasComponent<TransformComponent>(id))
		{
			const auto& transform = ecs.GetComponent<TransformComponent>(id);
			j["Transform Component"] = transform;
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
			j["Score Component"] = score;
		}
		if (ecs.HasComponent<RectColliderComponent>(id))
		{
			const auto& rectColl = ecs.GetComponent<RectColliderComponent>(id);
			j["RectCollider Component"] = rectColl;
		}
		if (ecs.HasComponent<HealthComponent>(id))
		{
			const auto& health = ecs.GetComponent<HealthComponent>(id);
			j["Health Component"] = health;
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

	bool Serializer::DeserializeSceneEntity_Json(ECS& ecs, std::unordered_map<UUID, EntityId>& entityMap, const json& entityData) noexcept
	{
		using json = nlohmann::ordered_json;

		if (!entityData.contains("Entity id"))
		{
			return false;
		}
		if (!entityData.contains("UUID"))
		{
			DEBUG_OUT("Need UUID in json file");
			return false;
		}

		assert(entityData["Entity id"] != NULL_ENTITY_ID && "Entity was invalid when serialized");

		const auto entity = ecs.CreateEntity();
		const auto uuid = UUID{ entityData["UUID"].get<std::string>() };
		ecs.AddComponent<UUIDComponent>(entity, uuid);

		entityMap[uuid] = entity;
		
		if (entityData.contains("Transform Component"))
		{
			TransformComponent transform = entityData["Transform Component"];
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
			ScoreComponent score = entityData["Score Component"];
			ecs.AddComponent<ScoreComponent>(entity, std::move(score));
		}
		if (entityData.contains("RectCollider Component"))
		{
			RectColliderComponent rectColl = entityData["RectCollider Component"];
			ecs.AddComponent<RectColliderComponent>(entity, std::move(rectColl));
		}
		if (entityData.contains("Health Component"))
		{
			HealthComponent health = entityData["Health Component"];
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
		return true;
	}

	void Serializer::OutputEntityData(const ECS& ecs, const EntityId id) const noexcept
	{
		if (ecs.HasComponent<TransformComponent>(id))
		{
			//unused in release
			const auto& transform [[maybe_unused]] = ecs.GetComponent<TransformComponent>(id);

			DEBUG_OUT("Transform Component\n");

			DEBUG_OUT("World pos " << transform.worldPos.x << " " << transform.worldPos.y << " " << transform.worldPos.z);
			DEBUG_OUT("Local pos " << transform.localPos.x << " " << transform.localPos.y << " " << transform.localPos.z);
			DEBUG_OUT("Rotation " << transform.rotation.x << " " << transform.rotation.y << " " << transform.rotation.z);
			DEBUG_OUT("Scale " << transform.scale.x << " " << transform.scale.y << " " << transform.scale.z);

			DEBUG_OUT("Relation ");
			DEBUG_OUT("Children " << transform.relation.children);
			DEBUG_OUT("First Child  " << transform.relation.firstChild);
			DEBUG_OUT("Previous Sibling " << transform.relation.prevSibling);
			DEBUG_OUT("Next Sibling " << transform.relation.nextSibling);
			DEBUG_OUT("Parent " << transform.relation.parent);

			DEBUG_OUT("Is pos dirty " << transform.isPosDirty << "\n");
		}
		if (ecs.HasComponent<SpriteComponent>(id))
		{
			const auto& sprite [[maybe_unused]] = ecs.GetComponent<SpriteComponent>(id);
			DEBUG_OUT("Sprite Component\n");

			DEBUG_OUT("Is Visible " << sprite.isVisible);
			DEBUG_OUT("Source rect " << sprite.m_SourceRect.x << " " << sprite.m_SourceRect.y << " " << sprite.m_SourceRect.width << " " << sprite.m_SourceRect.height);
			DEBUG_OUT("Texture " << sprite.m_pTexture->GetPath() << "\n");
		}
		//....
	}
}