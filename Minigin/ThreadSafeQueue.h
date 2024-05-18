#ifndef PENGIN_THREADSAFEQUEUE
#define PENGIN_THREADSAFEQUEUE

#include <queue>
#include <mutex>
#include <condition_variable>

namespace Pengin
{
    template<typename T>
    class ThreadSafeQueue final
    {
    public:
        ThreadSafeQueue() = default;
        ~ThreadSafeQueue() = default;

        void Push(T value) 
        {
            std::lock_guard<std::mutex> lock(m_Mutex);

            m_Queue.push(std::move(value));
            m_ConditionVariable.notify_one();
        }

        bool TryPop(T& value) 
        {
            std::lock_guard<std::mutex> lock(m_Mutex);

            if (m_Queue.empty()) 
            {
                return false;
            }

            value = std::move(m_Queue.front());
            m_Queue.pop();

            return true;
        }

        void WaitAndPop(T& value) 
        {
            std::unique_lock<std::mutex> lock(m_Mutex);

            m_ConditionVariable.wait(lock, [this] { return !m_Queue.empty(); });
            value = std::move(m_Queue.front());
            m_Queue.pop();
        }

        ThreadSafeQueue(const ThreadSafeQueue&) = delete;
        ThreadSafeQueue(ThreadSafeQueue&&) = delete;
        ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;
        ThreadSafeQueue& operator=(const ThreadSafeQueue&&) = delete;

    private:
        std::queue<T> m_Queue;
        mutable std::mutex m_Mutex;
        std::condition_variable m_ConditionVariable;
    };
}



#endif