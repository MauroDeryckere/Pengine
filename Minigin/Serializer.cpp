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
	//a UUID would be preferred in future for proper serialization (add a UUID component to each entity)
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

	bool Serializer::DeserializeScene(std::string& sceneName, ECS& ecs, const std::filesystem::path& scenePath) noexcept
	{
		const auto extension{ scenePath.extension() };
		if (extension == ".json")
		{
			return DeserializeScene_Json(sceneName, ecs, scenePath);
		}
		else
		{
			DEBUG_OUT("wrong file extension, only support .json");
			return false;
		}
	}

	bool Serializer::SerializeEntity(const ECS& ecs, const EntityId id, const std::filesystem::path& path) const noexcept
	{
		assert(id != NULL_ENTITY_ID);

		const auto extension{ path.extension() };		
		if (extension == ".json")
		{
			if (!SerializeEntity_Json(ecs, id, path))
			{
				return false;
			}
		}
		else
		{
			DEBUG_OUT("wrong file extension, only support .json");
			return false;
		}
		return true;
	}

	bool Serializer::DeserializeEntity(ECS& ecs, const std::filesystem::path& path) noexcept
	{
		const auto extension{ path.extension() };
		if (extension == ".json")
		{
			if (!DeserializeEntity_Json(ecs, path))
			{
				return false;
			}
		}
		else
		{
			DEBUG_OUT("wrong file extension, only support .json");
			return false;
		}
		return true;
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

	bool Serializer::DeserializeScene_Json(std::string& sceneName, ECS& ecs, const std::filesystem::path& scenePath) noexcept
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
					if (!DeserializeSceneEntity_Json(ecs, entityData))
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

	bool Serializer::SerializeEntity_Json(const ECS& ecs, const EntityId id, const std::filesystem::path& path) const noexcept
	{
		using json = nlohmann::ordered_json;
		json entityData;

		entityData["Entity id"] = id;

		if (ecs.HasComponent<TransformComponent>(id))
		{
			const auto& transform = ecs.GetComponent<TransformComponent>(id);
			entityData["Transform Component"] = transform;
		}
		if (ecs.HasComponent<SpriteComponent>(id))
		{
			const auto& sprite = ecs.GetComponent<SpriteComponent>(id);
			entityData["Sprite Component"] = sprite;
		}

		if (std::ofstream file{ path, std::ios::out }; file.is_open())
		{
			file << entityData.dump(4);
			return true;
		}
		return false;
	}

	bool Serializer::DeserializeEntity_Json(ECS& ecs, const std::filesystem::path& path) noexcept
	{
		using json = nlohmann::ordered_json;
		if (std::ifstream file{ path, std::ios::in }; file.is_open())
		{
			json entityData;
			file >> entityData;

			if (!entityData.contains("Entity id"))
			{
				return false;
			}

			EntityId id = entityData["Entity id"];
			id;
			const auto entity = ecs.CreateEntity(); //TODO UUID comp
			entity;

			if (entityData.contains("Transform Component"))
			{
				TransformComponent transform = entityData["Transform Component"];
				ecs.AddComponent<TransformComponent>(id, std::move(transform));
			}
			if (entityData.contains("Sprite Component"))
			{
				const auto& spriteData = entityData["Sprite Component"];

				const std::string texturePath = spriteData["path"];
				auto& sprite = (texturePath == "NO PATH" ? ecs.AddComponent<SpriteComponent>(entity) : ecs.AddComponent<SpriteComponent>(entity, texturePath));

				sprite.m_SourceRect = UtilStructs::Rectu16{ static_cast<uint16_t>(spriteData["Source rect"][0]),
															static_cast<uint16_t>(spriteData["Source rect"][1]),
															static_cast<uint16_t>(spriteData["Source rect"][2]),
															static_cast<uint16_t>(spriteData["Source rect"][3]) };
				sprite.isVisible = spriteData["is visible"];
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

		j["Entity id"] = id;
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
			//TODO - requires UUID
		}
		if (ecs.HasComponent<RectColliderComponent>(id))
		{
			const auto& rectColl = ecs.GetComponent<RectColliderComponent>(id);
			j["RectCollider Component"] = rectColl;
		}
		if (ecs.HasComponent<HealthComponent>(id))
		{
			//TODO - requires UUID
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

		return true;
	}

	bool Serializer::DeserializeSceneEntity_Json(ECS& ecs, const json& entityData) noexcept
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

		EntityId id = entityData["Entity id"];
		assert(id != NULL_ENTITY_ID && "Entity was invalid when serialized");

		const auto entity = ecs.CreateEntity();
		ecs.AddComponent<UUIDComponent>(id, UUID{ entityData["UUID"].get<std::string>() });

		//TODO scene map: UUID - entityID

		if (entityData.contains("Transform Component"))
		{
			TransformComponent transform = entityData["Transform Component"];
			ecs.AddComponent<TransformComponent>(id, std::move(transform));
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
			ecs.AddComponent<VelocityComponent>(id, std::move(vel));
		}
		if (entityData.contains("Text Component"))
		{
			const auto& textData = entityData["Text Component"];

			const std::string fontPath = textData["Path"];
			const unsigned fontSize = textData["FontSize"];

			auto& textComp = ecs.AddComponent<TextComponent>(id, fontPath, fontSize);
			textComp.m_Color = glm::u8vec4{ textData["Color"][0].get<uint8_t>(),
											textData["Color"][1].get<uint8_t>(),
											textData["Color"][2].get<uint8_t>(),
											textData["Color"][3].get<uint8_t>() };

			textComp.m_Text = textData["Text"].get<std::string>();

			textComp.needsTextureChange = true; //Always need to generate a texture upon deserializing
		}
		if (entityData.contains("RectCollider Component"))
		{
			RectColliderComponent rectColl = entityData["RectCollider Component"];
			ecs.AddComponent<RectColliderComponent>(id, std::move(rectColl));
		}
		if (entityData.contains("Animation Component"))
		{
			AnimationComponent ani = entityData["Animation Component"];
			ecs.AddComponent<AnimationComponent>(id, std::move(ani));
;		}
		if (entityData.contains("FPS Component"))
		{
			ecs.AddComponent<FPSCounterComponent>(id);
		}

		return true;
	}

	void Serializer::OutputEntityData(const ECS& ecs, const EntityId id) const noexcept
	{
		if (ecs.HasComponent<TransformComponent>(id))
		{
			const auto& transform = ecs.GetComponent<TransformComponent>(id);
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
			const auto& sprite = ecs.GetComponent<SpriteComponent>(id);
			DEBUG_OUT("Sprite Component\n");

			DEBUG_OUT("Is Visible " << sprite.isVisible);
			DEBUG_OUT("Source rect " << sprite.m_SourceRect.x << " " << sprite.m_SourceRect.y << " " << sprite.m_SourceRect.width << " " << sprite.m_SourceRect.height);
			DEBUG_OUT("Texture " << sprite.m_pTexture->GetPath() << "\n");
		}
		//....
	}
}