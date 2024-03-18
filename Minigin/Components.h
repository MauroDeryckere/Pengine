#ifndef COMPONENTS
#define COMPONENTS

#include "ECS.h"

#include "Texture2D.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Time.h"

#include "glm/glm.hpp"

#include <iostream>
#include <functional>
#include <memory>

namespace Pengin
{
	struct TransformComponent 
	{
		TransformComponent() = default;
		TransformComponent(const glm::vec3& pos) : m_Position{ pos } 
		{}
		
		glm::vec3 m_Position;
	};

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
		std::shared_ptr<dae::Texture2D> m_pTexture{nullptr};
		const EntityId m_Id;
	};

	class CharacterMovementComponent final
	{
	public:
		CharacterMovementComponent(float movementSpeed, EntityId id) :
			m_MovementSpeed{ movementSpeed },
			m_Id{ id }
		{}

		void Move(const glm::vec3& dir) const
		{
			//In future: move ID out, create view for comp types
			auto& tranform{ Pengin::ECS::GetInstance().GetComponent<TransformComponent>(m_Id) };
			auto vel = dir * m_MovementSpeed;
			tranform.m_Position += (vel * Time::GetInstance().GetElapsedSec());
		}

	private:
		const float m_MovementSpeed;
		const EntityId m_Id;
	};
}

#endif