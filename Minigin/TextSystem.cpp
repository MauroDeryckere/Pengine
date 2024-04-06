#include "TextSystem.h"

#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL_ttf.h> 

#include "ECS.h"
#include "EntityId.h"
#include "EventManager.h"

#include "TextComponent.h"
#include "SpriteComponent.h"
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
				const SDL_Color color{ entity.m_Color.w, entity.m_Color.x, entity.m_Color.y, entity.m_Color.z };
				const auto surf = TTF_RenderText_Blended(entity.m_pFont->GetFont(), entity.m_Text.c_str(), color);
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
				auto pTexture = std::make_shared<dae::Texture2D>(texture, "NO PATH"); //TODO

				auto& textureComp = m_ECS.GetComponent<SpriteComponent>(textComps.GetIdFromIterator(it));
				textureComp.m_pTexture = pTexture;

				entity.needsTextureChange = false;
			}

			++it;
		}
	}

	void TextSystem::SetText(const EntityId id, const std::string& text)
	{
		assert(m_ECS.HasComponent<SpriteComponent>(id));
		assert(m_ECS.HasComponent<TextComponent>(id));

		auto& textComp = m_ECS.GetComponent<TextComponent>(id);
		textComp.m_Text = text;

		const SDL_Color color{ textComp.m_Color.w, textComp.m_Color.x, textComp.m_Color.y, textComp.m_Color.z };
		const auto surf = TTF_RenderText_Blended(textComp.m_pFont->GetFont(), textComp.m_Text.c_str(), color);
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
		auto pTexture = std::make_shared<dae::Texture2D>(texture, "NO PATH"); //TODO

		auto& textureComp = m_ECS.GetComponent<SpriteComponent>(id);
		textureComp.m_pTexture = pTexture;

		textComp.needsTextureChange = false;
	}
}