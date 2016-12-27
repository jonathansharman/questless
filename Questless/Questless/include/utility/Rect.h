/**
* @file    Point.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for Point, a struct template that holds a 2D point, with given length and space types.
*/

#ifndef RECT_H
#define RECT_H

#include "Vector.h"

template <typename LengthType, typename SpaceType>
struct Rect
{
	enum class Origin { upper_left, upper_right, lower_left, lower_right }; /// @todo These don't really make sense in different spaces.

	using length_t = LengthType;
	using space_t = SpaceType;

	length_t x;
	length_t y;
	length_t w;
	length_t h;

	constexpr explicit Rect() : x{0}, y{0}, w{0}, h{0} {}
	constexpr explicit Rect(length_t x, length_t y, length_t w, length_t h) : x{x}, y{y}, w{w}, h{h} {}
	explicit Rect(length_t x, length_t y, length_t w, length_t h, Origin origin) : w{w}, h{h}
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

	auto position() const { return Point<length_t, space_t>{x, y}; }
	length_t right() const { return x + w - 1; } /// @todo This is space-dependent.
	length_t bottom() const { return y + h - 1; } /// @todo This is space-dependent.

	constexpr bool contains(Point<length_t, space_t> point) const
	{
		return point.x >= x && point.x < x + w && point.y >= y && point.y < y + h;
	}

	/// Translates the rectangle by the given offset.
	void translate(Vector<length_t, space_t> offset)
	{
		x += offset.x;
		y += offset.y;
	}

	/// @return A copy of the rectangle translated by the given offset.
	constexpr Rect translated(Vector<length_t, space_t> offset) const { return Rect{x + offset.x, y + offset.y, w, h}; }
};

// Point Type Definitions

using TextureRect = Rect<int, TextureSpace>;
using ScreenRect = Rect<int, ScreenSpace>;
using GameRect = Rect<double, GameSpace>;

#endif
