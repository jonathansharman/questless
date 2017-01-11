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

		friend std::ostream& operator <<(std::ostream& out, const Point& p)
		{
			out << '(' << p.x << ", " << p.y << ')';
			return out;
		}

		constexpr bool operator ==(const Point& right) const { return x == right.x && y == right.y; }
		constexpr bool operator !=(const Point& right) const { return x != right.x || y != right.y; }
		
		constexpr friend Point operator +(const Point& p, const Vector<space_t>& v) { return Point{p.x + v.x, p.y + v.y}; }
		constexpr friend Point operator +(const Vector<space_t>& v, Point p) { return Point{v.x + p.x, v.y + p.y}; }
		constexpr friend Point operator -(const Point& p, const Vector<space_t>& v) { return Point{p.x - v.x, p.y - v.y}; }
		constexpr friend Vector<space_t> operator -(const Point& p1, const Point& p2) { return Vector<space_t>{p1.x - p2.x, p1.y - p2.y}; }
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
		template <typename UnitsPerCircle, typename = std::enable_if_t<std::is_floating_point<scalar_t>::value>>
		void rotate(const Point& origin, const Angle<space_t, UnitsPerCircle>& dtheta) &
		{
			const scalar_t dtheta_radians = angle_cast<Radians<space_t>>(dtheta).count();
			const auto cos_dtheta = static_cast<scalar_t>(cos(dtheta_radians));
			const auto sin_dtheta = static_cast<scalar_t>(sin(dtheta_radians));
			auto offset = Vector<space_t>{origin.x, origin.y};
			*this -= offset;
			*this = Point
				{ static_cast<scalar_t>(x * cos_dtheta - y * sin_dtheta)
				, static_cast<scalar_t>(x * sin_dtheta + y * cos_dtheta)
				};
			*this += offset;
		}

		/// Creates a copy of the point, rotated about another point.
		/// @param origin The origin around which the point will be rotated.
		/// @param theta The counter-clockwise rotation to apply, in radians.
		template <typename UnitsPerCircle, typename = std::enable_if_t<std::is_floating_point<scalar_t>::value>>
		Point rotated(const Point& origin, const Angle<space_t, UnitsPerCircle>& dtheta) const /// @todo Cannot be constexpr because of cos() and sin().
		{
			const scalar_t dtheta_radians = angle_cast<Radians<space_t>>(dtheta).count();
			const auto cos_dtheta = static_cast<scalar_t>(cos(dtheta_radians));
			const auto sin_dtheta = static_cast<scalar_t>(sin(dtheta_radians));
			auto offset = Vector<space_t>{origin.x, origin.y};
			Point ret = *this - offset;
			ret = Point
				{ static_cast<scalar_t>(ret.x * cos_dtheta - ret.y * sin_dtheta)
				, static_cast<scalar_t>(ret.x * sin_dtheta + ret.y * cos_dtheta)
				};
			ret += offset;
			return ret;
		}
	};
}

#ifndef _DEBUG
#define DOCTEST_CONFIG_DISABLE
#endif
#include <doctest/doctest.h>

TEST_CASE("[Point] operations")
{
	struct IntSpace
	{
		using scalar_t = int;
	};
	struct DoubleSpace
	{
		using scalar_t = double;
	};

	units::Point<IntSpace> pi1{3, 4};

	SUBCASE("binary operations")
	{
		SUBCASE("addition")
		{
			constexpr units::Vector<IntSpace> vi{1, 6};
			constexpr units::Point<IntSpace> p_sum{4, 10};
			CHECK(pi1 + vi == p_sum);
			pi1 += vi;
			CHECK(pi1 == p_sum);
		}
		SUBCASE("subtraction")
		{
			constexpr units::Point<IntSpace> pi2{1, 6};
			constexpr units::Vector<IntSpace> v_diff{2, -2};
			CHECK(pi1 - pi2 == v_diff);

			constexpr units::Vector<IntSpace> vi{1, 6};
			constexpr units::Point<IntSpace> p_diff{2, -2};
			pi1 -= vi;
			CHECK(pi1 == p_diff);
		}
	}
	SUBCASE("scalar operations")
	{
		constexpr int k = 2;
		SUBCASE("scalar multiplication")
		{
			constexpr units::Point<IntSpace> p_prod{6, 8};
			CHECK(k * pi1 == p_prod);
			CHECK(pi1 * k == p_prod);
			pi1 *= k;
			CHECK(pi1 == p_prod);
		}
		SUBCASE("scalar division")
		{
			constexpr units::Point<IntSpace> p_quot{1, 2};
			CHECK(pi1 / k == p_quot);
			pi1 /= k;
			CHECK(pi1 == p_quot);
		}
	}
	SUBCASE("rotations")
	{
		units::Point<DoubleSpace> pd{3.0, 4.0};
		constexpr units::Degrees<DoubleSpace> dtheta{90.0};
		constexpr units::Point<DoubleSpace> pivot{1.0, 1.0};
		constexpr units::Point<DoubleSpace> pd_rot{-2.0, 3.0};
		auto pd_rot_actual = pd.rotated(pivot, dtheta);

		CHECK(pd_rot_actual.x == doctest::Approx(pd_rot.x));
		CHECK(pd_rot_actual.y == doctest::Approx(pd_rot.y));

		pd.rotate(pivot, dtheta);

		CHECK(pd.x == doctest::Approx(pd_rot.x));
		CHECK(pd.y == doctest::Approx(pd_rot.y));
	}
}

#endif
