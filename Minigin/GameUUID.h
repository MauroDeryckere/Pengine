#ifndef PENGIN_UUID
#define PENGIN_UUID

//TODO can we get rid of the include dependency over here somehow

#include <string>
#include <vector>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_hash.hpp>

namespace Pengin
{
	class GameUUID final
	{
	public:
		GameUUID(bool isNull = false) noexcept;
		GameUUID(const char*) = delete;

		GameUUID(const std::string& id, bool isPrettyStr = true) noexcept;

		~GameUUID() = default;

		const std::string GetUUID_PrettyStr() const noexcept;
		const std::vector<uint8_t> GetUUID_Bytes() const noexcept;

		operator bool() const noexcept;
		bool operator==(const GameUUID& other) const noexcept;

		[[nodiscard]] size_t Hash() const noexcept;

	private:
		friend struct std::hash<GameUUID>;
		boost::uuids::uuid m_UUID;
	};
}

namespace std
{
	template<>
	struct hash<Pengin::GameUUID>
	{
		size_t operator()(const Pengin::GameUUID& gameUUID) const noexcept
		{
			return gameUUID.Hash();
		}
	};
}

#endif