#include "TextComponent.h"

#include <stdexcept>
#include <SDL_ttf.h>
#include "GameObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"


dae::TextComponent::TextComponent(GameObject* pGameObj, std::shared_ptr<Font> pFont):
	TextureComponent(pGameObj),
	m_NeedsUpdate{true},
	m_pFont{std::move(pFont)},
	m_Text{""}
{
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::TextComponent::Update(float)
{
	if (m_NeedsUpdate)
	{
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_pTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	if (m_pTexture)
	{

		Renderer::GetInstance().RenderTexture(*m_pTexture, GetGameObj()->GetTransform().GetPosition().x, GetGameObj()->GetTransform().GetPosition().y);
	}
}
