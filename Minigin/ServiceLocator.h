#ifndef PENGIN_SERVICE_LOCATOR
#define PENGIN_SERVICE_LOCATOR

#include "SoundSystem.h"
#include <memory>
#include <mutex>

namespace Pengin
{
	class ServiceLocator final
	{
	public:
		[[nodiscard]] static SoundSystem& GetSoundSystem() { return (*m_pSoundSystem); }
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& pSoundSystem)
		{
			m_pSoundSystem = ( (!pSoundSystem) ? std::make_unique<NullSoundSystem>() : std::move(pSoundSystem));
		}

	private:
		static std::unique_ptr<SoundSystem> m_pSoundSystem;
	};
}

#endif