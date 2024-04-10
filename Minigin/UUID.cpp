#include "UUID.h"

#include <random>
#include <unordered_map>

#include <cassert>

namespace Pengin //TODO 128 but number
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	UUID::UUID() noexcept:
		m_UUID{ s_UniformDistribution(s_Engine) }
	{

	}

	UUID::UUID(const std::string& id, bool isPrettyStr) noexcept
	{ 
		if (isPrettyStr)
		{
			m_UUID = std::stoull(id);
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
		return std::to_string(m_UUID);
	}
}