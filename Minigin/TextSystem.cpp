#include "TextSystem.h"

#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL_ttf.h> 

#include "ECS.h"
#include "EntityId.h"
#include "EventManager.h"

#include "TextComponent.h"
#include "StaticTextureComponent.h"
#include "Texture2D.h"

#include <stdexcept>

namespace Pengin
{
	TextSystem::TextSystem(ECS& ecs):
		m_ECS{ecs},
		m_pObserver{ EventManager::GetInstance().CreateObserver() }
	{
		m_pObserver->RegisterForEvent(m_pObserver, "OnTextChangeEvent", [this](const void* eventData) {eventData; });
	}



	void TextSystem::Update()
	{
		auto textComps = m_ECS.GetComponents<TextComponent>();
		for (auto it = textComps.begin() ;auto& entity : textComps)
		{
			if (entity.needsTextureChange)
			{
				const auto surf = TTF_RenderText_Blended(entity.m_pFont->GetFont(), entity.m_Text.c_str(), entity.m_Color);
				if (!surf)
				{
					throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
				}

				auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
				if (!texture)
				{
					throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
				}

				SDL_FreeSurface(surf);
				auto pTexture = std::make_shared<dae::Texture2D>(texture);

				auto& textureComp = m_ECS.GetComponent<StaticTextureComponent>(textComps.GetIdFromIterator(it));
				textureComp.m_pTexture = pTexture;

				entity.needsTextureChange = false;
			}

			++it;
		}
	}

	void TextSystem::SetText(const EntityId id, const std::string& text)
	{
		assert(m_ECS.HasComponent<StaticTextureComponent>(id));
		assert(m_ECS.HasComponent<TextComponent>(id));

		auto& textComp = m_ECS.GetComponent<TextComponent>(id);
		textComp.m_Text = text;

		const auto surf = TTF_RenderText_Blended(textComp.m_pFont->GetFont(), textComp.m_Text.c_str(), textComp.m_Color);
		if (!surf)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}

		auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
		if (!texture)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}

		SDL_FreeSurface(surf);
		auto pTexture = std::make_shared<dae::Texture2D>(texture);

		auto& textureComp = m_ECS.GetComponent<StaticTextureComponent>(id);
		textureComp.m_pTexture = pTexture;
	}
}