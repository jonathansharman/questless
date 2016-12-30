/**
* @file    Vector.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION A generic 2D vector, with parameterized length and space types.
*/

#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

#include "Angle.h"

namespace units
{
	template <typename SpaceType>
	struct Vector
	{
		using space_t = SpaceType;
		using scalar_t = typename space_t::scalar_t;

		scalar_t x;
		scalar_t y;

		constexpr explicit Vector() : x{scalar_t{}}, y{scalar_t{}} {}

		constexpr explicit Vector(scalar_t x, scalar_t y) : x{std::move(x)}, y{std::move(y)} {}

		template <typename UnitsPerCircle>
		explicit Vector(Angle<space_t, UnitsPerCircle> theta, scalar_t r) // Cannot be constexpr because of cos() and sin().
			: x{static_cast<scalar_t>(r * cos(theta.count()))}
			, y{static_cast<scalar_t>(r * sin(theta.count()))}
		{}

		constexpr Vector(const Vector& v) : x{v.x}, y{v.y} {}

		bool operator ==(const Vector& right) const { return x == right.x && y == right.y; }
		bool operator !=(const Vector& right) const { return x != right.x || y != right.y; }

		friend Vector operator +(const Vector& v1, const Vector& v2) { return Vector{v1.x + v2.x, v1.y + v2.y}; }
		friend Vector operator -(const Vector& v1, const Vector& v2) { return Vector{v1.x - v2.x, v1.y - v2.y}; }
		friend Vector operator -(const Vector& v) { return Vector{-v.x, -v.y}; }
		friend Vector operator *(const Vector& v, scalar_t k) { return Vector{k * v.x, k * v.y}; }
		friend Vector operator *(scalar_t k, const Vector& v) { return Vector{k * v.x, k * v.y}; }
		friend Vector operator /(const Vector& v, scalar_t k) { return Vector{v.x / k, v.y / k}; }

		Vector& operator =(const Vector& v) & = default;

		Vector& operator +=(const Vector& v) &
		{
			x += v.x;
			y += v.y;
			return *this;
		}
		Vector& operator -=(const Vector& v) &
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}
		Vector& operator *=(scalar_t k) &
		{
			x *= k;
			y *= k;
			return *this;
		}
		Vector& operator /=(scalar_t k) &
		{
			x /= k;
			y /= k;
			return *this;
		}

		/// Rotates the vector, overwriting the original value.
		/// @param dtheta The counter-clockwise rotation to apply, in radians.
		template <typename UnitsPerCircle>
		void rotate(const Angle<space_t, UnitsPerCircle>& dtheta) &
		{
			auto cos_dtheta = static_cast<scalar_t>(cos(dtheta.count()));
			auto sin_dtheta = static_cast<scalar_t>(sin(dtheta.count()));
			*this = Vector{x * cos_dtheta - y * sin_dtheta, x * sin_dtheta + y * cos_dtheta};
		}

		/// Creates a rotated copy of the vector.
		/// @param dtheta The counter-clockwise rotation to apply, in radians.
		template <typename UnitsPerCircle>
		Vector rotated(const Angle<space_t, UnitsPerCircle>& dtheta) const
		{
			auto cos_dtheta = static_cast<scalar_t>(cos(dtheta.count()));
			auto sin_dtheta = static_cast<scalar_t>(sin(dtheta.count()));
			return Vector{this->x * cos_dtheta - this->y * sin_dtheta, this->x * sin_dtheta + this->y * cos_dtheta};
		}

		/// @return The vector's length.
		scalar_t length() const { return static_cast<scalar_t>(sqrt(x * x + y * y)); }

		/// @return The square of the vector's length.
		scalar_t length_squared() const { return static_cast<scalar_t>(x * x + y * y); }

		/// @return The counter-clockwise angle of the vector from the positive x-axis (in radians by default).
		template <typename UnitsPerCircle = std::ratio<6'283'185'307'179'586'476ull, 1'000'000'000'000'000'000ull>>
		Angle<space_t, UnitsPerCircle> angle() const { return Angle<space_t, UnitsPerCircle>{atan2(y, x)}; }
	};
}

#endif
