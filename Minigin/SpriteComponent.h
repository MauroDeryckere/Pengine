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
			m_pTexture{ dae::ResourceManager::GetInstance().LoadTexture(texturePath) },
			m_SourceRect{ sourceRect },
			isVisible{true}
		{}

		SpriteComponent(std::shared_ptr<dae::Texture2D> pTexture, UtilStructs::Rectu16 sourceRect = {}) :
			m_pTexture{ pTexture },
			m_SourceRect{ sourceRect },
			isVisible{ true }
		{}

		~SpriteComponent() = default;

		std::shared_ptr<dae::Texture2D> m_pTexture{};
		UtilStructs::Rectu16 m_SourceRect{};

		bool isVisible{ true };

	};
}

#endif