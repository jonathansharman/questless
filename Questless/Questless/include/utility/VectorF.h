/**
* @file    VectorF.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for VectorF, a struct that holds 2D double-precision floating-point vector.
*/

#ifndef VECTORF_H
#define VECTORF_H

#include <cmath>

#include "sdl-wrappers/basic-sdl-wrappers.h"

/// @todo Modernize and inline.

namespace questless
{
	struct PointF;

	struct VectorF
	{
		double x;
		double y;

		/// @return A vector from the origin (0, 0) to the given point.
		static constexpr VectorF to(PointF p);

		constexpr VectorF() : x(0.0), y(0.0) {}
		constexpr VectorF(double x, double y) : x(x), y(y) {}
		constexpr VectorF(const VectorF& v) : x(v.x), y(v.y) {}
		constexpr explicit VectorF(const sdl::Vector& v) : x(v.x), y(v.y) {}

		bool operator ==(const sdl::Vector& right) const { return x == right.x && y == right.y; }
		bool operator !=(const sdl::Vector& right) const { return x != right.x || y != right.y; }

		friend VectorF operator +(const VectorF& v1, const VectorF& v2);
		friend VectorF operator -(const VectorF& v1, const VectorF& v2);
		friend VectorF operator -(const VectorF& v) { return VectorF{-v.x, -v.y}; }
		friend VectorF operator *(VectorF v, double factor);
		friend VectorF operator *(double factor, VectorF v);
		friend VectorF operator /(VectorF v, double divisor);
	
		VectorF& operator =(const VectorF& v);

		VectorF& operator +=(const VectorF& v);
		VectorF& operator -=(const VectorF& v);
		VectorF& operator *=(double factor);
		VectorF& operator /=(double divisor);

		/// Rotates the vector, overwriting the original value.
		/// @param theta The counter-clockwise rotation to apply, in degrees.
		void rotate(double theta);

		/// Creates a rotated copy of the vector.
		/// @param theta The counter-clockwise rotation to apply, in degrees.
		VectorF rotated(double theta) const;

		/// @return The vector's magnitude.
		double magnitude() const { return sqrt(x * x + y * y); }

		/// @return The angle of the vector in degrees counter-clockwise from the positive x-axis.
		double angle() const;

		/// @return A vector with integer coordinates. Floating-point coordinates are rounded to the nearest whole number.
		sdl::Vector to_vector() const { return sdl::Vector{lround(x), lround(y)}; }
	};
}

#endif