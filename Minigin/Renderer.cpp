#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"

#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl2.h>

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	m_ImGUIWindow.Create(m_window);
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	Pengin::SceneManager::GetInstance().Render();

	SDL_RenderFlush(m_renderer);

	m_ImGUIWindow.BeginRender();
		Pengin::SceneManager::GetInstance().RenderImGUI();
		//ImGui::ShowDemoWindow();
	m_ImGUIWindow.EndRender(m_window);

	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
	m_ImGUIWindow.Destroy();
}

void dae::Renderer::RenderTexture(const Pengin::Texture2D& texture, int x, int y, const glm::vec3& scale, Rectu16 srcRect) const
{
	SDL_Rect dst;
	const bool isSrcRect{ srcRect };

	if (isSrcRect)
	{
		dst = SDL_Rect{x, y, static_cast<int>(srcRect.width), static_cast<int>(srcRect.height) };
	}
	else
	{
		dst.x = static_cast<int>(x);
		dst.y = static_cast<int>(y);
		SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	}
	
	dst.w *= static_cast<int>(scale.x);
	dst.h *= static_cast<int>(scale.y);

	if (isSrcRect)
	{
		SDL_Rect src{ srcRect.x, srcRect.y, static_cast<int>(srcRect.width), static_cast<int>(srcRect.height) };
		RenderTexture(texture, &dst, &src);
		return;
	}

	RenderTexture(texture, &dst, nullptr);
}

void dae::Renderer::RenderTexture(const Pengin::Texture2D& texture, const Recti& dstRect, Rectu16 srcRect) const
{
	SDL_Rect dst{ dstRect.x, dstRect.y, dstRect.width, dstRect.height };

	if (!srcRect)
	{
		RenderTexture(texture, &dst, nullptr);
		return;
	}

	SDL_Rect src{ srcRect.x, srcRect.y, srcRect.width ,srcRect.height };
	RenderTexture(texture, &dst, &src);
}

void dae::Renderer::RenderTexture(const Pengin::Texture2D& texture, int x, int y, uint16_t width, uint16_t height, const glm::vec3& scale, Rectu16 srcRect) const
{
	SDL_Rect dst{ x, y, width, height };

	dst.w *= static_cast<int>(scale.x);
	dst.h *= static_cast<int>(scale.y);

	if (!srcRect)
	{
		RenderTexture(texture, &dst, nullptr);
		return;
	}

	SDL_Rect src{ srcRect.x, srcRect.y, srcRect.width ,srcRect.height };
	RenderTexture(texture, &dst, &src);
}

void dae::Renderer::DrawLine(int x1, int y1, int x2, int y2, SDL_Color color) const
{
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2);
}


void dae::Renderer::FillRect(Rect16 dst, SDL_Color color) const
{
	SDL_Rect rect = { dst.x, dst.y, dst.width, dst.height };

	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(m_renderer, &rect);
}

void dae::Renderer::DrawRect(Rect16 dst, SDL_Color color) const
{
	SDL_Rect rect = { dst.x, dst.y, dst.width, dst.height };

	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(m_renderer, &rect);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const 
{
	return m_renderer; 
}

void dae::Renderer::RenderTexture(const Pengin::Texture2D& texture, SDL_Rect* pDstRect, SDL_Rect* pSrcRect) const
{
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), pSrcRect, pDstRect);
}
