#pragma once
#include <glm/vec2.hpp>

#include <string>
#include <string_view>

struct SDL_Texture;
namespace dae
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Texture2D final
	{
	public:
		Texture2D(SDL_Texture* texture, const std::string& path);
		~Texture2D();

		SDL_Texture* GetSDLTexture() const;
		glm::ivec2 GetSize() const;

		[[nodiscard]] const std::string& GetPath() const { return m_Path; }

		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;

	private:
		SDL_Texture* m_texture;
		const std::string m_Path;
	};
}
