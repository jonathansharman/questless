//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <ratio>
#include <ostream>

#include "constants.h"

namespace units
{
	//! An angle in some space.
	//! @tparam SpaceType The space type which contains angles of this type.
	//! @tparam UnitsPerCircle The number of angle units per full circle, as a std::ratio.
	template <typename SpaceType, typename UnitsPerCircle>
	class Angle
	{
	public:
		using space_t = SpaceType;
		using units_per_circle = UnitsPerCircle;
		using scalar_t = typename space_t::scalar_t;

		//! The number of this angle unit in one full circle.
		static constexpr Angle circle() { return Angle{static_cast<scalar_t>(units_per_circle::num) / static_cast<scalar_t>(units_per_circle::den)}; }

		constexpr explicit Angle() : _count{scalar_t{}} {}

		constexpr explicit Angle(scalar_t count) : _count{std::move(count)} {}

		//! The zero angle for the specified space and unit size, constructed using the default constructor of the scalar type.
		static constexpr Angle zero() { return Angle{scalar_t{}}; }

		constexpr scalar_t const& count() const& { return _count; }
		scalar_t&& count() && { return std::move(_count); }
		scalar_t& count() & { return _count; }

		friend std::ostream& operator <<(std::ostream& out, Angle const& theta)
		{
			out << theta.count();
			return out;
		}

		constexpr bool operator <(Angle const& that) const { return _count < that._count; }
		constexpr bool operator <=(Angle const& that) const { return _count <= that._count; }
		constexpr bool operator ==(Angle const& that) const { return _count == that._count; }
		constexpr bool operator !=(Angle const& that) const { return _count != that._count; }
		constexpr bool operator >=(Angle const& that) const { return _count >= that._count; }
		constexpr bool operator >(Angle const& that) const { return _count > that._count; }

		// angle + angle -> angle
		constexpr friend Angle operator +(Angle const& theta1, Angle const& theta2)
		{
			return Angle{theta1.count() + theta2.count()};
		}

		// angle - angle -> angle
		constexpr friend Angle operator -(Angle const& theta1, Angle const& theta2)
		{
			return Angle<SpaceType, UnitsPerCircle>{theta1.count() - theta2.count()};
		}

		// -angle -> angle
		constexpr friend Angle operator -(Angle const& theta)
		{
			return Angle<SpaceType, UnitsPerCircle>{-theta.count()};
		}

		// k * angle -> angle
		constexpr friend Angle operator *(scalar_t k, Angle const& theta)
		{
			return Angle<SpaceType, UnitsPerCircle>{k * theta.count()};
		}
		// angle * k -> angle
		constexpr friend Angle operator *(Angle const& theta, scalar_t k)
		{
			return Angle<SpaceType, UnitsPerCircle>{k * theta.count()};
		}

		// angle / k -> angle
		constexpr friend Angle operator /(Angle const& theta, scalar_t k)
		{
			return Angle<SpaceType, UnitsPerCircle>{theta.count() / k};
		}

		// Closed under addition.
		Angle& operator +=(Angle const& theta) &
		{
			_count += theta._count;
			return *this;
		}

		// Closed under subtraction.
		Angle& operator -=(Angle const& theta) &
		{
			_count -= theta._count;
			return *this;
		}

		// Closed under scalar multiplication.
		Angle& operator *=(scalar_t k) &
		{
			_count *= k;
			return *this;
		}

		// Closed under scalar division.
		Angle& operator /=(scalar_t k) &
		{
			_count /= k;
			return *this;
		}
	private:
		scalar_t _count;
	};

	//! Casts @p theta from SpaceType::Angle<UnitsPerCircle> to @p ToAngle.
	template<typename ToAngle, typename SpaceType, typename UnitsPerCircle>
	constexpr ToAngle angle_cast(Angle<SpaceType, UnitsPerCircle> const& theta)
	{
		using from_angle_t = Angle<SpaceType, UnitsPerCircle>;
		using ratio = std::ratio_divide<ToAngle::units_per_circle, UnitsPerCircle>;
		return ToAngle{static_cast<ToAngle::scalar_t>(theta.count() * static_cast<from_angle_t::scalar_t>(ratio::num) / static_cast<from_angle_t::scalar_t>(ratio::den))};
	}

	template <typename SpaceType>
	using Radians = Angle<SpaceType, std::ratio<static_cast<int>(constants::tau * 1'000'000), 1'000'000>>;

	template <typename SpaceType>
	using Degrees = Angle<SpaceType, std::ratio<360>>;

	template <typename SpaceType>
	using Gradians = Angle<SpaceType, std::ratio<400>>;
}
