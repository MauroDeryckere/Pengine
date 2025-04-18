#ifndef UTILSTRUCTS
#define UTILSTRUCTS

#include <compare>
#include <cstdint>
#include <cassert>
#include <random>
#include <type_traits> // For std::is_floating_point_v

namespace Pengin
{
	namespace UtilStructs
	{
		template<typename T> 
		requires std::is_arithmetic_v<T>
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
			{ }


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
		using Rect32 = Rect<int32_t>;
		using Recti = Rect<int32_t>;
		
		using Rectu8 = Rect<uint8_t>;
		using Rectu16 = Rect<uint16_t>; 
		using Rectu32 = Rect<uint32_t>; 
		using Rectu = Rect<uint32_t>;
	}

	namespace UtilFuncs
	{
		template <typename T>
		[[nodiscard]] inline const T RandomNumber(const T min, const T max) noexcept
		{
			static_assert(std::is_arithmetic_v<T>, "T must be a numeric type");

			thread_local static std::mt19937 gen{ std::random_device{}() };

			if constexpr (std::is_integral_v<T>) 
			{
				std::uniform_int_distribution<T> dis(min, max);
				return dis(gen);
			}
			else 
			{
				std::uniform_real_distribution<T> dis(min, max);
				return dis(gen);
			}
		}

		constexpr float EPSILON{ 0.001f };

		template<typename T>
		[[nodiscard]] inline constexpr bool IsPointInRect(const UtilStructs::Rect<T>& rect, T px, T py) noexcept 
		{
			static_assert(std::is_arithmetic_v<T>, "IsPointInRect requires arithmetic types");
		
			if constexpr (std::is_floating_point_v<T>)
			{
				static constexpr T CASTED_EPS{ static_cast<T>(EPSILON) };

				return not (px + CASTED_EPS <= rect.x 
						|| px >= rect.x + rect.width + CASTED_EPS 
						|| py + CASTED_EPS <= rect.y 
						|| py >= rect.y + rect.height + CASTED_EPS);
			}
			else
			{
				return not (px <= rect.x 
						|| px >= rect.x + rect.width
						|| py <= rect.y 
						|| py >= rect.y + rect.height);
			}
		}

		template<typename T>
		[[nodiscard]] inline constexpr bool IsCollidingAABB(const UtilStructs::Rect<T>& rect1, const UtilStructs::Rect<T>& rect2) noexcept
		{
			static_assert(std::is_arithmetic_v<T>, "IsCollidingAABB requires arithmetic types");

			if constexpr (std::is_floating_point_v<T>)
			{
				static constexpr T CASTED_EPS{ static_cast<T>(EPSILON) };

				return not (rect1.x + rect1.width + CASTED_EPS <= rect2.x 
						|| rect2.x + rect2.width + CASTED_EPS <= rect1.x 
						|| rect1.y + rect1.height + CASTED_EPS <= rect2.y 
						|| rect2.y + rect2.height + CASTED_EPS <= rect1.y);
			}
			else
			{
				return not (rect1.x + rect1.width <= rect2.x 
						|| rect2.x + rect2.width <= rect1.x 
						|| rect1.y + rect1.height <= rect2.y 
						|| rect2.y + rect2.height <= rect1.y);
			}
		}
	}
}


#endif
