#ifndef TEXTCOMPONENT
#define TEXTCOMPONENT

#include "ECS.h"
#include "ResourceManager.h"

#include "Font.h"

#include <string>
#include <memory>
#include <SDL_ttf.h>

namespace Pengin
{
	struct TextComponent final
	{
		TextComponent(const std::string& fontPath, unsigned fontSize, const std::string& text = "<EMPTY STRING>", SDL_Color color = {255, 255, 255, 255}) :
			m_Text{ text },
			m_pFont{ dae::ResourceManager::GetInstance().LoadFont(fontPath, fontSize) },
			m_Color{ color },
			needsTextureChange{ true }
		{}

		~TextComponent() = default;

		std::string m_Text;
		std::shared_ptr<dae::Font> m_pFont;
		SDL_Color m_Color{ 255,255,255,255 };

		bool needsTextureChange{ true };
	};
}

#endif