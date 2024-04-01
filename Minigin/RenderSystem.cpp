#include "RenderSystem.h"

#include "Renderer.h"
#include "ECS.h"

#include "TransformComponent.h"
#include "StaticTextureComponent.h"

namespace Pengin
{
	void RenderSystem::Render() const
	{
		auto& renderer{ dae::Renderer::GetInstance() };

		const auto& textureComponents{ m_ECS.GetComponents<StaticTextureComponent>() };

		for (auto it{ std::begin(textureComponents) }; auto& entity : textureComponents)
		{
			auto entityId{ textureComponents.GetIdFromIterator(it) };

			auto& pTexture = entity.m_pTexture;

			if (pTexture)
			{
				auto& transform = m_ECS.GetComponent<TransformComponent>(entityId);
				renderer.RenderTexture((*pTexture), transform.worldPos.x, transform.worldPos.y);
			}

			++it;
		}
	}
}

