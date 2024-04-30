#ifndef PENGIN_JSONSERIALIZER
#define PENGIN_JSONSERIALIZER

#include "Serializer.h"

#include <filesystem>
#include <memory>

namespace Pengin
{
	class JsonSerializer final : public Serializer
	{
	public:
		JsonSerializer();
		~JsonSerializer();

		[[nodiscard]] bool SerializeScene(const ECS& ecs, const SceneData& sceneData, const std::filesystem::path& scenePath) const noexcept;
		[[nodiscard]] bool DeserializeScene(SceneData& sceneData, std::unordered_map<GameUUID, EntityId>& entityMap, ECS& ecs, const std::filesystem::path& scenePath) noexcept;

		[[nodiscard]] bool SerializeInput(const std::filesystem::path& filePath) const noexcept;
		[[nodiscard]] std::pair<bool, InputDataVec> DeserializeInput(const std::filesystem::path& filePath) noexcept;

		[[nodiscard]] bool SerializeSceneEntity(const ECS& ecs, const EntityId entityId, const std::filesystem::path& filePath, bool keepUUID = false) const noexcept;
 		[[nodiscard]] std::pair<bool, EntityId> DerserializeSceneEntity(ECS& ecs, std::unordered_map<GameUUID, EntityId>& entityMap, const std::filesystem::path& filePath, bool newUUID = true) noexcept;

		JsonSerializer(const JsonSerializer&) = delete;
		JsonSerializer(JsonSerializer&&) = delete;
		JsonSerializer& operator=(const JsonSerializer&) = delete;
		JsonSerializer& operator=(const JsonSerializer&&) = delete;
		
	private:
		class JsonSerializerImpl;
		std::unique_ptr<JsonSerializerImpl> pImpl;
	};
}

#endif