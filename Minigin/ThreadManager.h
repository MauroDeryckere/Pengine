#ifndef PENGIN_THREADMANAGER
#define PENGIN_THREADMANAGER

#include "Singleton.h"

#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace Pengin
{
	class ThreadManager final : public Singleton<ThreadManager>
	{
	public:
		void Start();
		void Stop();

		void EnqueueSoundTask(const std::function<void()>& task);

		ThreadManager(const ThreadManager&) = delete;
		ThreadManager(ThreadManager&&) = delete;
		ThreadManager& operator=(const ThreadManager&) = delete;
		ThreadManager& operator=(ThreadManager&&) = delete;
	
	private:
		friend class Singleton<ThreadManager>;
		ThreadManager() = default;
		~ThreadManager() = default;

		std::jthread m_SoundThread;
		std::queue<std::function<void()>> m_SoundTasks;
		std::condition_variable m_SoundTaskCondition;
		mutable std::mutex m_Mutex;

		bool m_IsRunning{ false };
	};
}

#endif