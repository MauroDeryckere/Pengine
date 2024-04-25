#ifndef PENGIN_SERVICE_LOCATOR
#define PENGIN_SERVICE_LOCATOR

#include "SoundSystem.h"
#include <memory>

namespace Pengin
{
	class NullSoundSystem final : public SoundSystem
	{
	public:
		NullSoundSystem() = default;
		virtual ~NullSoundSystem() override = default;

		virtual void Update() {}
		virtual void LoadSound(const std::string&, bool, bool, bool) {}
		virtual void UnLoadSound(const std::string&) {}

		virtual const int32_t PlaySounds(const std::string&, const glm::vec3&, float) { return -1; }

		virtual void SetChannel3DPosition(int32_t, const glm::vec3&) {}
		virtual void SetChannelVolume(int32_t, float) {}

		NullSoundSystem(const NullSoundSystem&) = delete;
		NullSoundSystem(NullSoundSystem&&) = delete;
		NullSoundSystem& operator=(const NullSoundSystem&) = delete;
		NullSoundSystem& operator=(const NullSoundSystem&&) = delete;
	};

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