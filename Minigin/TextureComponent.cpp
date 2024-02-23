#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

#include <iostream>

dae::TextureComponent::TextureComponent(GameObject* pGameObj):
	RenderComponent(pGameObj)
{
}

void dae::TextureComponent::Update(float)
{

}

void dae::TextureComponent::Render() const
{
	//const auto& pos = m_transform.GetPosition();

	Renderer::GetInstance().RenderTexture(*m_texture, m_pGameObject->GetTransform().GetPosition().x, m_pGameObject->GetTransform().GetPosition().y);
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}
