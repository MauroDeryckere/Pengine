#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"


#include "Texture2D.h"

void dae::ResourceManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::shared_ptr<dae::Texture2D> dae::ResourceManager::LoadTexture(const std::string& path) const
{
	assert(!path.empty());

	const auto fullPath = m_DataPath + path;

	auto it = m_Textures.find(fullPath);
	if (it != m_Textures.end())
	{
		return it->second;
	}

	auto texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());

	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}

	auto texturePtr = std::make_shared<dae::Texture2D>(texture, path);
	m_Textures[fullPath] = texturePtr;
	return texturePtr;
}

std::shared_ptr<dae::Font> dae::ResourceManager::LoadFont(const std::string& path, unsigned size) const
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

	auto fontPtr = std::make_shared<dae::Font>(font, path, size);
	m_Fonts[fontKey] = fontPtr;
	return fontPtr;
}
