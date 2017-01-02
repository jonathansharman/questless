/**
* @file    Point.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION A generic 2D rectangle, with parameterized length and space types.
*/

#ifndef RECT_H
#define RECT_H

#include "Vector.h"

namespace units
{
	template <typename SpaceType>
	struct Rect
	{
		enum class Origin { upper_left, upper_right, lower_left, lower_right }; /// @todo These don't really make sense in different spaces.

		using space_t = SpaceType;
		using scalar_t = typename space_t::scalar_t;

		scalar_t x;
		scalar_t y;
		scalar_t w;
		scalar_t h;

		constexpr explicit Rect() : x{scalar_t{}}, y{scalar_t{}}, w{scalar_t{}}, h{scalar_t{}} {}
		constexpr explicit Rect(scalar_t x, scalar_t y, scalar_t w, scalar_t h) : x{x}, y{y}, w{w}, h{h} {}
		explicit Rect(scalar_t x, scalar_t y, scalar_t w, scalar_t h, Origin origin) : w{w}, h{h}
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

		friend std::ostream& operator <<(std::ostream& out, const Rect& r)
		{
			out << '(' << r.x << ", " << r.y << ", " << r.w << ", " << r.h << ')';
			return out;
		}

		constexpr friend bool operator ==(const Rect& left, const Rect& right)
		{
			return left.x == right.x && left.y == right.y && left.w == right.w && left.h == right.h;
		}
		constexpr friend bool operator !=(const Rect& left, const Rect& right)
		{
			return left.x != right.x || left.y != right.y || left.w != right.w || left.h != right.h;
		}

		constexpr auto position() const { return Point<space_t>{x, y}; }
		constexpr scalar_t right() const { return x + w - 1; } /// @todo This is space-dependent.
		constexpr scalar_t bottom() const { return y + h - 1; } /// @todo This is space-dependent.

		constexpr bool contains(Point<space_t> point) const
		{
			return point.x >= x && point.x < x + w && point.y >= y && point.y < y + h;
		}

		/// Translates the rectangle by the given offset.
		void translate(Vector<space_t> offset) &
		{
			x += offset.x;
			y += offset.y;
		}

		/// @return A copy of the rectangle translated by the given offset.
		constexpr Rect translated(Vector<space_t> offset) const { return Rect{x + offset.x, y + offset.y, w, h}; }

		/// Extends the given bounding rectangle by the given point.
		void extend(const Point<space_t>& point)
		{
			if (point.x < x) {
				w += x - point.x;
				x = point.x;
			} else if (point.x > x + w) {
				w = point.x - x;
			}
			if (point.y < y) {
				h += y - point.y;
				y = point.y;
			} else if (point.y > y + h) {
				h = point.y - y;
			}
		}
	};
}

#endif
