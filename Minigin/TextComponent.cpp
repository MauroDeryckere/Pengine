#include "TextComponent.h"

#include "GameObject.h"
#include "TextureComponent.h"

#include <stdexcept>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"


dae::TextComponent::TextComponent(GameObject* pGameObj, std::shared_ptr<Font> pFont):
	FunctionalComponent(pGameObj),
	m_NeedsUpdate{true},
	m_pFont{std::move(pFont)},
	m_Text{""},
	m_pTextureComponent{pGameObj->GetComponent<TextureComponent>() }
{
	assert(m_pTextureComponent);
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::TextComponent::Update()
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
		auto pTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;

		m_pTextureComponent->SetTexture(pTexture);
	}
}

