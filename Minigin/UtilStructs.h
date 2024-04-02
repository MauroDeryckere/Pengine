#ifndef UTILSTRUCTS
#define UTILSTRUCTS

#include <compare>
#include <cstdint>

namespace Pengin
{

	namespace UtilStructs
	{
		template<typename T>
		struct Rect final
		{
			T x;
			T y;
			T width;
			T height;

			template<typename U>
			constexpr explicit operator Rect<U>() const noexcept 
			{
				return Rect<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(width), static_cast<U>(height));
			}

			constexpr Rect() noexcept = default;

			constexpr explicit Rect(T _x, T _y, T _width, T _height) noexcept :
				x(_x),
				y(_y),
				width(_width),
				height(_height)
			{}

			constexpr auto operator<=>(const Rect& other) const noexcept
			{
				if (auto cmp = x <=> other.x; cmp != 0) return cmp;
				if (auto cmp = y <=> other.y; cmp != 0) return cmp;
				if (auto cmp = width <=> other.width; cmp != 0) return cmp;
				return height <=> other.height;
			}

			constexpr bool operator==(const Rect& other) const noexcept
			{
				return (*this <=> other) == std::strong_ordering::equal;
			}
		};

		using Rectf = Rect<float>;
		using Recti = Rect<int>;
		using Rectu = Rect<unsigned>;
		using Rect16 = Rect<int16_t>; 
		using Rectu16 = Rect<uint16_t>; 
		using Rect8 = Rect<int8_t>; 
		using Rectu8 = Rect<uint8_t>; 
	}
}


#endif
