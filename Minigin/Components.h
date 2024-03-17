#ifndef COMPONENTS
#define COMPONENTS

#include "Texture2D.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include "glm/glm.hpp"

namespace Pengin
{
	struct TransformComponent 
	{
		TransformComponent() = default;
		TransformComponent(const glm::vec3& pos) : m_Position{ pos } {}

		glm::vec3 m_Position;
	};

	struct TextureComponent
	{
		TextureComponent(TransformComponent& transform) :
			m_Transform{ transform } {}

		TextureComponent(const std::string& texturePath, TransformComponent& transform) :
			m_pTexture{ dae::ResourceManager::GetInstance().LoadTexture(texturePath) },
			m_Transform{ transform } {}

		void SetTexture(const std::string& texturePath) { m_pTexture = dae::ResourceManager::GetInstance().LoadTexture(texturePath); }

		void Render() const
		{
			if (m_pTexture)
			{
				dae::Renderer::GetInstance().RenderTexture(*m_pTexture, m_Transform.get().m_Position.x, m_Transform.get().m_Position.y);
			}
		}

		std::shared_ptr<dae::Texture2D> m_pTexture;
		std::reference_wrapper<TransformComponent> m_Transform;
	};
}

#endif