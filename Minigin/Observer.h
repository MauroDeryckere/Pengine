#ifndef OBSERVER
#define OBSERVER

#include <string>
#include <functional>
#include <memory>

namespace Pengin
{
	class Observer final : public std::enable_shared_from_this<Observer>
	{
	public:
		Observer(const std::string& name) :
			m_Name{ name } {}

		void RegisterForEvent(const std::string& eventName, std::function<void(const void* pEventData)> fCallBack);

		const std::string& GetName() const { return m_Name; }

	private:
		const std::string m_Name;
	};
}

#endif