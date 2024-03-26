#pragma once
#include <SDL.h>
#include "Singleton.h"

#include "UtilStructs.h"

namespace dae
{
	using namespace Pengin::UtilStructs;

	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, const Recti& dstRect, const Recti& srcRect = {}) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

		[[nodiscard]] SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }

	private:
		void RenderTexture(const Texture2D& texture, SDL_Rect* pDstRect, SDL_Rect* pSrcRect) const;
	};
}

