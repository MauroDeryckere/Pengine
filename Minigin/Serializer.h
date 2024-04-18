#ifndef SERIALIZER
#define SERIALIZER

#include "CoreIncludes.h"

#include <filesystem>
#include <json.hpp>

namespace Pengin
{
	class ECS;
	struct SceneData;

	using UserIndex = UUID;

	using InputData = std::tuple <UserIndex, unsigned>;
	using InputDataVec = std::vector<InputData>;

	class Serializer final : public dae::Singleton<Serializer>
	{
	public:
		[[nodiscard]] bool SerializeScene(const ECS& ecs, const SceneData& sceneData, const std::filesystem::path& scenePath) const noexcept;
		[[nodiscard]] bool DeserializeScene(SceneData& sceneData, std::unordered_map<UUID, EntityId>& entityMap, ECS& ecs, const std::filesystem::path& scenePath) noexcept;

		[[nodiscard]] bool SerializeInput(const std::filesystem::path& filePath) const noexcept;
		[[nodiscard]] std::pair<bool, InputDataVec> DeserializeInput(const std::filesystem::path& filePath) noexcept;

		[[nodiscard]] bool SerializeSceneEntity(const ECS& ecs, const EntityId entityId, const std::filesystem::path& filePath) const noexcept;
 		[[nodiscard]] std::pair<bool, EntityId> DerserializeSceneEntity(ECS& ecs, std::unordered_map<UUID, EntityId>& entityMap, const std::filesystem::path& filePath) noexcept;

		Serializer(const Serializer&) = delete;
		Serializer(Serializer&&) = delete;
		Serializer& operator=(const Serializer&) = delete;
		Serializer& operator=(const Serializer&&) = delete;

	private:
		friend class dae::Singleton<Serializer>;
		Serializer() = default;
		~Serializer() = default;

		bool SerializeScene_Json(const ECS& ecs, const SceneData& sceneData, const std::filesystem::path& scenePath) const noexcept;
		bool DeserializeScene_Json(SceneData& sceneData, std::unordered_map<UUID, EntityId>& entityMap, ECS& ecs, const std::filesystem::path& scenePath) noexcept;

		bool SerializeInput_Json(const std::filesystem::path& filePath) const noexcept;

		[[nodiscard]] std::pair<bool, InputDataVec> DeserializeInput_Json(const std::filesystem::path& filePath) noexcept;

		using json = nlohmann::ordered_json;
		bool SerializeSceneEntity_Json(const ECS& ecs, const EntityId id, json& j) const noexcept;
		std::pair<bool, EntityId> DeserializeSceneEntity_Json(ECS& ecs, std::unordered_map<UUID, EntityId>& entityMap, const json& entityData) noexcept;
	};
}

#endif