//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "frequency.hpp"

namespace units
{
	//! A rate data type designed to complement std::chrono::duration.
	template <typename QuantityType, typename TimeRep, typename Period = std::ratio<1>>
	class rate
	{
	public:
		using quantity_type = QuantityType;
		using time_rep = TimeRep;
		using period = Period;

		constexpr rate() : _step{quantity_type{}} {}

		//! @param step The amount of the quantity type per unit period.
		constexpr explicit rate(quantity_type step) : _step{std::move(step)} {}

		//! The zero rate for the specified representation and period, constructed using the default constructor of the quantity type.
		static constexpr rate<quantity_type, time_rep, period> zero() { return rate<quantity_type, time_rep, period>{quantity_type{}}; }

		//! The amount of the quantity type per unit period.
		constexpr quantity_type const& step() const& { return _step; }
		//! The amount of the quantity type per unit period.
		quantity_type&& step() && { return std::move(_step); }
		//! The amount of the quantity type per unit period.
		quantity_type& step() & { return _step; }

		constexpr bool operator <(rate<quantity_type, time_rep, period> const& that) const { return _step < that._step; }
		constexpr bool operator <=(rate<quantity_type, time_rep, period> const& that) const { return _step <= that._step; }
		constexpr bool operator ==(rate<quantity_type, time_rep, period> const& that) const { return _step == that._step; }
		constexpr bool operator !=(rate<quantity_type, time_rep, period> const& that) const { return _step != that._step; }
		constexpr bool operator >=(rate<quantity_type, time_rep, period> const& that) const { return _step >= that._step; }
		constexpr bool operator >(rate<quantity_type, time_rep, period> const& that) const { return _step > that._step; }

		// Closed under addition.
		rate<quantity_type, time_rep, period>& operator +=(rate<quantity_type, time_rep, period> const& rate) &
		{
			_step += rate._step;
			return *this;
		}

		// Closed under subtraction.
		rate<quantity_type, time_rep, period>& operator -=(rate<quantity_type, time_rep, period> const& rate) &
		{
			_step -= rate._step;
			return *this;
		}

		// Closed under scalar multiplication.
		rate<quantity_type, time_rep, period>& operator *=(time_rep const& k) &
		{
			_step *= k;
			return *this;
		}

		// Closed under scalar division.
		rate<quantity_type, time_rep, period>& operator /=(time_rep const& k) &
		{
			_step /= k;
			return *this;
		}
	private:
		quantity_type _step; ///> The amount of the quantity type per unit period.
	};

	// rate + rate -> rate
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr rate<QuantityType, TimeRep, Period> operator +(rate<QuantityType, TimeRep, Period> const& rate1, rate<QuantityType, TimeRep, Period> const& rate2)
	{
		return rate<QuantityType, TimeRep, Period>{rate1.step() + rate2.step()};
	}

	// rate - rate -> rate
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr rate<QuantityType, TimeRep, Period> operator -(rate<QuantityType, TimeRep, Period> const& rate1, rate<QuantityType, TimeRep, Period> const& rate2)
	{
		return rate<QuantityType, TimeRep, Period>{rate1.step() - rate2.step()};
	}

	// -rate -> rate
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr rate<QuantityType, TimeRep, Period> operator -(rate<QuantityType, TimeRep, Period> const& rate)
	{
		return units::rate<QuantityType, TimeRep, Period>{-rate.step()};
	}

	// k * rate -> rate
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr rate<QuantityType, TimeRep, Period> operator *(TimeRep const& k, rate<QuantityType, TimeRep, Period> const& rate)
	{
		return units::rate<QuantityType, TimeRep, Period>{k * rate.step()};
	}
	// rate * k -> rate
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr rate<QuantityType, TimeRep, Period> operator *(rate<QuantityType, TimeRep, Period> const& rate, TimeRep const& k)
	{
		return units::rate<QuantityType, TimeRep, Period>{k * rate.step()};
	}

	// rate * duration -> quantity
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr QuantityType operator *(rate<QuantityType, TimeRep, Period> const& rate, std::chrono::duration<TimeRep, Period> const& duration)
	{
		return rate.step() * duration.count();
	}
	// duration * rate -> quantity
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr QuantityType operator *(std::chrono::duration<TimeRep, Period> const& duration, rate<QuantityType, TimeRep, Period> const& rate)
	{
		return rate.step() * duration.count();
	}

	// quantity * frequency -> rate
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr rate<QuantityType, TimeRep, Period> operator *(QuantityType const& quantity, frequency<TimeRep, Period> f)
	{
		return rate<QuantityType, TimeRep, Period>{quantity * f.count()};
	}
	// frequency * quantity -> rate
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr rate<QuantityType, TimeRep, Period> operator *(frequency<TimeRep, Period> f, QuantityType const& quantity)
	{
		return rate<QuantityType, TimeRep, Period>{quantity * f.count()};
	}

	// rate / k -> rate
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr rate<QuantityType, TimeRep, Period> operator /(rate<QuantityType, TimeRep, Period> const& rate, TimeRep const& k)
	{
		return units::rate<QuantityType, TimeRep, Period>{rate.step() / k};
	}

	// rate / quantity -> frequency
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr frequency<TimeRep, Period> operator /(rate<QuantityType, TimeRep, Period> const& rate, QuantityType const& quantity)
	{
		return frequency<TimeRep, Period>{rate.step() / quantity};
	}

	// rate / frequency -> quantity
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr QuantityType operator /(rate<QuantityType, TimeRep, Period> const& rate, frequency<TimeRep, Period> f)
	{
		return rate.step() / f.count();
	}

	// quantity / rate -> duration
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr std::chrono::duration<TimeRep, Period> operator /(QuantityType const& quantity, rate<QuantityType, TimeRep, Period> const& rate)
	{
		return std::chrono::duration<QuantityType, TimeRep, Period>{quantity / rate._quantity};
	}

	//! @todo Why doesn't this seem to work in constexpr's?
	// quantity / duration -> rate
	template <typename QuantityType, typename TimeRep, typename Period>
	constexpr rate<QuantityType, TimeRep, Period> operator /(QuantityType const& quantity, std::chrono::duration<TimeRep, Period> const& duration)
	{
		return rate<QuantityType, TimeRep, Period>{quantity / duration.count()};
	}

	template<typename ToRate, typename QuantityType, typename TimeRep, typename Period>
	constexpr ToRate rate_cast(rate<QuantityType, TimeRep, Period> const& rate)
	{
		using ratio = std::ratio_divide<ToRate::Period, Period>;
		return ToRate{static_cast<typename ToRate::quantity_type>(rate.quantity() * r::den / r::num)};
	}
}

// Specialize common_type to prevent errors during overload resolution. (These type values don't actually make much sense.)
namespace std
{
	template <typename QuantityType, typename TimeRep, typename Period>
	struct common_type<units::rate<QuantityType, TimeRep, Period>, std::chrono::duration<TimeRep, Period>>
	{
		using type = TimeRep;
	};

	template <typename QuantityType, typename TimeRep, typename Period>
	struct common_type<std::chrono::duration<TimeRep, Period>, units::rate<QuantityType, TimeRep, Period>>
	{
		using type = TimeRep;
	};

	template <typename QuantityType, typename TimeRep, typename Period>
	struct common_type<units::rate<QuantityType, TimeRep, Period>, TimeRep>
	{
		using type = TimeRep;
	};

	template <typename QuantityType, typename TimeRep, typename Period>
	struct common_type<TimeRep, units::rate<QuantityType, TimeRep, Period>>
	{
		using type = TimeRep;
	};
}
