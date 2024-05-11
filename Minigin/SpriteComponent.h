#ifndef SPRITECOMPONENT
#define SPRITECOMPONENT

#include "ResourceManager.h"
#include "Texture2D.h"
#include "UtilStructs.h"

#include <memory>

#include "SerializationRegistry.h"

namespace Pengin
{	
	struct SpriteComponent final
	{
		std::shared_ptr<dae::Texture2D> pTexture { nullptr };
		UtilStructs::Rectu16 sourceRect{};

		bool isVisible{ true };

		SpriteComponent() = default;

		SpriteComponent(const std::string& texturePath, UtilStructs::Rectu16 sourceRect = {}) :
			pTexture{ dae::ResourceManager::GetInstance().LoadTexture(texturePath) },
			sourceRect{ sourceRect },
			isVisible{ true }
		{}

		SpriteComponent(std::shared_ptr<dae::Texture2D> pTexture, UtilStructs::Rectu16 sourceRect = {}) :
			pTexture{ pTexture },
			sourceRect{ sourceRect },
			isVisible{ true }
		{}

		~SpriteComponent() = default;

		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<SpriteComponent>(id);

			fieldSer.SerializeField("Texturepath", comp.pTexture ? comp.pTexture->GetPath() : "NO PATH", fieldVector);

			fieldSer.SerializeField("SourceRect", std::vector<uint16_t>{ comp.sourceRect.x, comp.sourceRect.y, comp.sourceRect.width, comp.sourceRect.height }, fieldVector);
			fieldSer.SerializeField("IsVisible", comp.isVisible, fieldVector);
		}
		static void Deserialize(const FieldSerializer& fieldSer, ECS& ecs, const EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>& serializedFields, const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]] )
		{
			std::string texturePath{};
			fieldSer.DeserializeField("Texturepath", texturePath, serializedFields);

			auto& sprite = (texturePath == "NO PATH" ? ecs.AddComponent<SpriteComponent>(id) : ecs.AddComponent<SpriteComponent>(id, texturePath));

			std::vector<uint16_t> srcRect{};

			fieldSer.DeserializeField("SourceRect", srcRect, serializedFields);
			sprite.sourceRect = UtilStructs::Rectu16{ srcRect[0], srcRect[1], srcRect[2], srcRect[3] };

			fieldSer.DeserializeField("IsVisible", sprite.isVisible, serializedFields);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(SpriteComponent, SpriteComponent::Serialize);
	REGISTER_DESERIALIZATION_FUNCTION(SpriteComponent, SpriteComponent::Deserialize);
}

#endif