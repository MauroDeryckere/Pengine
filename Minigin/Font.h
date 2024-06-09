#ifndef PENGIN_FONT
#define PENGIN_FONT

#include <string>

struct _TTF_Font;
namespace Pengin
{
	class Font final
	{
	public:
		explicit Font(_TTF_Font* pFont, const std::string& path, unsigned fontSize);
		~Font();

		[[nodiscard]] _TTF_Font* GetFont() const noexcept;
		[[nodiscard]] const std::string& GetPath() const noexcept { return m_FontPath; }
		[[nodiscard]] unsigned GetFontSize() const noexcept { return m_FontSize; }

		Font(const Font &) = delete;
		Font(Font &&) = delete;
		Font & operator= (const Font &) = delete;
		Font & operator= (const Font &&) = delete;

	private:
		_TTF_Font* m_Font;

		std::string m_FontPath;
		unsigned m_FontSize;
	};
}

#endif	