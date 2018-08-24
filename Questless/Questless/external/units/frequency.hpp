//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <ratio>
#include <chrono>

namespace units {
	//! A frequency data type designed to complement std::chrono::duration.
	template <typename Rep, typename Period = std::ratio<1>>
	class frequency {
	public:
		using rep = Rep;
		using period = Period;

		constexpr frequency() : _count{rep{}} {}

		//! @param count The number of cycles per unit period.
		constexpr explicit frequency(rep count) : _count{std::move(count)} {}

		//! The zero f for the specified representation and period.
		static constexpr frequency<rep, period> zero() { return frequency<rep, period>{std::chrono::duration_values<rep>::zero()}; }

		//! The number of cycles per unit period.
		constexpr rep const& count() const& { return _count; }
		//! The number of cycles per unit period.
		rep& count() & { return _count; }

		constexpr bool operator <(frequency<rep, period> that) const { return _count < that._count; }
		constexpr bool operator <=(frequency<rep, period> that) const { return _count <= that._count; }
		constexpr bool operator ==(frequency<rep, period> that) const { return _count == that._count; }
		constexpr bool operator !=(frequency<rep, period> that) const { return _count != that._count; }
		constexpr bool operator >=(frequency<rep, period> that) const { return _count >= that._count; }
		constexpr bool operator >(frequency<rep, period> that) const { return _count > that._count; }

		// Closed under addition.
		frequency<rep, period>& operator +=(frequency<rep, period> f) & {
			_count += f._count;
			return *this;
		}

		// Closed under subtraction.
		frequency<rep, period>& operator -=(frequency<rep, period> f) & {
			_count -= f._count;
			return *this;
		}

		// Closed under scalar multiplication.
		frequency<rep, period>& operator *=(rep k) & {
			_count *= k;
			return *this;
		}

		// Closed under scalar division.
		frequency<rep, period>& operator /=(rep k) & {
			_count /= k;
			return *this;
		}
	private:
		rep _count; ///> The number of cycles per unit period.
	};

	// frequency + frequency -> frequency
	template <typename Rep, typename Period>
	constexpr frequency<Rep, Period> operator +(frequency<Rep, Period> const& f1, frequency<Rep, Period> const& f2) {
		return frequency<Rep, Period>{f1.count() + f2.count()};
	}

	// frequency - frequency -> frequency
	template <typename Rep, typename Period>
	constexpr frequency<Rep, Period> operator -(frequency<Rep, Period> const& f1, frequency<Rep, Period> const& f2) {
		return frequency<Rep, Period>{f1.count() - f2.count()};
	}

	// -frequency -> frequency
	template <typename Rep, typename Period>
	constexpr frequency<Rep, Period> operator -(frequency<Rep, Period> const& f) {
		return frequency<Rep, Period>{-f.count()};
	}

	// k * frequency -> frequency
	template <typename Rep, typename Period>
	constexpr frequency<Rep, Period> operator *(Rep k, frequency<Rep, Period> const& f) {
		return frequency<Rep, Period>{k * f.count()};
	}
	// frequency * k -> frequency
	template <typename Rep, typename Period>
	constexpr frequency<Rep, Period> operator *(frequency<Rep, Period> const& f, Rep k) {
		return frequency<Rep, Period>{k * f.count()};
	}

	// frequency * duration -> k
	template <typename Rep, typename Period>
	constexpr Rep operator *(frequency<Rep, Period> const& f, std::chrono::duration<Rep, Period> duration) {
		return f.count() * duration.count();
	}
	// duration * frequency -> k
	template <typename Rep, typename Period>
	constexpr Rep operator *(std::chrono::duration<Rep, Period> duration, frequency<Rep, Period> const& f) {
		return f.count() * duration.count();
	}

	// frequency / k -> frequency
	template <typename Rep, typename Period>
	constexpr Rep operator /(frequency<Rep, Period> const& f, Rep k) {
		return frequency<Rep, Period>{f.count() / k};
	}

	// k / duration -> frequency
	template <typename Rep, typename Period>
	constexpr frequency<Rep, Period> operator /(Rep dividend, std::chrono::duration<Rep, Period> const& duration) {
		return frequency<Rep, Period>{dividend / duration.count()};
	}

	// k / frequency -> duration
	template <typename Rep, typename Period>
	constexpr std::chrono::duration<Rep, Period> operator /(Rep dividend, frequency<Rep, Period> const& f) {
		return std::chrono::duration<Rep, Period>{dividend / f.count()};
	}

	template<typename ToFrequency, typename Rep, typename Period>
	constexpr ToFrequency frequency_cast(frequency<Rep, Period> const& f) {
		using ratio = std::ratio_divide<ToFrequency::period, Period>;
		return ToFrequency{static_cast<typename ToFrequency::rep>(f.count() * ratio::den / ratio::num)};
	}

	//! Frequency type representing cycles per second.
	using hertz = frequency<double>;
	//! Frequency type representing thousounds of cycles per second or cycles per millisecond.
	using kilohertz = frequency<double, std::milli>;
}

//! @todo Where's the best place to put user-defined literals?

constexpr units::hertz operator ""_Hz(long double cycles) { return units::hertz{static_cast<double>(cycles)}; }
constexpr units::kilohertz operator ""_kHz(long double cycles) { return units::kilohertz{static_cast<double>(cycles)}; }

// Specialize common_type to prevent errors during overload resolution. (These type values don't actually make much sense.)
namespace std {
	template <typename Rep, typename Period>
	struct common_type<units::frequency<Rep, Period>, std::chrono::duration<Rep, Period>> {
		using type = Rep;
	};

	template <typename Rep, typename Period>
	struct common_type<std::chrono::duration<Rep, Period>, units::frequency<Rep, Period>> {
		using type = Rep;
	};

	template <typename Rep, typename Period>
	struct common_type<units::frequency<Rep, Period>, Rep> {
		using type = Rep;
	};

	template <typename Rep, typename Period>
	struct common_type<Rep, units::frequency<Rep, Period>> {
		using type = Rep;
	};
}
