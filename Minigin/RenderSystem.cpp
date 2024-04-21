#include "RenderSystem.h"

#include "Renderer.h"
#include "UtilStructs.h"
#include "ECS.h"

#include "TransformComponent.h"
#include "SpriteComponent.h"

namespace Pengin
{
	void RenderSystem::Render() const
	{
		auto& renderer{ dae::Renderer::GetInstance() };

		const auto& textureComponents{ m_ECS.GetComponents<SpriteComponent>() };

		for (auto it{ std::begin(textureComponents) }; auto& entity : textureComponents)
		{
			auto entityId{ textureComponents.GetIdFromIterator(it) };

			auto& pTexture = entity.pTexture;

			if (pTexture && entity.isVisible)
			{
				auto& transform = m_ECS.GetComponent<TransformComponent>(entityId);
				renderer.RenderTexture((*pTexture), static_cast<int>(transform.worldPos.x), static_cast<int>(transform.worldPos.y), transform.scale, entity.sourceRect);
			}

			++it;
		}
	}
}

