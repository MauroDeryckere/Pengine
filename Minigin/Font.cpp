#include <stdexcept>
#include <SDL_ttf.h>

#include "Font.h"
#include "ResourceManager.h"

namespace Pengin
{
	TTF_Font* Font::GetFont() const noexcept
	{
		return m_Font;
	}

	Font::Font(_TTF_Font* pFont, const std::string& path, unsigned fontSize) :
		m_Font{ pFont },
		m_FontPath{ path },
		m_FontSize{ fontSize }
	{

	}

	Font::~Font()
	{
		TTF_CloseFont(m_Font);
	}
}

