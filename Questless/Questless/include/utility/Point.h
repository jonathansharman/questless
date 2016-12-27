/**
* @file    Point.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for Point, a struct template that holds a 2D point, with given length and space types.
*/

#ifndef POINT_H
#define POINT_H

#include "Vector.h"

template <typename LengthType, typename SpaceType>
struct Point
{
	using length_t = LengthType;
	using space_t = SpaceType;

	length_t x;
	length_t y;

	constexpr explicit Point() : x{length_t{}}, y{length_t{}} {}
	constexpr explicit Point(length_t x, length_t y) : x{x}, y{y} {}
	constexpr Point(const Point& p) : x{p.x}, y{p.y} {}

	constexpr bool operator ==(const Point& right) const { return x == right.x && y == right.y; }
	constexpr bool operator !=(const Point& right) const { return x != right.x || y != right.y; }
		
	constexpr friend Point operator +(const Point& p, const Vector<length_t, space_t>& v) { return Point{p.x + v.x, p.y + v.y}; }
	constexpr friend Point operator +(const Vector<length_t, space_t>& v, Point p) { return Point{v.x + p.x, v.y + p.y}; }
	constexpr friend Point operator -(const Point& p, const Vector<length_t, space_t>& v) { return Point{p.x - v.x, p.y - v.y}; }
	constexpr friend auto operator -(const Point& p1, const Point& p2) { return Vector<length_t, space_t>{p1.x - p2.x, p1.y - p2.y}; }
	constexpr friend Point operator -(const Point& p) { return Point{-p.x, -p.y}; }
	constexpr friend Point operator *(const Point& p, length_t k) { return Point{k * p.x, k * p.y}; }
	constexpr friend Point operator *(length_t k, const Point& p) { return Point{k * p.x, k * p.y}; }
	constexpr friend Point operator /(const Point& p, length_t k) { return Point{p.x / k, p.y / k}; }

	Point& operator =(const Point& p) & = default;

	Point& operator +=(const Vector<length_t, space_t>& v) &
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	Point& operator -=(const Vector<length_t, space_t>& v) &
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	Point& operator *=(length_t factor) &
	{
		x *= factor;
		y *= factor;
		return *this;
	}
	Point& operator /=(length_t divisor) &
	{
		x /= divisor;
		y /= divisor;
		return *this;
	}

	/// Rotates the point about another point, overwriting the original value.
	/// @param origin The origin around which the point will be rotated.
	/// @param dtheta The counter-clockwise rotation to apply, in radians.
	void rotate(const Point& origin, questless::AngleRadians dtheta) & /// @todo: MIXING DEPENDENCIES
	{
		length_t cos_dtheta = static_cast<length_t>(cos(dtheta));
		length_t sin_dtheta = static_cast<length_t>(sin(dtheta));
		auto offset = Vector<length_t, space_t>{origin.x, origin.y};
		*this -= offset;
		*this = Point(x * cos_dtheta - y * sin_dtheta, x * sin_dtheta + y * cos_dtheta) + offset;
	}

	/// Creates a copy of the point, rotated about another point.
	/// @param origin The origin around which the point will be rotated.
	/// @param theta The counter-clockwise rotation to apply, in radians.
	Point rotated(const Point& origin, questless::AngleRadians dtheta) const /// @todo: MIXING DEPENDENCIES
	{
		length_t cos_dtheta = cos(dtheta);
		length_t sin_dtheta = sin(dtheta);
		auto offset = Vector<length_t, space_t>{origin.x, origin.y};
		Point temp = *this - offset;
		return Point(temp.x * cos_dtheta - temp.y * sin_dtheta, temp.x * sin_dtheta + temp.y * cos_dtheta) + offset;
	}
};

// Point Type Definitions

using TexturePoint = Point<int, TextureSpace>;
using SpriteSheetPoint = Point<int, SpriteSheetSpace>;
using ScreenPoint = Point<int, ScreenSpace>;
using GamePoint = Point<double, GameSpace>;

#endif
