#pragma once
#include "ImGUIWindow.h"

#include <SDL.h>
#include "Singleton.h"

#include "UtilStructs.h"
#include "glm/vec3.hpp"

#include "Texture2D.h" 

namespace dae
{
	using namespace Pengin::UtilStructs;

	//class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Pengin::Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	

		Pengin::ImGUIWindow m_ImGUIWindow{};

	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, int x, int y, const glm::vec3& scale, Rectu16 srcRect = {}) const;
		void RenderTexture(const Texture2D& texture, const Recti& dstRect, Rectu16 srcRect = {}) const;
		void RenderTexture(const Texture2D& texture, int x, int y, uint16_t width, uint16_t height, const glm::vec3& scale, Rectu16 srcRect = {}) const;

		//Debug rendering functions
		void DrawLine(int x1, int y1, int x2, int y2, SDL_Color color) const;

		void FillRect(Rect16 dst, SDL_Color color) const;
		void DrawRect(Rect16 dst, SDL_Color color) const;

		[[nodiscard]] SDL_Renderer* GetSDLRenderer() const;
		[[nodiscard]] Pengin::ImGUIWindow& GetImGUIWindow() { return m_ImGUIWindow; }
		[[nodiscard]] SDL_Window* GetSDLWindow() { return m_window; }

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(const Renderer&&) = delete;

	private:
		friend class Pengin::Singleton<Renderer>;
		Renderer() = default;
		~Renderer() = default;

		void RenderTexture(const Texture2D& texture, SDL_Rect* pDstRect, SDL_Rect* pSrcRect) const;
	};
}

