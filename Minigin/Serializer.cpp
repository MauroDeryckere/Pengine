#include "Serializer.h"

#include "ECS.h"
#include "Components.h"

#include <fstream>
#include <format>
#include <unordered_map>
#include <variant>

#include <string>

#include "JsonConversion.h"
#include <json.hpp>

namespace Pengin
{
	//a UUID would be preferred in future for proper serialization (add a UUID component to each entity)
	bool Serializer::SerializeEntity(ECS& ecs, const EntityId id, const std::filesystem::path& path) noexcept
	{
		assert(id != NULL_ENTITY_ID);

		constexpr bool S_OUTPUT_DATA{ true };
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
		
		if (S_OUTPUT_DATA)
		{
			DEBUG_OUT("\n Outputting entity data for entity: " << id );
			DEBUG_OUT("file path: " << path);

			OutputEntityData(ecs, id);
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

	bool Serializer::SerializeEntity_Json(ECS& ecs, const EntityId id, const std::filesystem::path& path) noexcept
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
		else
		{
			return false;
		}
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
				auto& sprite = (texturePath == "NO PATH" ? ecs.AddComponent<SpriteComponent>(entity, texturePath) : ecs.AddComponent<SpriteComponent>(entity));

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

	void Serializer::OutputEntityData(ECS& ecs, const EntityId id) const noexcept
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
	}
}