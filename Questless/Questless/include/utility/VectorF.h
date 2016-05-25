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
using sdl::Vector;

namespace questless
{
	struct VectorF
	{
		const double tau = 6.283185307179586476925;

		double x;
		double y;

		VectorF() : x(0.0), y(0.0) {}
		VectorF(double x, double y) : x(x), y(y) {}
		VectorF(const VectorF& v) : x(v.x), y(v.y) {}
		VectorF(const Vector& v) : x(v.x), y(v.y) {}

		bool operator ==(const Vector& right) const { return x == right.x && y == right.y; }
		bool operator !=(const Vector& right) const { return x != right.x || y != right.y; }

		friend VectorF operator +(const VectorF& v1, const VectorF& v2);
		friend VectorF operator -(const VectorF& v1, const VectorF& v2);
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
		inline double magnitude() const { return sqrt(x * x + y * y); }

		/// @return The angle of the vector in degrees counter-clockwise from the positive x-axis.
		inline double angle() const { return 360.0 * atan2(y, x) / tau; }

		/// @return A vector with integer coordinates. Floating-point coordinates are rounded to the nearest whole number.
		Vector to_vector() const;
	};
}

#endif