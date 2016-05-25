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
using sdl::Point;
#include "utility.h"
#include "VectorF.h"

namespace questless
{
	struct PointF
	{
		double x;
		double y;

		PointF() : x(0.0), y(0.0) {}
		PointF(double x, double y) : x(x), y(y) {}
		PointF(const PointF& p) : x(p.x), y(p.y) {}
		PointF(const Point& p) : x(p.x), y(p.y) {}

		bool operator ==(const Vector& right) const { return x == right.x && y == right.y; }
		bool operator !=(const Vector& right) const { return x != right.x || y != right.y; }
		
		friend PointF operator +(PointF p, const VectorF& v);
		friend PointF operator +(const VectorF& v, PointF p);
		friend PointF operator -(PointF p, const VectorF& v);
		friend VectorF operator -(const PointF& p1, const PointF& p2);
		friend PointF operator *(PointF p, double factor);
		friend PointF operator *(double factor, PointF p);
		friend PointF operator /(PointF p, double divisor);

		PointF& operator +=(const VectorF& v);
		PointF& operator -=(const VectorF& v);
		PointF& operator *=(double factor);
		PointF& operator /=(double divisor);

		/// Rotates the point about another point, overwriting the original value.
		/// @param origin The origin around which the point will be rotated.
		/// @param theta The counter-clockwise rotation to apply, in degrees.
		void rotate(const PointF& origin, double theta);

		/// Creates a copy of the point, rotated about another point.
		/// @param origin The origin around which the point will be rotated.
		/// @param theta The counter-clockwise rotation to apply, in degrees.
		PointF rotated(const PointF& origin, double theta) const;

		/// @return A point with integer coordinates. Floating-point coordinates are rounded to the nearest whole number.
		Point to_point() const;
	};
}

#endif