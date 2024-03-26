#pragma once
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class Texture2D;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);

		[[nodiscard]] std::shared_ptr<Texture2D> LoadTexture(std::string_view file) const;
		[[nodiscard]] std::shared_ptr<Font> LoadFont(std::string_view, unsigned size) const;

	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_dataPath;
	};
}
