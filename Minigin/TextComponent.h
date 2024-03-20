#ifndef TEXTCOMPONENT
#define TEXTCOMPONENT

#include "ECS.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "Font.h"
#include "Renderer.h"

#include <string>
#include <memory>

#include <stdexcept>
#include <SDL_ttf.h>

namespace Pengin
{
	class TextComponent final
	{
	public:
		TextComponent(EntityId id, const std::string& fontPath, unsigned fontSize, const std::string& text = "<EMPTY STRING>") :
			m_Id{ id },
			m_pFont{ dae::ResourceManager::GetInstance().LoadFont(fontPath, fontSize) },
			m_Text{ text }
		{}

		~TextComponent() = default;

		void Update()
		{
			if (m_NeedsUpdate)
			{
				const SDL_Color color = { 255,255,255,255 }; // only white text is supported now

				const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), color);
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

				auto& textureComp{ Pengin::ECS::GetInstance().GetComponent<TextureComponent>(m_Id) };
				textureComp.SetTexture(pTexture);
			}
		}

		void SetText(const std::string& text)
		{
			m_Text = text;
			m_NeedsUpdate = true;
		}

		const std::string& GetText() const { return m_Text; }

	private:
		bool m_NeedsUpdate{ true };
		std::string m_Text;

		std::shared_ptr<dae::Font> m_pFont;
		const EntityId m_Id;
	};
}

#endif