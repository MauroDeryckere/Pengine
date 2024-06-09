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
		BaseSystem{ },
		m_ECS{ecs},
		m_pObserver{ EventManager::GetInstance().CreateObserver() }
	{
		//Could use an event internally for text change
		//m_pObserver->RegisterForEvent(m_pObserver, "OnTextChangeEvent", [this](const BaseEvent& event) { std::cout << "text change event\n";  event; });
	}

	void TextSystem::Update()
	{
		auto textComps = m_ECS.GetComponents<TextComponent>();
		for (auto it = textComps.begin() ;auto& entity : textComps)
		{
			if (entity.needsTextureChange)
			{
				const SDL_Color color{ entity.color.x, entity.color.y, entity.color.z, entity.color.w };
				const auto surf = TTF_RenderText_Blended(entity.pFont->GetFont(), entity.text.c_str(), color);
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
				auto pTexture = std::make_shared<Texture2D>(texture, "NO PATH");

				auto& textureComp = m_ECS.GetComponent<SpriteComponent>(textComps.GetIdFromIterator(it));
				textureComp.pTexture = pTexture;

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
		textComp.text = text;

		const SDL_Color color{ textComp.color.x, textComp.color.y, textComp.color.z, textComp.color.w };
		const auto surf = TTF_RenderText_Blended(textComp.pFont->GetFont(), textComp.text.c_str(), color);
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
		auto pTexture = std::make_shared<Texture2D>(texture, "NO PATH");

		auto& textureComp = m_ECS.GetComponent<SpriteComponent>(id);
		textureComp.pTexture = pTexture;

		textComp.needsTextureChange = false;
	}
}