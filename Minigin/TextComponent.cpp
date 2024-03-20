#include "TextComponent.h"

#include "Renderer.h"
#include "TextureComponent.h"

#include <stdexcept>

namespace Pengin
{
	void TextComponent::Update()
	{
		if (m_NeedsUpdate)
		{
			const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);
			if (surf == nullptr)
			{
				throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
			}

			auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
			if (texture == nullptr)
			{
				throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
			}

			SDL_FreeSurface(surf);
			auto pTexture = std::make_shared<dae::Texture2D>(texture);
			m_NeedsUpdate = false;

			auto& textureComp{ ECS::GetInstance().GetComponent<TextureComponent>(m_Id) };
			textureComp.SetTexture(pTexture);
		}
	}
}