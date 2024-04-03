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

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);

	RenderTexture(texture, &dst, nullptr);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const Recti& dstRect, const Recti& srcRect) const
{
	SDL_Rect dst{ dstRect.x, dstRect.y, static_cast<int>(dstRect.width), static_cast<int>(dstRect.height) };

	if (srcRect == Recti{})
	{
		RenderTexture(texture, &dst, nullptr);
		return;
	}

	SDL_Rect src{ srcRect.x, srcRect.y, static_cast<int>(srcRect.width), static_cast<int>(srcRect.height) };
	RenderTexture(texture, &dst, &src);
}


void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{ static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height) };
	RenderTexture(texture, &dst, nullptr);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const 
{
	return m_renderer; 
}

void dae::Renderer::RenderTexture(const Texture2D& texture, SDL_Rect* pDstRect, SDL_Rect* pSrcRect) const
{
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), pSrcRect, pDstRect);
}
