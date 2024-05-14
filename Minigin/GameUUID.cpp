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
	const GameUUID GameUUID::INVALID_UUID{ true };

	GameUUID::GameUUID(bool isNull):
		m_UUID{ isNull ? s_NilGen() : s_NumGen() }
	{ }

	GameUUID::GameUUID(const std::string& id, bool isPrettyStr)
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
		return boost::uuids::to_string(m_UUID);
	}

	const std::vector<uint8_t> GameUUID::GetUUID_Bytes() const noexcept 
	{
		std::vector<uint8_t> v(m_UUID.size());
		std::copy(m_UUID.begin(), m_UUID.end(), v.begin());

		return v;
	}

	GameUUID::operator bool() const noexcept
	{
		return !m_UUID.is_nil();
		
	}

	bool GameUUID::operator==(const GameUUID& other) const noexcept
	{
		return m_UUID == other.m_UUID;
	}

	size_t GameUUID::Hash() const noexcept
	{
		return boost::uuids::hash_value(m_UUID);
	}
}