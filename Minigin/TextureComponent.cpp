#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

#include <iostream>

dae::TextureComponent::TextureComponent(GameObject* pGameObj):
	RenderComponent(pGameObj),
	m_pTexture{nullptr}
{
}

void dae::TextureComponent::Update(float)
{

}

void dae::TextureComponent::Render() const
{
	if (m_pTexture)
	{
		auto& worldPos{ GetGameObj()->GetWorldPosition() };
		Renderer::GetInstance().RenderTexture(*m_pTexture, worldPos.x, worldPos.y);
	}
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}
