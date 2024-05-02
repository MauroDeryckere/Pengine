#ifndef PENGIN_SERVICE_LOCATOR
#define PENGIN_SERVICE_LOCATOR

#include "SoundSystem.h"
#include "Serializer.h"
#include "FieldSerializer.h"

#include <memory>

namespace Pengin
{
	class ServiceLocator final
	{
	public:
		[[nodiscard]] static SoundSystem& GetSoundSystem() { return (*m_pSoundSystem); }
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& pSoundSystem)
		{
			m_pSoundSystem = ( (!pSoundSystem) ? std::make_unique<NullSoundSystem>() : std::move(pSoundSystem) );
		}

		[[nodiscard]] static Serializer& GetSerializer() { return (*m_pSerializer); }
		static void RegisterSerializer(std::unique_ptr<Serializer>&& pSerializer)
		{
			m_pSerializer = ( (!pSerializer) ? std::make_unique<NullSerializer>() : std::move(pSerializer) );
		}

	private:
		static std::unique_ptr<SoundSystem> m_pSoundSystem;
		static std::unique_ptr<Serializer> m_pSerializer;
	};
}

#endif