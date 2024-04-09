#ifndef PENGIN_UUID
#define PENGIN_UUID

#include <xhash>

#pragma warning (push)
	#pragma warning (disable: 4244)
	#include <uuid_v4.h>
#pragma warning (pop)

namespace Pengin
{
	class UUID final
	{
	public:
		UUID() noexcept;
		UUID(const UUIDv4::UUID& id) noexcept;
		UUID(const std::string& id, bool isPrettyStr = true) noexcept;

		const std::string GetUUID_Str() const noexcept;
		const std::string GetUUID_PrettyStr() const noexcept;

		~UUID() = default;

		bool operator==(const UUID& other) const noexcept
		{
			return m_UUID == other.m_UUID;
		}

	private:
		UUIDv4::UUID m_UUID;
	};
}

namespace std
{
	template<>
	struct hash<Pengin::UUID>
	{
		size_t operator()(const Pengin::UUID& UUID) const noexcept
		{
			//return UUIDv4::UUID{ UUID.GetUUID_Str() }.hash();
			return hash<std::string>()(UUID.GetUUID_Str());
		}
	};
}

#endif