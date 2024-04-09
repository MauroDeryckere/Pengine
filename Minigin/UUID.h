#ifndef PENGIN_UUID
#define PENGIN_UUID

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
			return hash<std::string>()(UUID.GetUUID_Str());
		}
	};
}

#endif