/**
* @file    Frequency.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION A frequency data type designed to complement the STL duration type.
*/

#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <ratio>
#include <chrono>

namespace questless
{
	template <typename Rep, typename Period = std::ratio<1>>
	class Frequency
	{
	public:
		using rep = Rep;
		using period = Period;

		constexpr Frequency() : _count{rep{}} {}

		/// @param cycles The number of cycles per unit period.
		constexpr explicit Frequency(rep cycles) : _count{std::move(cycles)} {}

		/// @return The zero f for the specified representation and period.
		constexpr static Frequency<rep, period> zero() { return Frequency<rep, period>{std::chrono::duration_values<rep>::zero()}; }

		/// @return The number of cycles per unit period.
		constexpr rep count() const { return _count; }

		constexpr bool operator <(Frequency<rep, period> right) const { return _count < right._count; }
		constexpr bool operator <=(Frequency<rep, period> right) const { return _count <= right._count; }
		constexpr bool operator ==(Frequency<rep, period> right) const { return _count == right._count; }
		constexpr bool operator !=(Frequency<rep, period> right) const { return _count != right._count; }
		constexpr bool operator >=(Frequency<rep, period> right) const { return _count >= right._count; }
		constexpr bool operator >(Frequency<rep, period> right) const { return _count > right._count; }

		// Closed under addition.
		Frequency<rep, period>& operator +=(Frequency<rep, period> f)
		{
			_count += f._count;
			return *this;
		}

		// Closed under subtraction.
		Frequency<rep, period>& operator -=(Frequency<rep, period> f)
		{
			_count -= f._count;
			return *this;
		}

		// Closed under scalar multiplication.
		Frequency<rep, period>& operator *=(rep factor)
		{
			_count *= factor;
			return *this;
		}

		// Closed under scalar division.
		Frequency<rep, period>& operator /=(rep factor)
		{
			_count /= factor;
			return *this;
		}
	private:
		rep _count; ///> The number of cycles per unit period.
	};

	// frequency + frequency -> frequency
	template <typename Rep, typename Period>
	constexpr Frequency<Rep, Period> operator +(Frequency<Rep, Period> f1, Frequency<Rep, Period> f2)
	{
		return Frequency<Rep, Period>{f1.count() + f2.count()};
	}

	// frequency - frequency -> frequency
	template <typename Rep, typename Period>
	constexpr Frequency<Rep, Period> operator -(Frequency<Rep, Period> f1, Frequency<Rep, Period> f2)
	{
		return Frequency<Rep, Period>{f1.count() - f2.count()};
	}

	// -frequency -> frequency
	template <typename Rep, typename Period>
	constexpr Frequency<Rep, Period> operator -(Frequency<Rep, Period> f)
	{
		return Frequency<Rep, Period>{-f.count()};
	}

	// k * frequency -> frequency
	template <typename Rep, typename Period>
	constexpr Frequency<Rep, Period> operator *(Rep factor, Frequency<Rep, Period> f)
	{
		return Frequency<Rep, Period>{factor * f.count()};
	}
	// frequency * k -> frequency
	template <typename Rep, typename Period>
	constexpr Frequency<Rep, Period> operator *(Frequency<Rep, Period> f, Rep factor)
	{
		return Frequency<Rep, Period>{factor * f.count()};
	}

	// frequency * duration -> k
	template <typename Rep, typename Period>
	constexpr Rep operator *(Frequency<Rep, Period> f, std::chrono::duration<Rep, Period> duration)
	{
		return f.count() * duration.count();
	}
	// duration * frequency -> k
	template <typename Rep, typename Period>
	constexpr Rep operator *(std::chrono::duration<Rep, Period> duration, Frequency<Rep, Period> f)
	{
		return f.count() * duration.count();
	}

	// k / duration -> frequency
	template <typename Rep, typename Period>
	constexpr Frequency<Rep, Period> operator /(Rep dividend, const std::chrono::duration<Rep, Period> duration)
	{
		return Frequency<Rep, Period>{dividend / duration.count()};
	}

	// k / frequency -> duration
	template <typename Rep, typename Period>
	constexpr std::chrono::duration<Rep, Period> operator /(Rep dividend, Frequency<Rep, Period> f)
	{
		return std::chrono::duration<Rep, Period>{dividend / f.count()};
	}

	template<typename ToFrequency, typename Rep, typename Period>
	constexpr ToFrequency frequency_cast(const Frequency<Rep, Period>& f)
	{
		using ratio = std::ratio_divide<ToFrequency::period, Period>;
		return ToFrequency{static_cast<ToFrequency::rep>(f.count() * ratio::den / ratio::num)};
	}

	/// Frequency type representing cycles per second.
	using Hertz = Frequency<double>;
	/// Frequency type representing thousounds of cycles per second or cycles per millisecond.
	using kiloHertz = Frequency<double, std::milli>;

	constexpr Hertz operator ""_Hz(long double cycles) { return Hertz{static_cast<double>(cycles)}; }
	constexpr kiloHertz operator ""_kHz(long double cycles) { return kiloHertz{static_cast<double>(cycles)}; }
}

// Specialize common_type to prevent errors during overload resolution. (These type values don't actually make much sense.)
namespace std
{
	template <typename Rep, typename Period>
	struct common_type<questless::Frequency<Rep, Period>, std::chrono::duration<Rep, Period>>
	{
		using type = Rep;
	};

	template <typename Rep, typename Period>
	struct common_type<std::chrono::duration<Rep, Period>, questless::Frequency<Rep, Period>>
	{
		using type = Rep;
	};

	template <typename Rep, typename Period>
	struct common_type<questless::Frequency<Rep, Period>, Rep>
	{
		using type = Rep;
	};

	template <typename Rep, typename Period>
	struct common_type<Rep, questless::Frequency<Rep, Period>>
	{
		using type = Rep;
	};
}

#endif
