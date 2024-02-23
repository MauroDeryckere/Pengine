#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <chrono>
#include <thread>

#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& renderer{ Renderer::GetInstance() };
	auto& sceneManager{ SceneManager::GetInstance() };
	auto& input{ InputManager::GetInstance() };
	
	using std::chrono::high_resolution_clock;
	using std::chrono::duration;

	constexpr auto msPerFrame{ 16.7f };
	constexpr auto fixedTimeStep{ 20.f };

	bool doContinue{ true };
	auto lastTime{ high_resolution_clock::now() };
	float lag{ 0.f };

	while (doContinue)
	{
		const auto currentTime{ high_resolution_clock::now() };
		const float deltaTime{ duration<float>(currentTime - lastTime).count() };
		lastTime = currentTime;
		lag += deltaTime;

		doContinue = input.ProcessInput();
		while (lag >= fixedTimeStep)
		{
			FixedUpdate(fixedTimeStep);
			lag -= fixedTimeStep;
		}

		Update(deltaTime, sceneManager);
		Render(renderer);

		const auto sleep_time{ currentTime + std::chrono::milliseconds(static_cast<long>(msPerFrame)) - high_resolution_clock::now() };
		std::this_thread::sleep_for(sleep_time);
	}

}

void dae::Minigin::FixedUpdate(float fixedTimeStep)
{
	fixedTimeStep;
}

void dae::Minigin::Update(float deltaTime, auto& u)
{
	u.Update(deltaTime);
}

void dae::Minigin::Render(auto& r) const
{
	r.Render();
}
