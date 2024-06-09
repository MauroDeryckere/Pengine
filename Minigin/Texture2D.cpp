#include <SDL.h>
#include "Texture2D.h"
#include <iostream>

namespace Pengin
{
	Texture2D::Texture2D(SDL_Texture* texture, const std::string& path) :
		m_texture{ texture },
		m_Path{ path }
	{ }

	Texture2D::~Texture2D()
	{
		SDL_DestroyTexture(m_texture);
	}

	glm::ivec2 Texture2D::GetSize() const noexcept
	{
		SDL_Rect dst;
		SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
		return { dst.w,dst.h };
	}

	SDL_Texture* Texture2D::GetSDLTexture() const noexcept
	{
		return m_texture;
	}
}




