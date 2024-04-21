#ifndef SPRITECOMPONENT
#define SPRITECOMPONENT

#include "ResourceManager.h"
#include "Texture2D.h"
#include "UtilStructs.h"

#include <memory>

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
	};
}

#endif