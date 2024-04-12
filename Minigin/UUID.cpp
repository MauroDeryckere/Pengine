#include "UUID.h"

#include <cassert>
#include <array>

thread_local static boost::uuids::random_generator s_NumGen;
thread_local static boost::uuids::string_generator s_StrGen;

namespace Pengin 
{
	UUID::UUID() noexcept:
		m_UUID{ s_NumGen() }
	{

	}

	UUID::UUID(const std::string& id, bool isPrettyStr) noexcept
	{ 
		if (isPrettyStr)
		{
			m_UUID = s_StrGen(id);
		}
		else
		{
			assert(id.size() == 16);
			for (size_t i = 0; i < 16; ++i) 
			{
				m_UUID.data[i] = static_cast<uint8_t>(id.data()[i]);
			}
		}
	}

	const std::string UUID::GetUUID_PrettyStr() const noexcept
	{
		return to_string(m_UUID);
	}

	const std::vector<uint8_t> UUID::GetUUID_Bytes() const noexcept 
	{
		std::vector<uint8_t> v(m_UUID.size());
		std::copy(m_UUID.begin(), m_UUID.end(), v.begin());

		return v;
	}
}