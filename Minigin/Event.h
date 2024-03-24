#pragma once

#include <string>

namespace Pengin
{
	class Event final
	{
	public:
		Event(const std::string& eventName, const void* eventData) :
			m_EventName{ eventName },
			m_EventData{ eventData }
		{}

		~Event() = default;

		[[nodiscard]] const std::string& GetEventName() const { return m_EventName; }
		[[nodiscard]] const void* GetEventData() const { return m_EventData;  }
		
		//Event(const Event&) = delete;
		//Event(Event&&) = delete;
		//Event& operator=(const Event&) = delete;
		//Event& operator=(const Event&&) = delete;

	private:
		const std::string m_EventName;
		const void* m_EventData;
	};
}



