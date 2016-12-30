/**
* @file    Point.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION A generic 2D point, with parameterized length and space types.
*/

#ifndef POINT_H
#define POINT_H

#include "Vector.h"

namespace units
{
	template <typename SpaceType>
	struct Point
	{
		using space_t = SpaceType;
		using scalar_t = typename space_t::scalar_t;

		scalar_t x;
		scalar_t y;

		constexpr explicit Point() : x{scalar_t{}}, y{scalar_t{}} {}
		constexpr explicit Point(scalar_t x, scalar_t y) : x{x}, y{y} {}
		constexpr Point(const Point& p) : x{p.x}, y{p.y} {}

		constexpr bool operator ==(const Point& right) const { return x == right.x && y == right.y; }
		constexpr bool operator !=(const Point& right) const { return x != right.x || y != right.y; }
		
		constexpr friend Point operator +(const Point& p, const Vector<space_t>& v) { return Point{p.x + v.x, p.y + v.y}; }
		constexpr friend Point operator +(const Vector<space_t>& v, Point p) { return Point{v.x + p.x, v.y + p.y}; }
		constexpr friend Point operator -(const Point& p, const Vector<space_t>& v) { return Point{p.x - v.x, p.y - v.y}; }
		constexpr friend auto operator -(const Point& p1, const Point& p2) { return Vector<space_t>{p1.x - p2.x, p1.y - p2.y}; }
		constexpr friend Point operator -(const Point& p) { return Point{-p.x, -p.y}; }
		constexpr friend Point operator *(const Point& p, scalar_t k) { return Point{k * p.x, k * p.y}; }
		constexpr friend Point operator *(scalar_t k, const Point& p) { return Point{k * p.x, k * p.y}; }
		constexpr friend Point operator /(const Point& p, scalar_t k) { return Point{p.x / k, p.y / k}; }

		Point& operator =(const Point& p) & = default;

		Point& operator +=(const Vector<space_t>& v) &
		{
			x += v.x;
			y += v.y;
			return *this;
		}
		Point& operator -=(const Vector<space_t>& v) &
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}
		Point& operator *=(scalar_t k) &
		{
			x *= k;
			y *= k;
			return *this;
		}
		Point& operator /=(scalar_t k) &
		{
			x /= k;
			y /= k;
			return *this;
		}

		/// Rotates the point about another point, overwriting the original value.
		/// @param origin The origin around which the point will be rotated.
		/// @param dtheta The counter-clockwise rotation to apply, in radians.
		template <typename UnitsPerCircle>
		void rotate(const Point& origin, const Angle<space_t, UnitsPerCircle>& dtheta) &
		{
			scalar_t cos_dtheta = static_cast<scalar_t>(cos(dtheta.count()));
			scalar_t sin_dtheta = static_cast<scalar_t>(sin(dtheta.count()));
			auto offset = Vector<space_t>{origin.x, origin.y};
			*this -= offset;
			*this = Point(x * cos_dtheta - y * sin_dtheta, x * sin_dtheta + y * cos_dtheta) + offset;
		}

		/// Creates a copy of the point, rotated about another point.
		/// @param origin The origin around which the point will be rotated.
		/// @param theta The counter-clockwise rotation to apply, in radians.
		template <typename UnitsPerCircle>
		Point rotated(const Point& origin, const Angle<space_t, UnitsPerCircle>& dtheta) const
		{
			scalar_t cos_dtheta = cos(dtheta.count());
			scalar_t sin_dtheta = sin(dtheta.count());
			auto offset = Vector<space_t>{origin.x, origin.y};
			Point temp = *this - offset;
			return Point(temp.x * cos_dtheta - temp.y * sin_dtheta, temp.x * sin_dtheta + temp.y * cos_dtheta) + offset;
		}
	};
}

#endif
