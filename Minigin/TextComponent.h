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
			text{ text },
			pFont{ dae::ResourceManager::GetInstance().LoadFont(fontPath, fontSize) },
			color{ color },
			needsTextureChange{ true }
		{
			assert(fontSize > 0 && "Can not have fontsize 0");
		}

		~TextComponent() = default;

		void SetText(const std::string& newText, const glm::u8vec4 newColor = {}) //allows to change the text without having to set all variables manually
		{
			text = newText;
			needsTextureChange = true;

			if (color != glm::u8vec4{})
			{
				color = newColor;
			}
		}

		std::string text;
		std::shared_ptr<dae::Font> pFont;
		glm::u8vec4 color{ 255, 255, 255, 255 };

		bool needsTextureChange{ true };
	};
}

#endif