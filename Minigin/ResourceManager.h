#ifndef PENGIN_RESOURCEMANAGER
#define PENGIN_RESOURCEMANAGER

#include <string>
#include <memory>
#include <unordered_map>
#include "Singleton.h"

#include "Texture2D.h"

namespace Pengin
{
	class Font;
	class ResourceManager final : public Pengin::Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);

		[[nodiscard]] std::shared_ptr<Texture2D> LoadTexture(const std::string& path) const;
		[[nodiscard]] std::shared_ptr<Font> LoadFont(const std::string& path, unsigned size) const;

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&&) = delete;

	private:
		friend class Pengin::Singleton<ResourceManager>;
		ResourceManager() = default;
		~ResourceManager() = default;

		std::string m_DataPath;

		friend class Texture2D;
		friend class Font;

		mutable std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_Textures;
		mutable std::unordered_map<std::string, std::shared_ptr<Font>> m_Fonts;

		void ReleaseTexture(const std::string& path) noexcept;
		void ReleaseFont(const std::string& path, unsigned size) noexcept;
	};
}

#endif