#include "GameUUID.h"

#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/uuid/uuid_hash.hpp"

#include <cassert>
#include <array>

thread_local static boost::uuids::random_generator s_NumGen;
thread_local static boost::uuids::string_generator s_StrGen;
thread_local static boost::uuids::nil_generator s_NilGen;

namespace Pengin 
{
	GameUUID::GameUUID(bool isNull) noexcept:
		m_UUID{ isNull ? s_NilGen() : s_NumGen() }
	{}

	GameUUID::GameUUID(const std::string& id, bool isPrettyStr) noexcept
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

	const std::string GameUUID::GetUUID_PrettyStr() const noexcept
	{
		return to_string(m_UUID);
	}

	const std::vector<uint8_t> GameUUID::GetUUID_Bytes() const noexcept 
	{
		std::vector<uint8_t> v(m_UUID.size());
		std::copy(m_UUID.begin(), m_UUID.end(), v.begin());

		return v;
	}
}