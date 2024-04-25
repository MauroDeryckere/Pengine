#ifndef PENGIN_SOUNDEVENT
#define PENGIN_SOUNDEVENT

#include "Event.h"
#include "SoundData.h"

namespace Pengin
{
	class SoundEvent final : public BaseEvent
	{
	public:
		SoundEvent(const std::string& eventName, const SoundData& soundData) :
			BaseEvent{ eventName },
			m_SoundData{ soundData }
		{ }

		virtual ~SoundEvent() override = default;

		[[nodiscard]] const SoundData& GetSounData() const noexcept { return m_SoundData; }

		SoundEvent(const SoundEvent&) = delete;
		SoundEvent(SoundEvent&&) = delete;
		SoundEvent& operator=(const SoundEvent&) = delete;
		SoundEvent& operator=(const SoundEvent&&) = delete;

	private:
		SoundData m_SoundData;
	};
}

#endif