#include "EventQueue.h"

#include "EventManager.h"

namespace Pengin
{
	void EventQueue::ProcessEvents()
	{
		while (!m_Queue.empty())
		{
			EventManager::GetInstance().ProcessEvent(m_Queue.front());
			m_Queue.pop();
		}
	}

	void EventQueue::AddEvent(const Event& event)
	{
		m_Queue.push(event);
	}
}