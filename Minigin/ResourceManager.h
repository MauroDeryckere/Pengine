#pragma once
#include <string>
#include <memory>
#include "Singleton.h"

#include "Texture2D.h"

namespace dae
{
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);

		[[nodiscard]] std::shared_ptr<Texture2D> LoadTexture(const std::string& path) const;
		[[nodiscard]] std::shared_ptr<Font> LoadFont(const std::string& path, unsigned size) const;

	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;

		std::string m_dataPath;
	};
}
