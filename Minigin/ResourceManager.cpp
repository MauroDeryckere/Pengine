#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"


#include "Texture2D.h"

namespace Pengin
{
	void ResourceManager::Init(const std::string& dataPath)
	{
		m_DataPath = dataPath;

		if (TTF_Init() != 0)
		{
			throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
		}
	}

	std::shared_ptr<Texture2D> ResourceManager::LoadTexture(const std::string& path) const
	{
		assert(!path.empty());

		const auto fullPath = m_DataPath + path;

		auto it = m_Textures.find(fullPath);
		if (it != m_Textures.end())
		{
			return it->second;
		}

		auto texture = IMG_LoadTexture(dae::Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());

		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
		}

		auto texturePtr = std::make_shared<Texture2D>(texture, path);
		m_Textures[fullPath] = texturePtr;
		return texturePtr;
	}

	std::shared_ptr<Font> ResourceManager::LoadFont(const std::string& path, unsigned size) const
	{
		assert(!path.empty());

		const auto fullPath = m_DataPath + path;

		auto fontKey = fullPath + std::to_string(size);
		auto it = m_Fonts.find(fontKey);
		if (it != m_Fonts.end())
		{
			return it->second;
		}

		auto font = TTF_OpenFont(fullPath.c_str(), size);
		if (font == nullptr)
		{
			throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
		}

		auto fontPtr = std::make_shared<Font>(font, path, size);
		m_Fonts[fontKey] = fontPtr;
		return fontPtr;
	}

	void ResourceManager::ReleaseTexture(const std::string& path) noexcept
	{
		auto it = m_Textures.find(m_DataPath + path);

		if (it == end(m_Textures))
		{
			return;
		}

		m_Textures.erase(it);
	}
	void ResourceManager::ReleaseFont(const std::string& path, unsigned size) noexcept
	{
		auto it = m_Fonts.find(m_DataPath + path + std::to_string(size));

		if (it == end(m_Fonts))
		{
			return;
		}

		m_Fonts.erase(it);
	}
}


