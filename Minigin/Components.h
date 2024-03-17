#ifndef COMPONENTS
#define COMPONENTS

#include "Texture2D.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Time.h"

#include "glm/glm.hpp"

#include <iostream>
#include <functional>

namespace Pengin
{
	struct TransformComponent 
	{
		TransformComponent() = default;
		TransformComponent(const glm::vec3& pos) : m_Position{ pos } { std::cout << "pos" << pos.x << "\n"; }

		glm::vec3 m_Position;
	};

	class TextureComponent final
	{
	public:
		TextureComponent(const TransformComponent* transform) :
			m_Transform{ transform }
		{}

		TextureComponent(const std::string& texturePath, const TransformComponent* transform) :
			m_pTexture{ dae::ResourceManager::GetInstance().LoadTexture(texturePath) },
			m_Transform{transform}
		{
			std::cout << "in texture const" << transform->m_Position.x << "\t" << m_Transform->m_Position.x << "\n";
		}

		void SetTexture(const std::string& texturePath) { m_pTexture = dae::ResourceManager::GetInstance().LoadTexture(texturePath); }

		void Render() const
		{
			if (m_pTexture)
			{
				//std::cout << m_Transform.get().m_Position.x << " in texture \n";
				dae::Renderer::GetInstance().RenderTexture(*m_pTexture, m_Transform->m_Position.x, m_Transform->m_Position.y);
			}
		}

	private:
		std::shared_ptr<dae::Texture2D> m_pTexture;
		const TransformComponent* m_Transform;
	};

	class CharacterMovementComponent final
	{
	public:
		CharacterMovementComponent(float movementSpeed, TransformComponent& transform) :
			m_MovementSpeed{ movementSpeed },
			m_Transform{ transform }
		{}

		void Move(const glm::vec3& dir)
		{
			auto vel = dir * m_MovementSpeed;
			m_Transform.get().m_Position += (vel * Time::GetInstance().GetElapsedSec());
		}

	private:
		float m_MovementSpeed;

		std::reference_wrapper<TransformComponent> m_Transform;
	};
}

#endif