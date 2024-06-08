#include <stdexcept>
#include <SDL_ttf.h>
#include "Font.h"

//Unregister in destructor TODO

TTF_Font* dae::Font::GetFont() const {
	return m_font;
}

dae::Font::Font(_TTF_Font* pFont, const std::string& path, unsigned fontSize) :
	m_font{ pFont },
	m_FontPath{ path },
	m_FontSize{ fontSize }
{

}

dae::Font::~Font()
{
	TTF_CloseFont(m_font);
}