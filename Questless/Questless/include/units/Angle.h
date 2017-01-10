/**
* @file    Angle.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Represents an angle in some space.
*/

#ifndef ANGLE_H
#define ANGLE_H

#include <ratio>
#include <ostream>

namespace units
{
	/// @tparam SpaceType The space type which contains angles of this type.
	/// @tparam UnitsPerCircle The number of angle units per full circle, as a std::ratio.
	template <typename SpaceType, typename UnitsPerCircle>
	class Angle
	{
	public:
		using space_t = SpaceType;
		using units_per_circle = UnitsPerCircle;
		using scalar_t = typename space_t::scalar_t;

		/// The number of this angle unit in one full circle.
		static constexpr Angle circle() { return Angle{static_cast<scalar_t>(units_per_circle::num) / static_cast<scalar_t>(units_per_circle::den)}; }

		constexpr explicit Angle() : _count{scalar_t{}} {}

		constexpr explicit Angle(scalar_t count) : _count{std::move(count)} {}

		/// @return The zero angle for the specified space and unit size, constructed using the default constructor of the scalar type.
		constexpr static Angle<SpaceType, UnitsPerCircle> zero() { return Angle<SpaceType, UnitsPerCircle>{scalar_t{}}; }

		constexpr const scalar_t& count() const & { return _count; }
		scalar_t&& count() && { return std::move(_count); }
		scalar_t& count() & { return _count; }

		friend std::ostream& operator <<(std::ostream& out, const Angle& theta)
		{
			out << theta.count();
			return out;
		}

		constexpr bool operator <(const Angle<space_t, units_per_circle>& right) const { return _count < right._count; }
		constexpr bool operator <=(const Angle<space_t, units_per_circle>& right) const { return _count <= right._count; }
		constexpr bool operator ==(const Angle<space_t, units_per_circle>& right) const { return _count == right._count; }
		constexpr bool operator !=(const Angle<space_t, units_per_circle>& right) const { return _count != right._count; }
		constexpr bool operator >=(const Angle<space_t, units_per_circle>& right) const { return _count >= right._count; }
		constexpr bool operator >(const Angle<space_t, units_per_circle>& right) const { return _count > right._count; }

		// Closed under addition.
		Angle<space_t, units_per_circle>& operator +=(const Angle<space_t, units_per_circle>& theta) &
		{
			_count += theta._count;
			return *this;
		}

		// Closed under subtraction.
		Angle<space_t, units_per_circle>& operator -=(const Angle<space_t, units_per_circle>& theta) &
		{
			_count -= theta._count;
			return *this;
		}

		// Closed under scalar multiplication.
		Angle<space_t, units_per_circle>& operator *=(scalar_t k) &
		{
			_count *= k;
			return *this;
		}

		// Closed under scalar division.
		Angle<space_t, units_per_circle>& operator /=(scalar_t k) &
		{
			_count /= k;
			return *this;
		}
	private:
		scalar_t _count;
	};

	// angle + angle -> angle
	template <typename SpaceType, typename UnitsPerCircle>
	constexpr Angle<SpaceType, UnitsPerCircle> operator +(const Angle<SpaceType, UnitsPerCircle>& theta1, const Angle<SpaceType, UnitsPerCircle>& theta2)
	{
		return Angle<SpaceType, UnitsPerCircle>{theta1.count() + theta2.count()};
	}

	// angle - angle -> angle
	template <typename SpaceType, typename UnitsPerCircle>
	constexpr Angle<SpaceType, UnitsPerCircle> operator -(const Angle<SpaceType, UnitsPerCircle>& theta1, const Angle<SpaceType, UnitsPerCircle>& theta2)
	{
		return Angle<SpaceType, UnitsPerCircle>{theta1.count() - theta2.count()};
	}

	// -angle -> angle
	template <typename SpaceType, typename UnitsPerCircle>
	constexpr Angle<SpaceType, UnitsPerCircle> operator -(const Angle<SpaceType, UnitsPerCircle>& theta)
	{
		return Angle<SpaceType, UnitsPerCircle>{-theta.count()};
	}

	// k * angle -> angle
	template <typename SpaceType, typename UnitsPerCircle>
	constexpr Angle<SpaceType, UnitsPerCircle> operator *(typename SpaceType::scalar_t k, const Angle<SpaceType, UnitsPerCircle>& theta)
	{
		return Angle<SpaceType, UnitsPerCircle>{k * theta.count()};
	}
	// angle * k -> angle
	template <typename SpaceType, typename UnitsPerCircle>
	constexpr Angle<SpaceType, UnitsPerCircle> operator *(const Angle<SpaceType, UnitsPerCircle>& theta, typename SpaceType::scalar_t k)
	{
		return Angle<SpaceType, UnitsPerCircle>{k * theta.count()};
	}

	// angle / k -> angle
	template <typename SpaceType, typename UnitsPerCircle>
	constexpr Angle<SpaceType, UnitsPerCircle> operator /(const Angle<SpaceType, UnitsPerCircle>& theta, typename SpaceType::scalar_t k)
	{
		return Angle<SpaceType, UnitsPerCircle>{theta.count() / k};
	}

	template<typename ToAngle, typename SpaceType, typename UnitsPerCircle>
	constexpr ToAngle angle_cast(const Angle<SpaceType, UnitsPerCircle>& theta)
	{
		using from_angle_t = Angle<SpaceType, UnitsPerCircle>;
		using ratio = std::ratio_divide<ToAngle::units_per_circle, UnitsPerCircle>;
		return ToAngle{static_cast<ToAngle::scalar_t>(theta.count() * static_cast<from_angle_t::scalar_t>(ratio::num) / static_cast<from_angle_t::scalar_t>(ratio::den))};
	}

	template <typename SpaceType>
	using Radians = Angle<SpaceType, std::ratio<6'283'185/*'307'179'586ull*/, 1'000'000/*'000'000'000ull*/>>;

	template <typename SpaceType>
	using Degrees = Angle<SpaceType, std::ratio<360>>;

	template <typename SpaceType>
	using Gradians = Angle<SpaceType, std::ratio<400>>;
}

#include <doctest/doctest.h>

TEST_CASE("[Angle] operations")
{
	struct DoubleSpace
	{
		using scalar_t = double;
	};

	SUBCASE("conversions")
	{
		constexpr auto circle_degrees = units::angle_cast<units::Degrees<DoubleSpace>>(units::Radians<DoubleSpace>::circle());
		CHECK(circle_degrees.count() == doctest::Approx(360.0));
	}
}

#endif