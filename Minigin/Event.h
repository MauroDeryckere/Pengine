#ifndef PENGIN_EVENT
#define PENGIN_EVENT

#include <string>
#include "SoundData.h"

namespace Pengin
{
	class BaseEvent
	{
	public:
		BaseEvent(const std::string& eventName) :
			m_EventName{ eventName }
		{}

		virtual ~BaseEvent() = default;

		[[nodiscard]] const std::string& GetEventName() const noexcept { return m_EventName; }
		
		BaseEvent(const BaseEvent&) = delete;
		BaseEvent(BaseEvent&&) = delete;
		BaseEvent& operator=(const BaseEvent&) = delete;
		BaseEvent& operator=(const BaseEvent&&) = delete;

	private:
		const std::string m_EventName;
	};

	//Engine defined sound event
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