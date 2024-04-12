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

		const std::string GetUUID_PrettyStr() const noexcept;
		const std::vector<uint8_t> GetUUID_Bytes() const noexcept;

		bool operator==(const UUID& other) const noexcept
		{
			return m_UUID == other.m_UUID;
		}

	private:
		friend struct std::hash<UUID>;

		boost::uuids::uuid m_UUID;
	};
}

namespace std
{
	template<>
	struct hash<Pengin::UUID>
	{
		size_t operator()(const Pengin::UUID& UUID) const noexcept
		{
			return boost::uuids::hash_value(UUID.m_UUID);
		}
	};
}

#endif