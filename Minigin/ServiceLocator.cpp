#include "ServiceLocator.h"

//Default intitialize systems in the service locator

namespace Pengin
{
	std::unique_ptr<SoundSystem> ServiceLocator::m_pSoundSystem{ std::make_unique<NullSoundSystem>() };
}
