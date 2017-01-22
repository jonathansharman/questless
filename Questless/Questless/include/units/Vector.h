/**
* @file    Vector.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION A generic 2D vector, with parameterized length and space types.
*/

#pragma once

#include <cmath>
#include <ostream>

#include "Angle.h"
#include "constexpr-math.h"

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

		template <typename UnitsPerCircle, typename = std::enable_if_t<std::is_floating_point<scalar_t>::value>>
		explicit Vector(Angle<space_t, UnitsPerCircle> theta, scalar_t r) /// @todo Cannot be constexpr because of cos() and sin().
			: x{static_cast<scalar_t>(r * cos(theta.count()))}
			, y{static_cast<scalar_t>(r * sin(theta.count()))}
		{}

		constexpr Vector(Vector const& v) : x{v.x}, y{v.y} {}

		friend std::ostream& operator <<(std::ostream& out, Vector const& v)
		{
			out << '(' << v.x << ", " << v.y << ')';
			return out;
		}

		bool operator ==(Vector const& right) const { return x == right.x && y == right.y; }
		bool operator !=(Vector const& right) const { return x != right.x || y != right.y; }

		friend Vector operator +(Vector const& v1, Vector const& v2) { return Vector{v1.x + v2.x, v1.y + v2.y}; }
		friend Vector operator -(Vector const& v1, Vector const& v2) { return Vector{v1.x - v2.x, v1.y - v2.y}; }
		friend Vector operator -(Vector const& v) { return Vector{-v.x, -v.y}; }
		friend Vector operator *(Vector const& v, scalar_t k) { return Vector{k * v.x, k * v.y}; }
		friend Vector operator *(scalar_t k, Vector const& v) { return Vector{k * v.x, k * v.y}; }
		friend Vector operator /(Vector const& v, scalar_t k) { return Vector{v.x / k, v.y / k}; }

		Vector& operator =(Vector const& v) & = default;

		Vector& operator +=(Vector const& v) &
		{
			x += v.x;
			y += v.y;
			return *this;
		}
		Vector& operator -=(Vector const& v) &
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
		template <typename UnitsPerCircle, typename = std::enable_if_t<std::is_floating_point<scalar_t>::value>>
		void rotate(Angle<space_t, UnitsPerCircle> const& dtheta) &
		{
			scalar_t const dtheta_radians = angle_cast<Radians<space_t>>(dtheta).count();
			auto const cos_dtheta = static_cast<scalar_t>(cos(dtheta_radians));
			auto const sin_dtheta = static_cast<scalar_t>(sin(dtheta_radians));
			*this = Vector
				{ static_cast<scalar_t>(x * cos_dtheta - y * sin_dtheta)
				, static_cast<scalar_t>(x * sin_dtheta + y * cos_dtheta)
				};
		}

		/// Creates a rotated copy of the vector.
		/// @param dtheta The counter-clockwise rotation to apply, in radians.
		template <typename UnitsPerCircle, typename = std::enable_if_t<std::is_floating_point<scalar_t>::value>>
		Vector rotated(Angle<space_t, UnitsPerCircle> const& dtheta) const /// @todo Cannot be constexpr because of cos() and sin().
		{
			scalar_t const dtheta_radians = angle_cast<Radians<space_t>>(dtheta).count();
			auto const cos_dtheta = static_cast<scalar_t>(cos(dtheta_radians));
			auto const sin_dtheta = static_cast<scalar_t>(sin(dtheta_radians));
			return Vector
				{ static_cast<scalar_t>(x * cos_dtheta - y * sin_dtheta)
				, static_cast<scalar_t>(x * sin_dtheta + y * cos_dtheta)
				};
		}

		/// @return The vector's length.
		constexpr scalar_t length() const { return static_cast<scalar_t>(constexpr_math::sqrt(x * x + y * y)); }

		/// @return The square of the vector's length.
		constexpr scalar_t length_squared() const { return x * x + y * y; }

		/// @return The counter-clockwise angle of the vector from the positive x-axis (in radians by default).
		template <typename UnitsPerCircle = Radians<space_t>::units_per_circle, typename = std::enable_if_t<std::is_floating_point<scalar_t>::value>>
		Angle<space_t, UnitsPerCircle> angle() const /// @todo Cannot be constexpr because of atan2().
		{
			return Angle<space_t, UnitsPerCircle>{static_cast<scalar_t>(atan2(y, x))};
		}
	};
}

#ifndef _DEBUG
#define DOCTEST_CONFIG_DISABLE
#endif
#include <doctest/doctest.h>

TEST_CASE("[Vector] operations")
{
	struct IntSpace
	{
		using scalar_t = int;
	};
	struct DoubleSpace
	{
		using scalar_t = double;
	};

	units::Vector<IntSpace> vi1{3, 4};

	SUBCASE("length")
	{
		CHECK(vi1.length() == 5);
		CHECK(vi1.length_squared() == 25);
	}
	SUBCASE("angle")
	{
		constexpr units::Vector<DoubleSpace> vd{3.0, 4.0};
		CHECK(vd.angle().count() == doctest::Approx(0.927295218));
	}
	SUBCASE("binary operations")
	{
		constexpr units::Vector<IntSpace> vi2{1, 6};
		SUBCASE("addition")
		{
			constexpr units::Vector<IntSpace> v_sum{4, 10};
			CHECK(vi1 + vi2 == v_sum);
			vi1 += vi2;
			CHECK(vi1 == v_sum);
		}
		SUBCASE("subtraction")
		{
			constexpr units::Vector<IntSpace> v_diff{2, -2};
			CHECK(vi1 - vi2 == v_diff);
			vi1 -= vi2;
			CHECK(vi1 == v_diff);
		}
	}
	SUBCASE("scalar operations")
	{
		constexpr int k = 2;
		SUBCASE("scalar multiplication")
		{
			constexpr units::Vector<IntSpace> v_prod{6, 8};
			CHECK(k * vi1 == v_prod);
			CHECK(vi1 * k == v_prod);
			vi1 *= k;
			CHECK(vi1 == v_prod);
		}
		SUBCASE("scalar division")
		{
			constexpr units::Vector<IntSpace> v_quot{1, 2};
			CHECK(vi1 / k == v_quot);
			vi1 /= k;
			CHECK(vi1 == v_quot);
		}
	}
	SUBCASE("rotations")
	{
		units::Vector<DoubleSpace> vd{3.0, 4.0};
		constexpr units::Degrees<DoubleSpace> dtheta{90.0};
		constexpr units::Vector<DoubleSpace> vd_rot{-4.0, 3.0};
		auto vd_rot_actual = vd.rotated(dtheta);

		CHECK(vd_rot_actual.x == doctest::Approx(vd_rot.x));
		CHECK(vd_rot_actual.y == doctest::Approx(vd_rot.y));

		vd.rotate(dtheta);

		CHECK(vd.x == doctest::Approx(vd_rot.x));
		CHECK(vd.y == doctest::Approx(vd_rot.y));
	}
}
