/**
* @file    basic-sdl-wrappers.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Interface for some simple wrappers around various SDL structs, mainly for the purpose of adding constructors. Also includes some complementary structs, like Vector.
*/

#ifndef BASIC_SDL_WRAPPERS_H
#define BASIC_SDL_WRAPPERS_H

#include <cstdint>

#include <SDL.h>

namespace sdl
{
	struct Point;

	struct Vector
	{
		int x;
		int y;

		/// @return A vector from the origin (0, 0) to the given point.
		static constexpr Vector to(Point p);

		constexpr Vector() : x{0}, y{0} {}
		constexpr Vector(int x, int y) : x{x}, y{y} {}

		constexpr bool operator ==(Vector right) const { return x == right.x && y == right.y; }
		constexpr bool operator !=(Vector right) const { return x != right.x || y != right.y; }

		constexpr friend Vector operator +(Vector v1, Vector v2) { return {v1.x + v2.x, v1.y + v2.y}; }
		constexpr friend Vector operator -(Vector v1, Vector v2) { return {v1.x - v2.x, v1.y - v2.y}; }
		constexpr friend Vector operator -(const Vector& v) { return{-v.x, -v.y}; }

		Vector& operator =(Vector v)
		{
			x = v.x;
			y = v.y;
			return *this;
		}
		Vector& operator +=(Vector v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}
		Vector& operator -=(Vector v)
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}

		/// @return The square of the vector's length.
		int length_squared() const { return x * x + y * y; }
	};

	struct Point
	{
		int x;
		int y;

		constexpr Point() : x{0}, y{0} {}
		constexpr Point(int x, int y) : x{x}, y{y} {}

		constexpr friend bool operator ==(Point left, Point right) { return left.x == right.x && left.y == right.y; }
		constexpr friend bool operator !=(Point left, Point&right) { return left.x != right.x || left.y != right.y; }
		
		constexpr friend Point operator +(const Point& p, const Vector& v) { return {p.x + v.x, p.y + v.y}; }
		constexpr friend Point operator +(const Vector& v, const Point& p) { return {v.x + p.x, v.y + p.y}; }
		constexpr friend Point operator -(const Point& p, const Vector& v) { return {p.x - v.x, p.y - v.y}; }
		constexpr friend Vector operator -(const Point& p1, const Point& p2) { return{p1.x - p2.x, p1.y - p2.y}; }
		constexpr friend Point operator -(const Point& p) { return{-p.x, -p.y}; }

		Point& operator =(const Point& p)
		{
			x = p.x;
			y = p.y;
			return *this;
		}
		Point& operator +=(const Vector& v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}
		Point& operator -=(const Vector& v)
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}

		constexpr operator SDL_Point() const { return {x, y}; }
	};

	inline constexpr Vector Vector::to(Point p)
	{
		return Vector{p.x, p.y};
	}

	struct Rect
	{
		enum class Origin { upper_left, upper_right, lower_left, lower_right };

		int x;
		int y;
		int w;
		int h;

		constexpr Rect() : x{0}, y{0}, w{0}, h{0} {}
		constexpr Rect(int x, int y, int w, int h) : x{x}, y{y}, w{w}, h{h} {}
		Rect(int x, int y, int w, int h, Origin origin) : w{w}, h{h}
		{
			switch (origin) {
				case Origin::upper_left:
					this->x = x;
					this->y = y;
					break;
				case Origin::upper_right:
					this->x = x - w + 1;
					this->y = y;
					break;
				case Origin::lower_left:
					this->x = x;
					this->y = y - h + 1;
					break;
				case Origin::lower_right:
					this->x = x - w + 1;
					this->y = y - h + 1;
					break;
			}
		}

		constexpr friend bool operator ==(const Rect& left, const Rect& right)
		{
			return left.x == right.x && left.y == right.y && left.w == right.w && left.h == right.h;
		}
		constexpr friend bool operator !=(const Rect& left, const Rect& right)
		{
			return left.x != right.x || left.y != right.y || left.w != right.w || left.h != right.h;
		}

		Point position() const { return Point{x, y}; }
		int right() const { return x + w - 1; }
		int bottom() const { return y + h - 1; }

		constexpr operator SDL_Rect() const { return {x, y, w, h}; }

		constexpr bool contains(Point point) const
		{
			return point.x >= x && point.x < x + w && point.y >= y && point.y < y + h;
		}

		/// Translates the rectangle by the given offset.
		void translate(Vector offset)
		{
			x += offset.x;
			y += offset.y;
		}

		/// @return A copy of the rectangle translated by the given offset.
		constexpr Rect translated(Vector offset) const { return {x + offset.x, y + offset.y, w, h}; }
	};

	struct Color
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		static constexpr Color clear() { return {0, 0, 0, 0}; }
		static constexpr Color white(uint8_t alpha = 255) { return {255, 255, 255, alpha}; }
		static constexpr Color silver(uint8_t alpha = 255) { return {192, 192, 192, alpha}; }
		static constexpr Color gray(uint8_t alpha = 255) { return {128, 128, 128, alpha}; }
		static constexpr Color black(uint8_t alpha = 255) { return {0, 0, 0, alpha}; }
		static constexpr Color red(uint8_t alpha = 255) { return {255, 0, 0, alpha}; }
		static constexpr Color lime(uint8_t alpha = 255) { return {0, 255, 0, alpha}; }
		static constexpr Color green(uint8_t alpha = 255) { return {0, 192, 0, alpha}; }
		static constexpr Color blue(uint8_t alpha = 255) { return {0, 0, 255, alpha}; }
		static constexpr Color cyan(uint8_t alpha = 255) { return {0, 255, 255, alpha}; }
		static constexpr Color yellow(uint8_t alpha = 255) { return {255, 255, 0, alpha}; }
		static constexpr Color magenta(uint8_t alpha = 255) { return {255, 0, 255, alpha}; }
		static constexpr Color orange(uint8_t alpha = 255) { return {255, 128, 0, alpha}; }
		static constexpr Color purple(uint8_t alpha = 255) { return {128, 0, 128, alpha}; }
		static constexpr Color teal(uint8_t alpha = 255) { return {0, 128, 128, alpha}; }
		static constexpr Color tan(uint8_t alpha = 255) { return {192, 160, 128, alpha}; }
		static constexpr Color brown(uint8_t alpha = 255) { return {128, 64, 0, alpha}; }

		constexpr Color() : r{0}, g{0}, b{0}, a{255} {}
		constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r{r}, g{g}, b{b}, a{a} {}
		constexpr Color(Color color, uint8_t a) : r{color.r}, g{color.g}, b{color.b}, a{a} {}

		constexpr friend bool operator ==(Color left, Color right)
		{
			return left.r == right.r && left.g == right.g && left.b == right.b && left.a == right.a;
		}
		constexpr friend bool operator !=(Color left, Color right)
		{
			return left.r != right.r || left.g != right.g || left.b != right.b || left.a != right.a;
		}

		Color operator *(double factor) const
		{
			uint8_t r_new = static_cast<uint8_t>(factor * this->r);
			uint8_t g_new = static_cast<uint8_t>(factor * this->g);
			uint8_t b_new = static_cast<uint8_t>(factor * this->b);
			return Color{r_new, g_new, b_new, this->a};
		}

		Color& operator *=(double factor)
		{
			r = static_cast<uint8_t>(factor * r);
			g = static_cast<uint8_t>(factor * g);
			b = static_cast<uint8_t>(factor * b);
			return *this;
		}

		constexpr operator SDL_Color() const { return {r, g, b, a}; }
	};
}

#endif
