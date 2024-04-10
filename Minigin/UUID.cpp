#include "UUID.h"

#include <cassert>

namespace Pengin 
{
	UUID::UUID() noexcept:
		m_UUID{ boost::uuids::random_generator()() }
	{

	}

	UUID::UUID(const std::string& id, bool isPrettyStr) noexcept
	{ 
		if (isPrettyStr)
		{
			m_UUID = boost::uuids::string_generator()(id);
		}
		else
		{
			//TODO
		}
	}

	const std::string UUID::GetUUID_Str() const noexcept //TODO
	{
		const std::string bytes{ };
		return bytes;
	}
	const std::string UUID::GetUUID_PrettyStr() const noexcept
	{
		return to_string(m_UUID);
	}
}