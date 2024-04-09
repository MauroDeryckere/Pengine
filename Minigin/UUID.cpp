#include "UUID.h"

namespace Pengin
{
	static UUIDv4::UUIDGenerator<std::mt19937_64> s_UUIDGenerator;

	UUID::UUID() noexcept:
		m_UUID{ s_UUIDGenerator.getUUID() }
	{ }

	UUID::UUID(const UUIDv4::UUID& id) noexcept :
		m_UUID{ id }
	{ }

	UUID::UUID(const std::string& id, bool isPrettyStr) noexcept
	{ 
		if (isPrettyStr)
		{
			m_UUID = UUIDv4::UUID::fromStrFactory(id);
		}
		else
		{
			m_UUID = UUIDv4::UUID{ id };
		}
	}

	const std::string UUID::GetUUID_Str() const noexcept
	{
		const std::string bytes{ m_UUID.bytes() };
		return bytes;
	}
	const std::string UUID::GetUUID_PrettyStr() const noexcept
	{
		return m_UUID.str();
	}
}