//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Vector.h"

namespace units
{
	//! A 2D point in @p SpaceType space.
	template <typename SpaceType>
	struct Point
	{
		using space_t = SpaceType;
		using scalar_t = typename space_t::scalar_t;

		scalar_t x;
		scalar_t y;

		constexpr explicit Point() : x{scalar_t{}}, y{scalar_t{}} {}
		constexpr explicit Point(scalar_t x, scalar_t y) : x{x}, y{y} {}
		constexpr Point(Point const& that) : x{that.x}, y{that.y} {}

		friend std::ostream& operator <<(std::ostream& out, Point const& p)
		{
			out << '(' << p.x << ", " << p.y << ')';
			return out;
		}

		constexpr bool operator ==(Point const& that) const { return x == that.x && y == that.y; }
		constexpr bool operator !=(Point const& that) const { return x != that.x || y != that.y; }
		
		constexpr friend Point operator +(Point const& p, Vector<space_t> const& v) { return Point{p.x + v.x, p.y + v.y}; }
		constexpr friend Point operator +(Vector<space_t> const& v, Point p) { return Point{v.x + p.x, v.y + p.y}; }
		constexpr friend Point operator -(Point const& p, Vector<space_t> const& v) { return Point{p.x - v.x, p.y - v.y}; }
		constexpr friend Vector<space_t> operator -(Point const& p1, Point const& p2) { return Vector<space_t>{p1.x - p2.x, p1.y - p2.y}; }

		Point& operator =(Point const&) & = default;

		Point& operator +=(Vector<space_t> const& v) &
		{
			x += v.x;
			y += v.y;
			return *this;
		}
		Point& operator -=(Vector<space_t> const& v) &
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}

		//! Rotates the point about another point, overwriting the original value.
		//! @param origin The origin around which the point will be rotated.
		//! @param dtheta The counter-clockwise rotation to apply, in radians.
		template <typename UnitsPerCircle, typename = std::enable_if_t<std::is_floating_point<scalar_t>::value>>
		void rotate(Point const& origin, Angle<space_t, UnitsPerCircle> const& dtheta) &
		{
			scalar_t const dtheta_radians = angle_cast<Radians<space_t>>(dtheta).count();
			auto const cos_dtheta = static_cast<scalar_t>(cos(dtheta_radians));
			auto const sin_dtheta = static_cast<scalar_t>(sin(dtheta_radians));
			auto offset = Vector<space_t>{origin.x, origin.y};
			*this -= offset;
			*this = Point
				{ static_cast<scalar_t>(x * cos_dtheta - y * sin_dtheta)
				, static_cast<scalar_t>(x * sin_dtheta + y * cos_dtheta)
				};
			*this += offset;
		}

		//! Creates a copy of the point, rotated about another point.
		//! @param origin The origin around which the point will be rotated.
		//! @param theta The counter-clockwise rotation to apply, in radians.
		template <typename UnitsPerCircle, typename = std::enable_if_t<std::is_floating_point<scalar_t>::value>>
		Point rotated(Point const& origin, Angle<space_t, UnitsPerCircle> const& dtheta) const //! @todo Cannot be constexpr because of cos() and sin().
		{
			scalar_t const dtheta_radians = angle_cast<Radians<space_t>>(dtheta).count();
			auto const cos_dtheta = static_cast<scalar_t>(cos(dtheta_radians));
			auto const sin_dtheta = static_cast<scalar_t>(sin(dtheta_radians));
			auto offset = Vector<space_t>{origin.x, origin.y};
			Point result = *this - offset;
			result = Point
				{ static_cast<scalar_t>(result.x * cos_dtheta - result.y * sin_dtheta)
				, static_cast<scalar_t>(result.x * sin_dtheta + result.y * cos_dtheta)
				};
			result += offset;
			return result;
		}
	};
}

#ifndef _DEBUG
#define DOCTEST_CONFIG_DISABLE
#endif
#include <doctest.h>

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
