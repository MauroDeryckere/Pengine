#ifndef TEXTURECOMPONENT
#define TEXTURECOMPONENT

#include "ECS.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "Texture2D.h"

namespace Pengin
{
	class TextureComponent final
	{
	public:
		TextureComponent(EntityId id) :
			m_Id{ id }
		{}
		TextureComponent(const std::string& texturePath, EntityId id) :
			m_pTexture{ dae::ResourceManager::GetInstance().LoadTexture(texturePath) },
			m_Id{ id }
		{}
		~TextureComponent() = default;

		void SetTexture(const std::string& texturePath) { m_pTexture = dae::ResourceManager::GetInstance().LoadTexture(texturePath); }
		void SetTexture(std::shared_ptr<dae::Texture2D> texture) { m_pTexture = texture; }

		void Render() const
		{
			//In future: move ID out, create view for comp types
			if (m_pTexture)
			{
				auto& ecs = ECS::GetInstance();
				const auto& tranform{ ecs.GetComponent<TransformComponent>(m_Id) };

				dae::Renderer::GetInstance().RenderTexture(*m_pTexture, tranform.m_Position.x, tranform.m_Position.y);
			}
		}
	private:
		std::shared_ptr<dae::Texture2D> m_pTexture{ nullptr };
		const EntityId m_Id;
	};
}

#endif