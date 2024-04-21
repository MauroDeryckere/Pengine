#ifndef PENGIN_IMGUIWINDOW
#define PENGIN_IMGUIWINDOW

typedef union SDL_Event SDL_Event;
struct SDL_Window;

namespace Pengin
{
	class ImGUIWindow final
	{
	public:
		ImGUIWindow() = default;
		~ImGUIWindow() = default;

		void Create(SDL_Window* pWindow);
		void Destroy();

		void ProcessEvent(SDL_Event& event);

		void BeginRender() const;
		void EndRender(SDL_Window* pWindow) const;

		[[nodiscard]] bool UsedMouse() const noexcept;
		[[nodiscard]] bool UsedKeyboard() const noexcept;

		ImGUIWindow(const ImGUIWindow&) = delete;
		ImGUIWindow(ImGUIWindow&&) = delete;
		ImGUIWindow& operator=(const ImGUIWindow&) = delete;
		ImGUIWindow& operator=(const ImGUIWindow&&) = delete;

	private:
		void SetupImGuiStyle(float alpha = 1.f);
	};
}

#endif