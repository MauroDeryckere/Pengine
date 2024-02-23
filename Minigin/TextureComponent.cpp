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
	//const auto& pos = m_transform.GetPosition();
	if (m_pTexture)
	{

		Renderer::GetInstance().RenderTexture(*m_pTexture, m_pGameObject->GetTransform().GetPosition().x, m_pGameObject->GetTransform().GetPosition().y);
	}
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}
