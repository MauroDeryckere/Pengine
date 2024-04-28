#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_opengl.h>

#include <thread>

#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "GameTime.h"
#include "ImGUIWindow.h"

#include "ServiceLocator.h"
#include "FModSoundSystem.h"
#include "LoggingSoundSystem.h"

#include "ResourceManager.h"

#ifdef USE_STEAMWORKS
	#pragma warning (push)
	#pragma warning (disable: 4996)
		#include "steam_api.h"
	#pragma warning (pop)
#endif

SDL_Window* g_Window{};

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

	#ifdef SDL_HINT_IME_SHOW_UI
		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
	#endif

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	const SDL_WindowFlags windowFlags = static_cast<SDL_WindowFlags>( SDL_WINDOW_OPENGL
																	| SDL_WINDOW_RESIZABLE
																	| SDL_WINDOW_ALLOW_HIGHDPI
																	);
	g_Window = SDL_CreateWindow(
		"Pengine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		windowFlags
	);

	if (g_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_Window);
	//auto context = SDL_GL_CreateContext(g_window);
	//SDL_GL_MakeCurrent(g_window, context);
	//SDL_GL_SetSwapInterval(1); // Enable vsync

	ResourceManager::GetInstance().Init(dataPath);

	#ifdef DEBUG_MODE
		Pengin::ServiceLocator::RegisterSoundSystem(std::make_unique<Pengin::LoggingSoundSystem>(std::make_unique<Pengin::FModSoundSytem>()));
	#else
		Pengin::ServiceLocator::RegisterSoundSystem(std::make_unique<Pengin::FModSoundSytem>());
	#endif
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_Window);
	g_Window = nullptr;

	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& renderer{ Renderer::GetInstance() };
	auto& sceneManager{ Pengin::SceneManager::GetInstance() };
	auto& input{ Pengin::InputManager::GetInstance() };
	auto& time{ Pengin::GameTime::GetInstance() };
	
	bool doContinue{ true };

	while (doContinue)
	{
		time.Update();

		doContinue = input.ProcessInput();

		while (time.IsLag())
		{
			std::cout << "FIXED UPD\n";
			sceneManager.FixedUpdate();
			time.ProcessLag();
		}

		sceneManager.Update();
		renderer.Render();

		#ifdef USE_STEAMWORKS
			SteamAPI_RunCallbacks();
		#endif

		std::this_thread::sleep_for(time.GetSleepTime());
	}
}
