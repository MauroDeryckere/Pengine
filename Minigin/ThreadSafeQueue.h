#ifndef PENGIN_THREADSAFEQUEUE
#define PENGIN_THREADSAFEQUEUE

#include <queue>
#include <mutex>
#include <condition_variable>

namespace Pengin
{
    template<typename T>
    requires std::is_move_assignable_v<T>
    class ThreadSafeQueue final
    {
    public:
        ThreadSafeQueue() = default;
        ~ThreadSafeQueue() = default;

        template<typename U>
        requires std::is_convertible_v<U, T>
        void Push(U&& value) noexcept
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Queue.push(std::forward<U>(value));
            m_ConditionVariable.notify_one();
        }

        template<typename... Args>
        requires std::is_constructible_v<T, Args...>
        void Emplace(Args&&... args) noexcept
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Queue.emplace(std::forward<Args>(args)...);
            m_ConditionVariable.notify_one();
        }

        [[nodiscard]] bool TryPop(T& value) noexcept
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

        void WaitAndPop(T& value) noexcept
        {
            std::unique_lock<std::mutex> lock(m_Mutex);

            m_ConditionVariable.wait(lock, [this] { return !m_Queue.empty(); });
            value = std::move(m_Queue.front());
            m_Queue.pop();
        }

        [[nodiscard]] bool Empty() const noexcept
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            return m_Queue.empty();
        }

        [[nodiscard]] size_t Size() const noexcept
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            return m_Queue.size();
        }

        void Clear() noexcept
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Queue = std::queue<T>{ };
            m_ConditionVariable.notify_all();
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