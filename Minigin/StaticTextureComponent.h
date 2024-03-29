#ifndef STATICTEXTURECOMPONENT
#define STATICTEXTURECOMPONENT

#include "ECS.h"
#include "ResourceManager.h"
#include "Texture2D.h"

#include <memory>

namespace Pengin
{
	class StaticTextureComponent final
	{
	public:
		StaticTextureComponent(EntityId id) :
			m_Id{ id }
		{}
		StaticTextureComponent(const std::string& texturePath, EntityId id) :
			m_pTexture{ dae::ResourceManager::GetInstance().LoadTexture(texturePath) },
			m_Id{ id }
		{}

		~StaticTextureComponent() = default;

		void SetTexture(const std::string& texturePath) { m_pTexture = dae::ResourceManager::GetInstance().LoadTexture(texturePath); }
		void SetTexture(std::shared_ptr<dae::Texture2D> texture) { m_pTexture = texture; }

		void Render() const;

	private:
		std::shared_ptr<dae::Texture2D> m_pTexture{ nullptr };
		const EntityId m_Id;
	};
}

#endif