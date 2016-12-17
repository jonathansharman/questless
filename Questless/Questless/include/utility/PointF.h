/**
* @file    PointF.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for PointF, a struct that holds a 2D double-precision floating-point point.
*/

#ifndef POINTF_H
#define POINTF_H

#include "sdl-wrappers/basic-sdl-wrappers.h"
#include "utility.h"
#include "VectorF.h"

namespace questless
{
	struct PointF
	{
		double x;
		double y;

		constexpr PointF() : x{0.0}, y{0.0} {}
		constexpr PointF(double x, double y) : x{x}, y{y} {}
		constexpr PointF(const PointF& p) : x{p.x}, y{p.y} {}
		constexpr explicit PointF(const sdl::Point& p) : x(p.x), y(p.y) {}

		constexpr bool operator ==(const sdl::Vector& right) const { return x == right.x && y == right.y; }
		constexpr bool operator !=(const sdl::Vector& right) const { return x != right.x || y != right.y; }
		
		constexpr friend PointF operator +(PointF p, const VectorF& v) { return PointF{p.x + v.x, p.y + v.y}; }
		constexpr friend PointF operator +(const VectorF& v, PointF p) { return PointF{v.x + p.x, v.y + p.y}; }
		constexpr friend PointF operator -(PointF p, const VectorF& v) { return PointF{p.x - v.x, p.y - v.y}; }
		constexpr friend VectorF operator -(const PointF& p1, const PointF& p2) { return VectorF{p1.x - p2.x, p1.y - p2.y}; }
		constexpr friend PointF operator *(PointF p, double k) { return PointF{k * p.x, k * p.y}; }
		constexpr friend PointF operator *(double k, PointF p) { return PointF{k * p.x, k * p.y}; }
		constexpr friend PointF operator /(PointF p, double k) { return PointF{p.x / k, p.y / k}; }

		PointF& operator =(const PointF& p) & = default;

		PointF& operator +=(const VectorF& v) &
		{
			x += v.x;
			y += v.y;
			return *this;
		}
		PointF& operator -=(const VectorF& v) &
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}
		PointF& operator *=(double factor) &
		{
			x *= factor;
			y *= factor;
			return *this;
		}
		PointF& operator /=(double divisor) &
		{
			x /= divisor;
			y /= divisor;
			return *this;
		}

		/// Rotates the point about another point, overwriting the original value.
		/// @param origin The origin around which the point will be rotated.
		/// @param dtheta The counter-clockwise rotation to apply, in radians.
		void rotate(const PointF& origin, AngleRadians dtheta) &
		{
			double cos_dtheta = cos(dtheta);
			double sin_dtheta = sin(dtheta);
			VectorF offset = VectorF(origin.x, origin.y);
			*this -= offset;
			*this = PointF(x * cos_dtheta - y * sin_dtheta, x * sin_dtheta + y * cos_dtheta) + offset;
		}

		/// Creates a copy of the point, rotated about another point.
		/// @param origin The origin around which the point will be rotated.
		/// @param theta The counter-clockwise rotation to apply, in radians.
		PointF rotated(const PointF& origin, AngleRadians dtheta) const
		{
			double cos_dtheta = cos(dtheta);
			double sin_dtheta = sin(dtheta);
			VectorF offset = VectorF(origin.x, origin.y);
			PointF temp = *this - offset;
			return PointF(temp.x * cos_dtheta - temp.y * sin_dtheta, temp.x * sin_dtheta + temp.y * cos_dtheta) + offset;
		}

		/// @return A point with integer coordinates. Floating-point coordinates are rounded to the nearest whole number.
		sdl::Point to_point() const { return sdl::Point(lround(x), lround(y)); }
	};
}

#endif
