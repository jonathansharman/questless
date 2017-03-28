/**
* @file    Rate.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION A rate data type designed to complement std::chrono::duration.
*/

#pragma once

#include "Frequency.h"

namespace units
{
	template <typename QuantityType, typename TimeRep, typename Period = std::ratio<1>>
	class Rate
	{
	public:
		using quantity_t = QuantityType;
		using time_rep = TimeRep;
		using period = Period;

		constexpr Rate() : _step{quantity_t{}} {}

		/// @param step The amount of the quantity type per unit period.
		constexpr explicit Rate(quantity_t step) : _step{std::move(step)} {}

		/// @return The zero rate for the specified representation and period, constructed using the default constructor of the quantity type.
		static constexpr Rate<quantity_t, time_rep, period> zero() { return Rate<quantity_t, time_rep, period>{quantity_t{}}; }

		/// @return The amount of the quantity type per unit period.
		constexpr quantity_t const& step() const& { return _step; }
		/// @return The amount of the quantity type per unit period.
		quantity_t&& step() && { return std::move(_step); }
		/// @return The amount of the quantity type per unit period.
		quantity_t& step() & { return _step; }

		constexpr bool operator <(Rate<quantity_t, time_rep, period> const& that) const { return _step < that._step; }
		constexpr bool operator <=(Rate<quantity_t, time_rep, period> const& that) const { return _step <= that._step; }
		constexpr bool operator ==(Rate<quantity_t, time_rep, period> const& that) const { return _step == that._step; }
		constexpr bool operator !=(Rate<quantity_t, time_rep, period> const& that) const { return _step != that._step; }
		constexpr bool operator >=(Rate<quantity_t, time_rep, period> const& that) const { return _step >= that._step; }
		constexpr bool operator >(Rate<quantity_t, time_rep, period> const& that) const { return _step > that._step; }

		// Closed under addition.
		Rate<quantity_t, time_rep, period>& operator +=(Rate<quantity_t, time_rep, period> const& rate) &
		{
			_step += rate._step;
			return *this;
		}

		// Closed under subtraction.
		Rate<quantity_t, time_rep, period>& operator -=(Rate<quantity_t, time_rep, period> const& rate) &
		{
			_step -= rate._step;
			return *this;
		}

		// Closed under scalar multiplication.
		Rate<quantity_t, time_rep, period>& operator *=(time_rep const& k) &
		{
			_step *= k;
			return *this;
		}

		// Closed under scalar division.
		Rate<quantity_t, time_rep, period>& operator /=(time_rep const& k) &
		{
			_step /= k;
			return *this;
		}
	private:
		quantity_t _step; ///> The amount of the quantity type per unit period.
	};

	// rate + rate -> rate
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr Rate<QuantityType, TimeRep, Period> operator +(Rate<QuantityType, TimeRep, Period> const& rate1, Rate<QuantityType, TimeRep, Period> const& rate2)
	{
		return Rate<QuantityType, TimeRep, Period>{rate1.step() + rate2.step()};
	}

	// rate - rate -> rate
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr Rate<QuantityType, TimeRep, Period> operator -(Rate<QuantityType, TimeRep, Period> const& rate1, Rate<QuantityType, TimeRep, Period> const& rate2)
	{
		return Rate<QuantityType, TimeRep, Period>{rate1.step() - rate2.step()};
	}

	// -rate -> rate
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr Rate<QuantityType, TimeRep, Period> operator -(Rate<QuantityType, TimeRep, Period> const& rate)
	{
		return Rate<QuantityType, TimeRep, Period>{-rate.step()};
	}

	// k * rate -> rate
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr Rate<QuantityType, TimeRep, Period> operator *(TimeRep const& k, Rate<QuantityType, TimeRep, Period> const& rate)
	{
		return Rate<QuantityType, TimeRep, Period>{k * rate.step()};
	}
	// rate * k -> rate
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr Rate<QuantityType, TimeRep, Period> operator *(Rate<QuantityType, TimeRep, Period> const& rate, TimeRep const& k)
	{
		return Rate<QuantityType, TimeRep, Period>{k * rate.step()};
	}

	// rate * duration -> quantity
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr QuantityType operator *(Rate<QuantityType, TimeRep, Period> const& rate, std::chrono::duration<TimeRep, Period> const& duration)
	{
		return rate.step() * duration.count();
	}
	// duration * rate -> quantity
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr QuantityType operator *(std::chrono::duration<TimeRep, Period> const& duration, Rate<QuantityType, TimeRep, Period> const& rate)
	{
		return rate.step() * duration.count();
	}

	// quantity * frequency -> rate
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr Rate<QuantityType, TimeRep, Period> operator *(QuantityType const& quantity, Frequency<TimeRep, Period> f)
	{
		return Rate<QuantityType, TimeRep, Period>{quantity * f.count()};
	}
	// frequency * quantity -> rate
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr Rate<QuantityType, TimeRep, Period> operator *(Frequency<TimeRep, Period> f, QuantityType const& quantity)
	{
		return Rate<QuantityType, TimeRep, Period>{quantity * f.count()};
	}

	// rate / k -> rate
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr Rate<QuantityType, TimeRep, Period> operator /(Rate<QuantityType, TimeRep, Period> const& rate, TimeRep const& k)
	{
		return Rate<QuantityType, TimeRep, Period>{rate.step() / k};
	}

	// rate / quantity -> frequency
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr Frequency<TimeRep, Period> operator /(Rate<QuantityType, TimeRep, Period> const& rate, QuantityType const& quantity)
	{
		return Frequency<TimeRep, Period>{rate.step() / quantity};
	}

	// rate / frequency -> quantity
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr QuantityType operator /(Rate<QuantityType, TimeRep, Period> const& rate, Frequency<TimeRep, Period> f)
	{
		return rate.step() / f.count();
	}

	// quantity / rate -> duration
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr std::chrono::duration<TimeRep, Period> operator /(QuantityType const& quantity, Rate<QuantityType, TimeRep, Period> const& rate)
	{
		return std::chrono::duration<QuantityType, TimeRep, Period>{quantity / rate._quantity};
	}

	/// @todo Why doesn't this seem to work in constexpr's?
	// quantity / duration -> rate
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr Rate<QuantityType, TimeRep, Period> operator /(QuantityType const& quantity, std::chrono::duration<TimeRep, Period> const& duration)
	{
		return Rate<QuantityType, TimeRep, Period>{quantity / duration.count()};
	}

	template<typename ToRate, typename QuantityType, typename TimeRep, typename Period>
	constexpr ToRate rate_cast(Rate<QuantityType, TimeRep, Period> const& rate)
	{
		using ratio = std::ratio_divide<ToRate::Period, Period>;
		return ToRate{static_cast<ToRate::quantity_t>(rate.quantity() * r::den / r::num)};
	}
}

// Specialize common_type to prevent errors during overload resolution. (These type values don't actually make much sense.)
namespace std
{
	template <typename QuantityType, typename TimeRep, typename Period>
	struct common_type<units::Rate<QuantityType, TimeRep, Period>, std::chrono::duration<TimeRep, Period>>
	{
		using type = TimeRep;
	};

	template <typename QuantityType, typename TimeRep, typename Period>
	struct common_type<std::chrono::duration<TimeRep, Period>, units::Rate<QuantityType, TimeRep, Period>>
	{
		using type = TimeRep;
	};

	template <typename QuantityType, typename TimeRep, typename Period>
	struct common_type<units::Rate<QuantityType, TimeRep, Period>, TimeRep>
	{
		using type = TimeRep;
	};

	template <typename QuantityType, typename TimeRep, typename Period>
	struct common_type<TimeRep, units::Rate<QuantityType, TimeRep, Period>>
	{
		using type = TimeRep;
	};
}
