#include "Serializer.h"

#include "ECS.h"
#include "Components.h"

#include <fstream>
#include <format>
#include <unordered_map>
#include <variant>

namespace Pengin
{
	//a UUID would be preferred in future for proper serialization (add a UUID component to each entity)

	//TODO: no longer use a txt format, but support binary / json/ similar

	void Serializer::SerializeEntity(ECS& ecs, const EntityId id, const std::filesystem::path& path) noexcept
	{
		assert(id != NULL_ENTITY_ID);

		constexpr bool S_OUTPUT_DATA{ true };
		
		if (S_OUTPUT_DATA)
		{
			DEBUG_OUT("\n Outputting entity data for entity: " << id );
			DEBUG_OUT("file path: " << path);

			OutputEntityData(ecs, id);
		}

		if (std::ofstream file{ path, std::ios::out }; file.is_open())
		{
			if (ecs.HasComponent<TransformComponent>(id))
			{
				const auto& transform = ecs.GetComponent<TransformComponent>(id);
				file << "Transform";
				file << std::format(" {} {} {}",transform.worldPos.x, transform.worldPos.y, transform.worldPos.z);
				file << std::format(" {} {} {}",transform.localPos.x, transform.localPos.y, transform.localPos.z);
				file << std::format(" {} {} {}",transform.rotation.x, transform.rotation.y, transform.rotation.z);
				file << std::format(" {} {} {}",transform.scale.x, transform.scale.y, transform.scale.z);

				const auto& relation = transform.relation;
				file << " " << relation.children;
				file << " " << relation.firstChild;
				file << " " << relation.prevSibling;
				file << " " << relation.nextSibling;
				file << " " << relation.parent;

				file << std::format(" {}", transform.isPosDirty ? "true" : "false");
				file << std::endl;
			}
			if (ecs.HasComponent<SpriteComponent>(id))
			{
				const auto& sprite = ecs.GetComponent<SpriteComponent>(id);
				file << "Sprite";
				file << std::format(" {}", sprite.isVisible ? "true" : "false");
				file << std::format(" {} {} {} {}", sprite.m_SourceRect.x, sprite.m_SourceRect.y, sprite.m_SourceRect.width, sprite.m_SourceRect.height);
				file << " " << sprite.m_pTexture->GetPath();
				file << std::endl;
			}
		}
	}

	void Serializer::DeserializeEntity(ECS& ecs, const std::filesystem::path& path) noexcept
	{
		const auto entity = ecs.CreateEntity();

		if (std::ifstream file{ path, std::ios::in }; file.is_open())
		{
			std::string line;
			while (std::getline(file, line))
			{
				std::istringstream iss{ line };

				std::string componentType;
				iss >> componentType;

				if (componentType == "Transform")
				{
					auto& transform = ecs.AddComponent<TransformComponent>(entity);
				
					iss >> transform.worldPos.x >> transform.worldPos.y >> transform.worldPos.z
						>> transform.localPos.x >> transform.localPos.y >> transform.localPos.z
						>> transform.rotation.x >> transform.rotation.y >> transform.rotation.z
						>> transform.scale.x >> transform.scale.y >> transform.scale.z

						>> transform.relation.children
						>> transform.relation.firstChild
						>> transform.relation.prevSibling
						>> transform.relation.nextSibling
						>> transform.relation.parent;

					std::string isPosDirty;
					iss >> isPosDirty;
					transform.isPosDirty = (isPosDirty == "true");
				}
				else if (componentType == "Sprite")
				{
					bool visible;
					UtilStructs::Rect16 src;

					std::string texturePath;

					std::string isVisible;
					iss >> isVisible;
					visible = (isVisible == "true");

					iss >> src.x >> src.y >> src.width >> src.height
						>> texturePath;

					ecs.AddComponent<SpriteComponent>(entity, texturePath);
				}
			}
		}

		OutputEntityData(ecs, entity);
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