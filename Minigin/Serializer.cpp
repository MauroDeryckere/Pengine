#include "Serializer.h"

#include "ECS.h"
#include "Components.h"

#include <fstream>
#include <format>
#include <unordered_map>
#include <variant>

#include <string>

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
			entityData["Transform Component"] = 
			{
				{"worldPos", {transform.worldPos.x, transform.worldPos.y, transform.worldPos.z} },
				{"localPos", {transform.localPos.x, transform.localPos.y, transform.localPos.z} },
				{"rotation", {transform.rotation.x, transform.rotation.y, transform.rotation.z} },
				{"scale",    {transform.scale.x, transform.scale.y, transform.scale.z} },
				{"relation", 
					{
						{"children", transform.relation.children},
						{"firstChild", transform.relation.firstChild},
						{"prevSibling", transform.relation.prevSibling},
						{"nextSibling", transform.relation.nextSibling},
						{"parent", transform.relation.parent}
					}	
				},
				{"isPosDirty", transform.isPosDirty}
			};
		}

		if (ecs.HasComponent<SpriteComponent>(id))
		{
			const auto& sprite = ecs.GetComponent<SpriteComponent>(id);
			entityData["Sprite Component"] =
			{
				{"Source rect", {sprite.m_SourceRect.x, sprite.m_SourceRect.y, sprite.m_SourceRect.width, sprite.m_SourceRect.height} },
				{"is visible", sprite.isVisible },
				{"path", sprite.m_pTexture->GetPath() },
			};
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

			if (entityData.contains("Entity id"))
			{
				EntityId id = entityData["Entity id"];
				id;

				const auto entity = ecs.CreateEntity(); //TODO UUID comp

				for (auto it = entityData.begin(); it != entityData.end(); ++it)
				{
					if (it.key() == "Entity id")
						continue;

					if (it.key() == "Transform Component")
					{
						const auto& transformData = it.value();

						auto& transform = ecs.AddComponent<TransformComponent>(entity);
						transform.worldPos = { transformData["worldPos"][0], transformData["worldPos"][1], transformData["worldPos"][2] };
						transform.localPos = { transformData["localPos"][0], transformData["localPos"][1], transformData["localPos"][2] };
						transform.rotation = { transformData["rotation"][0], transformData["rotation"][1], transformData["rotation"][2] };
						transform.scale = { transformData["scale"][0], transformData["scale"][1], transformData["scale"][2] };

							transform.relation.children = transformData["relation"]["children"];
							transform.relation.firstChild = transformData["relation"]["firstChild"];
							transform.relation.prevSibling = transformData["relation"]["prevSibling"];
							transform.relation.nextSibling = transformData["relation"]["nextSibling"];
							transform.relation.parent = transformData["relation"]["parent"];

						transform.isPosDirty = transformData["isPosDirty"];
					}

					else if (it.key() == "Sprite Component")
					{
						const auto& spriteData = it.value();

						const std::string texturePath = spriteData["path"];
						auto& sprite = ecs.AddComponent<SpriteComponent>(entity, texturePath);

						sprite.m_SourceRect = UtilStructs::Rectu16{ static_cast<uint16_t>(spriteData["Source rect"][0]),
																	static_cast<uint16_t>(spriteData["Source rect"][1]),
																	static_cast<uint16_t>(spriteData["Source rect"][2]),
																	static_cast<uint16_t>(spriteData["Source rect"][3]) };
						sprite.isVisible = spriteData["is visible"];
					}
				}
				return true;
			}
			return false;
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