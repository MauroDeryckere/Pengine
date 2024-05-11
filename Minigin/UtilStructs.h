#ifndef UTILSTRUCTS
#define UTILSTRUCTS

#include <compare>
#include <cstdint>
#include <cassert>

#include <type_traits> // For std::is_floating_point_v

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
			{
				assert(width >= 0 && height >= 0);
			}

			Rect(const Rect& other) noexcept = default; 
			Rect(Rect&& other) noexcept = default;
			Rect& operator=(const Rect& other) noexcept = default;
			Rect& operator=(Rect&& other) noexcept = default; 

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

			constexpr operator bool() const noexcept { return *this != Rect<T>{}; }
		};

		using Rectf = Rect<float>;

		using Rect8 = Rect<int8_t>;
		using Rect16 = Rect<int16_t>; 
		using Recti = Rect<int32_t>;
		
		using Rectu8 = Rect<uint8_t>;
		using Rectu16 = Rect<uint16_t>; 
		using Rectu = Rect<uint32_t>;
	}

	namespace UtilFuncs
	{
		constexpr float EPSILON{ 0.001f };

		template<typename T>
		[[nodiscard]] inline constexpr bool IsCollidingAABB(const UtilStructs::Rect<T>& rect1, const UtilStructs::Rect<T>& rect2) noexcept
		{
			if constexpr (std::is_floating_point_v<T>)
			{
				constexpr T CASTED_EPS{ static_cast<T>(EPSILON) };

				if (rect1.x + rect1.width + CASTED_EPS < rect2.x || rect2.x + rect2.width + CASTED_EPS < rect1.x)
				{
					return false;
				}

				if (rect1.y + rect1.height + CASTED_EPS < rect2.y || rect2.y + rect2.height + CASTED_EPS < rect1.y)
				{
					return false;
				}
				
				return true;
			}
			else
			{
				if ((rect1.x + rect1.width) < rect2.x || (rect2.x + rect2.width) < rect1.x)
				{
					return false;
				}

				if (rect1.y > (rect2.y + rect2.height) || rect2.y > (rect1.y + rect1.height))
				{
					return false;
				}

				return true;
			}
		}
	}
}


#endif
