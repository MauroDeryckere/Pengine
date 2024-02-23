#ifndef TEXTURECOMPONENT
#define TEXTURECOMPONENT

#include <string>
#include <memory>

#include "RenderComponent.h"
#include "Texture2D.h"

namespace dae
{
	class TextureComponent final :
		public RenderComponent
	{
	public:
		TextureComponent(GameObject* pGameObj);
		~TextureComponent() override = default;

		void Update(float) override; 
		void Render() const override;

		void SetTexture(const std::string& filename);

		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

	private:
		std::shared_ptr<Texture2D> m_texture{};

	};
}

#endif