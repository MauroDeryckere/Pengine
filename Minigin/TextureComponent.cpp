#include "StaticTextureComponent.h"

#include "TransformComponent.h"

#include "Renderer.h"

namespace Pengin
{
	void StaticTextureComponent::Render() const
	{
		//In future: move ID out, create view for comp types (?)
		if (m_pTexture)
		{
			auto& ecs = ECS::GetInstance();
			const auto& tranform{ ecs.GetComponent<TransformComponent>(m_Id) };

			dae::Renderer::GetInstance().RenderTexture(*m_pTexture, 
														tranform.m_Position.x, 
														tranform.m_Position.y, 
														m_pTexture->GetSize().x * tranform.m_Scale.x,
														m_pTexture->GetSize().y * tranform.m_Scale.y);
		}
	}
}