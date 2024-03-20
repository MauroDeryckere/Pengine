#ifndef EVENT
#define EVENT

#include <string>

namespace Pengin
{
	class Event final
	{
	public:
		Event(const std::string& eventName, const void* eventData = nullptr) :
			m_Name{ eventName },
			m_EventData{ eventData }
		{}

		~Event() = default;

		const std::string& GetName() const { return m_Name; }
		const void* GetData() const { return m_EventData; }

	private:
		const std::string m_Name;
		const void* m_EventData;
	};
}

#endif