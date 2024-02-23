#ifndef TEXTCOMPONENT
#define TEXTCOMPONENT

#include "TextureComponent.h"

namespace dae
{
	class Font;
	class Texture2D;

	class TextComponent final:
		public TextureComponent
	{
	public:
		TextComponent(GameObject* pGameObj, std::shared_ptr<Font> font);
		~TextComponent() override = default;

		void SetText(const std::string& text);

		virtual void Update(float) override;
		virtual void Render() const override;

	private:
		bool m_NeedsUpdate;
		std::string m_Text;

		std::shared_ptr<Font> m_pFont;

	};
}

#endif