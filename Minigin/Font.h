#pragma once
#include <string>

struct _TTF_Font;
namespace dae
{
	/**
	 * Simple RAII wrapper for a _TTF_Font
	 */
	class Font final
	{
	public:
		_TTF_Font* GetFont() const;
		explicit Font(_TTF_Font* pFont, const std::string& path, unsigned fontSize);
		~Font();

		const std::string& GetPath() const noexcept { return m_FontPath; }
		unsigned GetFontSize() const noexcept { return m_FontSize; }

		Font(const Font &) = delete;
		Font(Font &&) = delete;
		Font & operator= (const Font &) = delete;
		Font & operator= (const Font &&) = delete;
	private:
		_TTF_Font* m_font;

		std::string m_FontPath;
		unsigned m_FontSize;
	};
}
