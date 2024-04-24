#ifndef SERIALIZER
#define SERIALIZER

#include "CoreIncludes.h"
#include <filesystem>
#include <memory>

namespace Pengin
{
	class ECS;
	struct SceneData;

	using UserIndex = GameUUID;

	using InputData = std::tuple <UserIndex, unsigned>;
	using InputDataVec = std::vector<InputData>;
	class Serializer final : public Pengin::Singleton<Serializer>
	{
	public:
		[[nodiscard]] bool SerializeScene(const ECS& ecs, const SceneData& sceneData, const std::filesystem::path& scenePath) const noexcept;
		[[nodiscard]] bool DeserializeScene(SceneData& sceneData, std::unordered_map<GameUUID, EntityId>& entityMap, ECS& ecs, const std::filesystem::path& scenePath) noexcept;

		[[nodiscard]] bool SerializeInput(const std::filesystem::path& filePath) const noexcept;
		[[nodiscard]] std::pair<bool, InputDataVec> DeserializeInput(const std::filesystem::path& filePath) noexcept;

		[[nodiscard]] bool SerializeSceneEntity(const ECS& ecs, const EntityId entityId, const std::filesystem::path& filePath, bool keepUUID = false) const noexcept;
 		[[nodiscard]] std::pair<bool, EntityId> DerserializeSceneEntity(ECS& ecs, std::unordered_map<GameUUID, EntityId>& entityMap, const std::filesystem::path& filePath, bool newUUID = true) noexcept;

		Serializer(const Serializer&) = delete;
		Serializer(Serializer&&) = delete;
		Serializer& operator=(const Serializer&) = delete;
		Serializer& operator=(const Serializer&&) = delete;

	private:
		friend class Pengin::Singleton<Serializer>;
		Serializer();
		~Serializer();

		class SerializerImpl;
		std::unique_ptr<SerializerImpl> pImpl;
	};
}

#endif