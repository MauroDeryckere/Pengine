#ifndef PENGIN_TEXTURE2D
#define PENGIN_TEXTURE2D

#include <glm/vec2.hpp>

#include <string>
#include <string_view>

struct SDL_Texture;
namespace Pengin
{
	class Texture2D final
	{
	public:
		Texture2D(SDL_Texture* texture, const std::string& path);
		~Texture2D();

		[[nodiscard]] SDL_Texture* GetSDLTexture() const noexcept;
		[[nodiscard]] glm::ivec2 GetSize() const noexcept;

		[[nodiscard]] const std::string& GetPath() const noexcept { return m_Path; }

		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;

	private:
		SDL_Texture* m_texture;
		const std::string m_Path;
	};
}

#endif