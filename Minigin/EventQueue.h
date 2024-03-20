#ifndef EVENTQUEUE
#define EVENTQUEUE

#include <queue>
#include "Event.h"

namespace Pengin
{
	class EventQueue final
	{
	public:
		EventQueue() = default;
		~EventQueue() = default;

		void ProcessEvents();

		void AddEvent(const Event& event);
	
		EventQueue(const EventQueue&) = delete;
		EventQueue(EventQueue&&) = delete;
		EventQueue& operator=(const EventQueue&) = delete;
		EventQueue& operator=(const EventQueue&&) = delete;

	private:
		std::queue<Event> m_Queue;
	};
}

#endif