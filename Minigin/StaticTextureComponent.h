#ifndef STATICTEXTURECOMPONENT
#define STATICTEXTURECOMPONENT

#include "ECS.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "UtilStructs.h"

#include <memory>

namespace Pengin
{
	//Change to RenderComponent
	//Store:
	//Texture
	//isVisible
	//SourceRect
	
	struct StaticTextureComponent final
	{
	public:
		StaticTextureComponent() = default;

		StaticTextureComponent(const std::string& texturePath) :
			m_pTexture{ dae::ResourceManager::GetInstance().LoadTexture(texturePath) }
		{}

		~StaticTextureComponent() = default;

		std::shared_ptr<dae::Texture2D> m_pTexture{ nullptr };
		
	};
}

#endif