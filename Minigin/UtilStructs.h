#ifndef UTILSTRUCTS
#define UTILSTRUCTS

namespace Pengin
{

	namespace UtilStructs
	{
		struct Rectf final
		{
			float x;
			float y;
			float width;
			float height;

			Rectf() = default;

			Rectf(float x, float _y, float _width, float _height) : 
				x(x), 
				y(_y), 
				width(_width), 
				height(_height) 
			{}

			/*auto operator<=>(const Rectf& other) const
			{
				if (auto cmp = x <=> other.x; cmp != 0) return cmp;
				if (auto cmp = y <=> other.y; cmp != 0) return cmp;
				if (auto cmp = width <=> other.width; cmp != 0) return cmp;
				return height <=> other.height;
			}

			bool operator==(const Rectf& other) const
			{
				return (*this <=> other) == std::strong_ordering::equal;
			}*/
		};

		struct Recti final
		{
			int x;
			int y;
			unsigned width;
			unsigned height;

			Recti() = default;

			Recti(int _x, int _y, unsigned _width, unsigned _height) :
				x{ _x },
				y(_y),
				width(_width),
				height(_height)
			{}

			auto operator<=>(const Recti& other) const
			{
				if (auto cmp = x <=> other.x; cmp != 0) return cmp;
				if (auto cmp = y <=> other.y; cmp != 0) return cmp;
				if (auto cmp = width <=> other.width; cmp != 0) return cmp;
				return height <=> other.height;
			}

			bool operator==(const Recti& other) const
			{
				return (*this <=> other) == std::strong_ordering::equal;
			}
		};
	}
}


#endif
