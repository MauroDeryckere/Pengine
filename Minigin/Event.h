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
		
		//Event(const Event&) = delete;
		//Event(Event&&) = delete;
		//Event& operator=(const Event&) = delete;
		//Event& operator=(const Event&&) = delete;

	private:
		const std::string m_EventName;
	};


	template<typename EventDataType>
	class Event final : public BaseEvent
	{
	public:
		Event(const std::string& eventName, const EventDataType& eventData) :
			BaseEvent{ eventName },
			m_EventData{ eventData }
		{ }

		virtual ~Event() override = default;

		const EventDataType& GetEventData() const noexcept { return m_EventData; }

		//Event(const Event&) = delete;
		//Event(Event&&) = delete;
		//Event& operator=(const Event&) = delete;
		//Event& operator=(const Event&&) = delete;
	private:
		EventDataType m_EventData;
	};

	using NoParamEvent = BaseEvent;
	using SoundEvent = Event<SoundData>;
}

#endif