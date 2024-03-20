#ifndef OBSERVER
#define OBSERVER

#include <string>
#include <functional>
#include <memory>

namespace Pengin
{
	using fEventCallback = std::function<void(const void* pEventData)>;

	class Observer final : public std::enable_shared_from_this<Observer>
	{
	public:
		Observer(const std::string& name = "") :
			m_Name{ name } {}

		~Observer() = default;

		void RegisterForEvent(const std::string& eventName, fEventCallback fCallBack);

		const std::string& GetName() const { return m_Name; }

		Observer(const Observer&) = delete;
		Observer(Observer&&) = delete;
		Observer& operator=(const Observer&) = delete;
		Observer& operator=(const Observer&&) = delete;

	private:
		//Name can be useful for debugging purposes but isn't used past that
		const std::string m_Name;
	};
}

#endif