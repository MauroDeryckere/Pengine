#include "ThreadManager.h"

namespace Pengin
{
	void ThreadManager::Start()
	{
		m_IsRunning = true;

		m_SoundThread = 
		std::jthread([this]() 
		{
			while (true)
			{
				std::function<void()> task{};

				//Scope for lock
				{
					std::unique_lock<std::mutex> lock(m_Mutex);
					m_SoundTaskCondition.wait(lock, [this] { return !m_IsRunning || !m_SoundTasks.empty(); });

					if (!m_SoundTasks.empty())
					{
						task = std::move(m_SoundTasks.front());
						m_SoundTasks.pop();
					}
					else if (!m_IsRunning)
					{
						break;
					}
				}

				if (task)
				{
					task();
				}
			}
		});
	}

	void ThreadManager::Stop()
	{
		m_IsRunning = false;

		m_SoundTaskCondition.notify_one();
		if (m_SoundThread.joinable())
		{
			m_SoundThread.join();
		}
	}

	void ThreadManager::EnqueueSoundTask(const std::function<void()>& task)
	{
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_SoundTasks.push(task);
		}
		m_SoundTaskCondition.notify_one();
	}
}
