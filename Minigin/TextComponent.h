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
	class TextComponent final
	{
	public:
		TextComponent(EntityId id, const std::string& fontPath, unsigned fontSize, const std::string& text = "<EMPTY STRING>", SDL_Color color = {255,255,255,255}) :
			m_Text{ text },
			m_pFont{ dae::ResourceManager::GetInstance().LoadFont(fontPath, fontSize) },
			m_Id{ id },
			m_Color{ color }
		{}

		~TextComponent() = default;

		void Update();

		void SetText(const std::string& text)
		{
			m_Text = text;
			m_NeedsUpdate = true;
		}

		const std::string& GetText() const { return m_Text; }

	private:
		std::string m_Text;

		std::shared_ptr<dae::Font> m_pFont;
		const EntityId m_Id;

		const SDL_Color m_Color{ 255,255,255,255 };
		bool m_NeedsUpdate{ true };
	};
}

#endif