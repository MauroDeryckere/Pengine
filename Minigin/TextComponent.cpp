#include "TextComponent.h"
#include "Renderer.h"

//namespace Pengin
//{
//	void TextComponent::Update()
//	{
//		if (m_NeedsUpdate)
//		{
//			auto& textureComp = m_ECS.GetComponent<StaticTextureComponent>(id);
//
//			const auto surf = TTF_RenderText_Blended(textComp.m_pFont->GetFont(), textComp.m_Text.c_str(), textComp.m_Color);
//			if (surf == nullptr)
//			{
//				throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
//			}
//
//			auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
//			if (texture == nullptr)
//			{
//				throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
//			}
//
//			SDL_FreeSurface(surf);
//			auto pTexture = std::make_shared<dae::Texture2D>(texture);
//
//			textureComp.m_pTexture = pTexture;
//		}
//	}
//}
