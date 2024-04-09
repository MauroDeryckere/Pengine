#ifndef SERIALIZER
#define SERIALIZER

#include "CoreIncludes.h"

#include <filesystem>
#include <json.hpp>

namespace Pengin
{
	class ECS;

	class Serializer final : public dae::Singleton<Serializer>
	{
	public:
		[[nodiscard]] bool SerializeScene(const ECS& ecs, const std::string& sceneName, const std::filesystem::path& scenePath) const noexcept;
		[[nodiscard]] bool DeserializeScene(std::string& sceneName, std::unordered_map<UUID, EntityId>& entityMap, ECS& ecs, const std::filesystem::path& scenePath) noexcept;

		bool SerializeEntity(const ECS& ecs, const EntityId id, const std::filesystem::path& path) const noexcept;
		bool DeserializeEntity(ECS& ecs, const std::filesystem::path& path) noexcept;

		Serializer(const Serializer&) = delete;
		Serializer(Serializer&&) = delete;
		Serializer& operator=(const Serializer&) = delete;
		Serializer& operator=(const Serializer&&) = delete;

	private:
		friend class dae::Singleton<Serializer>;
		Serializer() = default;
		~Serializer() = default;

		[[nodiscard]] bool SerializeScene_Json(const ECS& ecs, const std::string& sceneName, const std::filesystem::path& scenePath) const noexcept;
		[[nodiscard]] bool DeserializeScene_Json(std::string& sceneName, std::unordered_map<UUID, EntityId>& entityMap, ECS& ecs, const std::filesystem::path& scenePath) noexcept;

		using json = nlohmann::ordered_json;
		[[nodiscard]] bool SerializeSceneEntity_Json(const ECS& ecs, const EntityId id, json& j) const noexcept;
		[[nodiscard]] bool DeserializeSceneEntity_Json(ECS& ecs, std::unordered_map<UUID, EntityId>& entityMap, const json& entityData) noexcept;

		void OutputEntityData(const ECS& ecs, const EntityId id) const noexcept;
	};
}

#endif