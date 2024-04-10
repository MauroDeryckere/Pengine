#ifndef PENGIN_UUID
#define PENGIN_UUID

#include "CoreIncludes.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp> 
#include <boost/uuid/uuid_io.hpp> 

namespace Pengin
{
	class UUID final
	{
	public:
		UUID() noexcept;
		UUID(const std::string& id, bool isPrettyStr = true) noexcept;

		~UUID() = default;

		const std::string GetUUID_Str() const noexcept;
		const std::string GetUUID_PrettyStr() const noexcept;

		bool operator==(const UUID& other) const noexcept
		{
			return m_UUID == other.m_UUID;
		}

	private:
		boost::uuids::uuid m_UUID;
	};
}

namespace std
{
	template<>
	struct hash<Pengin::UUID>
	{
		//TODO swap to the boost hash func
		size_t operator()(const Pengin::UUID& UUID) const noexcept
		{
			return hash<std::string>()(UUID.GetUUID_PrettyStr());
		}
	};
}

#endif