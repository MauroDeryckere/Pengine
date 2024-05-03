#ifndef PENGIN_SERIALIZER
#define PENGIN_SERIALIZER

#include "DebugOutput.h"

#include "GameUUID.h"
#include "EntityId.h"

#include <vector>
#include <tuple>
#include <filesystem>

namespace Pengin
{
	class ECS;
	struct SceneData;

	using UserIndex = GameUUID;

	using InputData = std::tuple <UserIndex, unsigned>;
	using InputDataVec = std::vector<InputData>;

	class FieldSerializer;

	class Serializer
	{
	public:
		Serializer() = default;
		virtual ~Serializer() = default;

		[[nodiscard]] virtual bool SerializeScene(const ECS& ecs, const SceneData& sceneData, const std::filesystem::path& scenePath) const noexcept = 0;
		[[nodiscard]] virtual bool DeserializeScene(SceneData& sceneData, std::unordered_map<GameUUID, EntityId>& entityMap, ECS& ecs, const std::filesystem::path& scenePath) noexcept = 0;

		[[nodiscard]] virtual bool SerializeInput(const std::filesystem::path& filePath) const noexcept = 0;
		[[nodiscard]] virtual std::pair<bool, InputDataVec> DeserializeInput(const std::filesystem::path& filePath) noexcept = 0;

		[[nodiscard]] virtual bool SerializeSceneEntity(const ECS& ecs, const EntityId entityId, const std::filesystem::path& filePath, bool keepUUID = false) const noexcept = 0;
		[[nodiscard]] virtual std::pair<bool, EntityId> DerserializeSceneEntity(ECS& ecs, std::unordered_map<GameUUID, EntityId>& entityMap, const std::filesystem::path& filePath, bool newUUID = true) noexcept = 0;

		[[nodiscard]] virtual FieldSerializer* GetFieldSerializer() noexcept = 0;

		[[nodiscard]] virtual const std::unordered_map<std::string, std::vector<uint8_t>> ParseJsonStr(const std::string& json) const noexcept = 0;
	};

	class NullSerializer final : public Serializer
	{
	public:
		NullSerializer() = default;
		virtual ~NullSerializer() override = default;

		[[nodiscard]] bool SerializeScene(const ECS&, const SceneData&, const std::filesystem::path&) const noexcept { return false; }
		[[nodiscard]] bool DeserializeScene(SceneData&, std::unordered_map<GameUUID, EntityId>&, ECS&, const std::filesystem::path&) noexcept { return false; }

		[[nodiscard]] bool SerializeInput(const std::filesystem::path&) const noexcept { return false; }
		[[nodiscard]] std::pair<bool, InputDataVec> DeserializeInput(const std::filesystem::path&) noexcept { return { false, {} }; }

		[[nodiscard]] bool SerializeSceneEntity(const ECS&, const EntityId, const std::filesystem::path&, bool) const noexcept { return false; }
		[[nodiscard]] std::pair<bool, EntityId> DerserializeSceneEntity(ECS&, std::unordered_map<GameUUID, EntityId>&, const std::filesystem::path&, bool) noexcept { return { false, NULL_ENTITY_ID }; }

		[[nodiscard]] FieldSerializer* GetFieldSerializer() noexcept 
		{ 
			DEBUG_OUT("WARNING: Getting an empty fieldserializer, will not work");
			return nullptr;
		}

		[[nodiscard]] const std::unordered_map<std::string, std::vector<uint8_t>> ParseJsonStr(const std::string&) const noexcept { return {}; }
	};
}

#endif