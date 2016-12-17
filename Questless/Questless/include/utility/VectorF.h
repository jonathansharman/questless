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
#include "units/geometry.h"

namespace questless
{
	struct VectorF
	{
		double x;
		double y;

		constexpr VectorF() : x{0.0}, y{0.0} {}
		constexpr VectorF(double x, double y) : x{x}, y{y} {}
		VectorF(AngleRadians theta, Length r) : x{r * cos(theta)}, y{r * sin(theta)} {} // Cannot be constexpr because of cos() and sin().
		constexpr VectorF(const VectorF& v) : x{v.x}, y{v.y} {}
		constexpr explicit VectorF(const sdl::Vector& v) : x{static_cast<double>(v.x)}, y{static_cast<double>(v.y)} {}

		static constexpr VectorF zero() { return VectorF{}; }

		bool operator ==(const sdl::Vector& right) const { return x == right.x && y == right.y; }
		bool operator !=(const sdl::Vector& right) const { return x != right.x || y != right.y; }

		friend VectorF operator +(const VectorF& v1, const VectorF& v2) { return VectorF{v1.x + v2.x, v1.y + v2.y}; }
		friend VectorF operator -(const VectorF& v1, const VectorF& v2) { return VectorF{v1.x - v2.x, v1.y - v2.y}; }
		friend VectorF operator -(const VectorF& v) { return VectorF{-v.x, -v.y}; }
		friend VectorF operator *(VectorF v, double factor) { return VectorF{factor * v.x, factor * v.y}; }
		friend VectorF operator *(double factor, VectorF v) { return VectorF{factor * v.x, factor * v.y}; }
		friend VectorF operator /(VectorF v, double divisor) { return VectorF{v.x / divisor, v.y / divisor}; }

		VectorF& operator =(const VectorF& v) & = default;

		VectorF& operator +=(const VectorF& v) &
		{
			x += v.x;
			y += v.y;
			return *this;
		}
		VectorF& operator -=(const VectorF& v) &
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}
		VectorF& operator *=(double factor) &
		{
			x *= factor;
			y *= factor;
			return *this;
		}
		VectorF& operator /=(double divisor) &
		{
			x /= divisor;
			y /= divisor;
			return *this;
		}

		/// Rotates the vector, overwriting the original value.
		/// @param dtheta The counter-clockwise rotation to apply, in radians.
		void rotate(AngleRadians dtheta)
		{
			double cos_dtheta = cos(dtheta);
			double sin_dtheta = sin(dtheta);
			*this = VectorF{x * cos_dtheta - y * sin_dtheta, x * sin_dtheta + y * cos_dtheta};
		}

		/// Creates a rotated copy of the vector.
		/// @param dtheta The counter-clockwise rotation to apply, in radians.
		VectorF rotated(AngleRadians dtheta) const
		{
			double cos_dtheta = cos(dtheta);
			double sin_dtheta = sin(dtheta);
			return VectorF{this->x * cos_dtheta - this->y * sin_dtheta, this->x * sin_dtheta + this->y * cos_dtheta};
		}

		/// @return The vector's length.
		Length length() const { return Length{sqrt(x * x + y * y)}; }

		/// @return The square of the vector's length.
		LengthSquared length_squared() const { return LengthSquared{x * x + y * y}; }

		/// @return The counter-clockwise angle of the vector from the positive x-axis, in radians.
		AngleRadians angle() const { return AngleRadians{atan2(y, x)}; }

		/// @return A vector with integer coordinates. Floating-point coordinates are rounded to the nearest whole number.
		sdl::Vector to_vector() const { return sdl::Vector{lround(x), lround(y)}; }
	};
}

#endif
