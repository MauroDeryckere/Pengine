#ifndef SERIALIZER
#define SERIALIZER

#include "CoreIncludes.h"

#include <filesystem>

namespace Pengin
{
	class ECS;

	class Serializer final : public dae::Singleton<Serializer>
	{
	public:
		void SerializeEntity(ECS& ecs, const EntityId id, const std::filesystem::path& path) noexcept;

		void DeserializeEntity(ECS& ecs, const std::filesystem::path& path) noexcept;

		Serializer(const Serializer&) = delete;
		Serializer(Serializer&&) = delete;
		Serializer& operator=(const Serializer&) = delete;
		Serializer& operator=(const Serializer&&) = delete;

	private:
		friend class dae::Singleton<Serializer>;
		Serializer() = default;
		~Serializer() = default;

		void OutputEntityData(ECS& ecs, const EntityId id) const noexcept;
	};
}

#endif