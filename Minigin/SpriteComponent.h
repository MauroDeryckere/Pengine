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
		SpriteComponent() = default;

		SpriteComponent(const std::string& texturePath, UtilStructs::Rectu16 sourceRect = {}) :
			pTexture{ dae::ResourceManager::GetInstance().LoadTexture(texturePath) },
			sourceRect{ sourceRect },
			isVisible{true}
		{}

		SpriteComponent(std::shared_ptr<dae::Texture2D> pTexture, UtilStructs::Rectu16 sourceRect = {}) :
			pTexture{ pTexture },
			sourceRect{ sourceRect },
			isVisible{ true }
		{}

		~SpriteComponent() = default;

		std::shared_ptr<dae::Texture2D> pTexture { nullptr };
		UtilStructs::Rectu16 sourceRect{};

		bool isVisible{ true };

		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<SpriteComponent>(id);

			fieldSer.SerializeField("Texturepath", comp.pTexture ? comp.pTexture->GetPath() : "NO PATH", fieldVector);

			fieldSer.SerializeField("SourceRect", std::vector<uint16_t>{ comp.sourceRect.x, comp.sourceRect.y, comp.sourceRect.width, comp.sourceRect.height }, fieldVector);
			fieldSer.SerializeField("IsVisible", comp.isVisible, fieldVector);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(SpriteComponent, SpriteComponent::Serialize);
}

#endif