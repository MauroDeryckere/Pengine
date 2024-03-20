#ifndef EVENTQUEUE
#define EVENTQUEUE

#include <queue>
#include <string>

namespace Pengin
{
	class EventQueue final
	{
	public:
		EventQueue() = default;
		~EventQueue() = default;

		void ProcessEvents();

		void AddEvent(const std::string& eventName, const void* eventData);
	
	private:
		std::queue<std::pair<std::string, const void*>> m_Queue;
	};
}

#endif