#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

#include <iostream>

dae::TextureComponent::TextureComponent(GameObject* pGameObj):
	RenderComponent(pGameObj)
{
	std::cout << "crreated text comp\n";
}

dae::TextureComponent::~TextureComponent()
{
	std::cout << "deleted the texture comp\n";
}

void dae::TextureComponent::Update(float)
{

}

void dae::TextureComponent::Render() const
{
	//const auto& pos = m_transform.GetPosition();

	std::cout << "IN THIS FUNCTION \n\n";
	Renderer::GetInstance().RenderTexture(*m_texture, m_pGameObject->GetTransform().GetPosition().x, m_pGameObject->GetTransform().GetPosition().y);
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	std::cout << "Set the texture \n";
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}
