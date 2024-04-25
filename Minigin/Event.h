#ifndef PENGIN_EVENT
#define PENGIN_EVENT

#include <string>
namespace Pengin
{
	//the constructor is public here and not protected because it is possible to simply broadcast a BaseEvent (only eventName)
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
}

#endif