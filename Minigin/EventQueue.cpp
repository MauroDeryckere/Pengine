#include "EventQueue.h"

#include "EventManager.h"

namespace Pengin
{
	void EventQueue::ProcessEvents()
	{
		while (!m_Queue.empty())
		{
			EventManager::GetInstance().ProcessEvent(m_Queue.front().first, m_Queue.front().second);
			m_Queue.pop();
		}
	}

	void EventQueue::AddEvent(const std::string& eventName, const void* eventData)
	{
		m_Queue.push({ eventName, eventData });
	}
}