#ifndef RENDERCOMPONENT
#define RENDERCOMPONENT

#include "Component.h"

namespace dae
{
	class RenderComponent
		: public Component
	{
	public:
		RenderComponent(GameObject* pGameObj);
		virtual ~RenderComponent() override = default;

		virtual void Update(float) = 0;
		virtual void Render() const = 0;

		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

	private:

	};
}

#endif


//#pragma once
//#include <SDL.h>
//#include "Singleton.h"
//
//namespace dae
//{
//	class Texture2D;
//	/**
//	 * Simple RAII wrapper for the SDL renderer
//	 */
//	class Renderer final : public Singleton<Renderer>
//	{
//		SDL_Renderer* m_renderer{};
//		SDL_Window* m_window{};
//		SDL_Color m_clearColor{};
//	public:
//		void Init(SDL_Window* window);
//		void Render() const;
//		void Destroy();
//
//		void RenderTexture(const Texture2D& texture, float x, float y) const;
//		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
//
//		SDL_Renderer* GetSDLRenderer() const;
//
//		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
//		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
//	};
//}



