#ifndef COMPONENTS
#define COMPONENTS

#include "Texture2D.h"
#include "ResourceManager.h"

namespace Pengin
{
	struct TestComponent
	{
		int randomintfortesting = 1;

		TestComponent() = default;
		TestComponent(int testConstructorArgs) : randomintfortesting{ testConstructorArgs } {}
	};

	struct PositionComponent
	{
		PositionComponent() = default;

		int x, y;
	};

	struct TextureComponent
	{
		TextureComponent() = default;
		TextureComponent(const std::string& texturePath) : m_pTexture{ m_pTexture = dae::ResourceManager::GetInstance().LoadTexture(texturePath) } {}

		void SetTexture(const std::string& texturePath) { m_pTexture = dae::ResourceManager::GetInstance().LoadTexture(texturePath); }

		std::shared_ptr<dae::Texture2D> m_pTexture{};
	};
}

#endif