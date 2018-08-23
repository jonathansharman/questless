//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "frequency.hpp"

namespace units {
	//! A rate data type designed to complement std::chrono::duration.
	//! @tparam Quantity The type used to represent the quantity, e.g. in "miles per hour" the quantity type is "miles".
	//! @tparam TimeRep The type to represent clock ticks.
	//! @tparam Period The number of seconds per clock tick, as a ratio.
	template <typename Quantity, typename TimeRep, typename Period = std::ratio<1>>
	class rate {
	public:
		using quantity_type = Quantity;
		using time_rep = TimeRep;
		using period = Period;

		constexpr rate() : _step{quantity_type{}} {}

		//! @param step The amount of the quantity type per unit period.
		constexpr explicit rate(quantity_type step) : _step{std::move(step)} {}

		//! The zero rate for the specified representation and period, constructed using the default constructor of the quantity type.
		static constexpr rate<quantity_type, time_rep, period> zero() { return rate<quantity_type, time_rep, period>{quantity_type{}}; }

		//! The amount of the quantity type per unit period.
		constexpr quantity_type& step() { return _step; }
		//! The amount of the quantity type per unit period.
		constexpr quantity_type step() const { return _step; }

		constexpr bool operator <(rate<quantity_type, time_rep, period> const& that) const { return _step < that._step; }
		constexpr bool operator <=(rate<quantity_type, time_rep, period> const& that) const { return _step <= that._step; }
		constexpr bool operator ==(rate<quantity_type, time_rep, period> const& that) const { return _step == that._step; }
		constexpr bool operator !=(rate<quantity_type, time_rep, period> const& that) const { return _step != that._step; }
		constexpr bool operator >=(rate<quantity_type, time_rep, period> const& that) const { return _step >= that._step; }
		constexpr bool operator >(rate<quantity_type, time_rep, period> const& that) const { return _step > that._step; }

		// Closed under addition.
		constexpr rate<quantity_type, time_rep, period>& operator +=(rate<quantity_type, time_rep, period> const& rate) & {
			_step += rate._step;
			return *this;
		}

		// Closed under subtraction.
		constexpr rate<quantity_type, time_rep, period>& operator -=(rate<quantity_type, time_rep, period> const& rate) & {
			_step -= rate._step;
			return *this;
		}

		// Closed under scalar multiplication.
		constexpr rate<quantity_type, time_rep, period>& operator *=(time_rep const& k) & {
			_step *= k;
			return *this;
		}

		// Closed under scalar division.
		constexpr rate<quantity_type, time_rep, period>& operator /=(time_rep const& k) & {
			_step /= k;
			return *this;
		}
	private:
		quantity_type _step; ///> The amount of the quantity type per unit period.
	};

	// rate + rate -> rate
	template <typename Quantity, typename TimeRep, typename Period>
	constexpr rate<Quantity, TimeRep, Period> operator +(rate<Quantity, TimeRep, Period> const& rate1, rate<Quantity, TimeRep, Period> const& rate2) {
		return rate<Quantity, TimeRep, Period>{rate1.step() + rate2.step()};
	}

	// rate - rate -> rate
	template <typename Quantity, typename TimeRep, typename Period>
	constexpr rate<Quantity, TimeRep, Period> operator -(rate<Quantity, TimeRep, Period> const& rate1, rate<Quantity, TimeRep, Period> const& rate2) {
		return rate<Quantity, TimeRep, Period>{rate1.step() - rate2.step()};
	}

	// -rate -> rate
	template <typename Quantity, typename TimeRep, typename Period>
	constexpr rate<Quantity, TimeRep, Period> operator -(rate<Quantity, TimeRep, Period> const& rate) {
		return units::rate<Quantity, TimeRep, Period>{-rate.step()};
	}

	// k * rate -> rate
	template <typename Quantity, typename TimeRep, typename Period>
	constexpr rate<Quantity, TimeRep, Period> operator *(TimeRep const& k, rate<Quantity, TimeRep, Period> const& rate) {
		return units::rate<Quantity, TimeRep, Period>{k * rate.step()};
	}
	// rate * k -> rate
	template <typename Quantity, typename TimeRep, typename Period>
	constexpr rate<Quantity, TimeRep, Period> operator *(rate<Quantity, TimeRep, Period> const& rate, TimeRep const& k) {
		return units::rate<Quantity, TimeRep, Period>{k * rate.step()};
	}

	// rate * duration -> quantity
	template <typename Quantity, typename TimeRep, typename Period>
	constexpr Quantity operator *(rate<Quantity, TimeRep, Period> const& rate, std::chrono::duration<TimeRep, Period> const& duration) {
		return rate.step() * duration.count();
	}
	// duration * rate -> quantity
	template <typename Quantity, typename TimeRep, typename Period>
	constexpr Quantity operator *(std::chrono::duration<TimeRep, Period> const& duration, rate<Quantity, TimeRep, Period> const& rate) {
		return rate.step() * duration.count();
	}

	// quantity * frequency -> rate
	template <typename Quantity, typename TimeRep, typename Period>
	constexpr rate<Quantity, TimeRep, Period> operator *(Quantity const& quantity, frequency<TimeRep, Period> f) {
		return rate<Quantity, TimeRep, Period>{quantity * f.count()};
	}
	// frequency * quantity -> rate
	template <typename Quantity, typename TimeRep, typename Period>
	constexpr rate<Quantity, TimeRep, Period> operator *(frequency<TimeRep, Period> f, Quantity const& quantity) {
		return rate<Quantity, TimeRep, Period>{quantity * f.count()};
	}

	// rate / k -> rate
	template <typename Quantity, typename TimeRep, typename Period>
	constexpr rate<Quantity, TimeRep, Period> operator /(rate<Quantity, TimeRep, Period> const& rate, TimeRep const& k) {
		return units::rate<Quantity, TimeRep, Period>{rate.step() / k};
	}

	// rate / quantity -> frequency
	template <typename Quantity, typename TimeRep, typename Period>
	constexpr frequency<TimeRep, Period> operator /(rate<Quantity, TimeRep, Period> const& rate, Quantity const& quantity) {
		return frequency<TimeRep, Period>{rate.step() / quantity};
	}

	// rate / frequency -> quantity
	template <typename Quantity, typename TimeRep, typename Period>
	constexpr Quantity operator /(rate<Quantity, TimeRep, Period> const& rate, frequency<TimeRep, Period> f) {
		return rate.step() / f.count();
	}

	// quantity / rate -> duration
	template <typename Quantity, typename TimeRep, typename Period>
	constexpr std::chrono::duration<TimeRep, Period> operator /(Quantity const& quantity, rate<Quantity, TimeRep, Period> const& rate) {
		return std::chrono::duration<TimeRep, Period>{quantity / rate._step};
	}

	//! @todo Why doesn't this seem to work in constexpr's?
	// quantity / duration -> rate
	template <typename Quantity, typename TimeRep, typename Period>
	constexpr rate<Quantity, TimeRep, Period> operator /(Quantity const& quantity, std::chrono::duration<TimeRep, Period> const& duration) {
		return rate<Quantity, TimeRep, Period>{quantity / duration.count()};
	}

	template<typename ToRate, typename Quantity, typename TimeRep, typename Period>
	constexpr ToRate rate_cast(rate<Quantity, TimeRep, Period> const& rate) {
		using ratio = std::ratio_divide<ToRate::Period, Period>;
		return ToRate{static_cast<typename ToRate::quantity_type>(rate.quantity() * r::den / r::num)};
	}
}

// Specialize common_type to prevent errors during overload resolution. (These type values don't actually make much sense.)
namespace std {
	template <typename Quantity, typename TimeRep, typename Period>
	struct common_type<units::rate<Quantity, TimeRep, Period>, std::chrono::duration<TimeRep, Period>> {
		using type = TimeRep;
	};

	template <typename Quantity, typename TimeRep, typename Period>
	struct common_type<std::chrono::duration<TimeRep, Period>, units::rate<Quantity, TimeRep, Period>> {
		using type = TimeRep;
	};

	template <typename Quantity, typename TimeRep, typename Period>
	struct common_type<units::rate<Quantity, TimeRep, Period>, TimeRep> {
		using type = TimeRep;
	};

	template <typename Quantity, typename TimeRep, typename Period>
	struct common_type<TimeRep, units::rate<Quantity, TimeRep, Period>> {
		using type = TimeRep;
	};
}
