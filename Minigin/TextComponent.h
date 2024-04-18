#ifndef TEXTCOMPONENT
#define TEXTCOMPONENT

#include "ResourceManager.h"

#include "Font.h"
#include "glm/vec4.hpp"

#include <string>
#include <memory>

namespace Pengin
{
	struct TextComponent final
	{
		TextComponent(const std::string& fontPath, unsigned fontSize, const std::string& text = "<EMPTY STRING>", const glm::u8vec4& color = {255, 255, 255, 255}) :
			m_Text{ text },
			m_pFont{ dae::ResourceManager::GetInstance().LoadFont(fontPath, fontSize) },
			m_Color{ color },
			needsTextureChange{ true }
		{
			assert(fontSize > 0 && "Can not have fontsize 0");
		}

		~TextComponent() = default;

		void SetText(const std::string& text, const glm::u8vec4 color = {}) //allows to change the text without having to set all variables manually
		{
			m_Text = text;
			needsTextureChange = true;

			if (color != glm::u8vec4{})
			{
				m_Color = color;
			}
		}

		std::string m_Text;
		std::shared_ptr<dae::Font> m_pFont;
		glm::u8vec4 m_Color{ 255, 255, 255, 255 };

		bool needsTextureChange{ true };
	};
}

#endif