/**
* @file    Frequency.h
* @author  Jonathan Sharman
*
* @section LICENSE
*
* @todo This
*
* @section DESCRIPTION
*
* This file implements a frequency data type designed to complement the STL duration type.
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
		using period_t = Period;

		/// Default constructs a frequency object to 0 Hz.
		constexpr Frequency() : _cycles{0} {}

		/// @todo Mark the following constructor explicit if Microsoft ever fixes the bug in the duration * operator. (see <http://stackoverflow.com/questions/24022142/user-defined-overloaded-operator-with-stdchronoduration/24023059>)

		/// Constructs a frequency object, used to hold the value of a frequency and perform operations with std::duration objects.
		/// @param cycles The number of cycles per unit period.
		constexpr Frequency(Rep cycles) : _cycles{cycles} {}

		/// @return The zero frequency for the specified representation and period.
		constexpr static Frequency<Rep, Period> zero() { return Frequency<Rep, Period>{0}; }

		/// @return The number of cycles per period.
		Rep count() const { return _cycles; }

		/// Calculates the period corresponding to the frequency, i.e. its reciprocal.
		/// @note Beware of rounding errors when using an integral representation type.
		/// @return The period.
		std::chrono::duration<Rep, Period> period() { return std::chrono::duration<Rep, Period>{1 / _cycles}; }

		constexpr bool operator ==(const Frequency<Rep, Period>& right) const { return _cycles == right._cycles; }
		constexpr bool operator !=(const Frequency<Rep, Period>& right) const { return _cycles != right._cycles; }

		constexpr friend Frequency<Rep, Period> operator +(Frequency<Rep, Period> f1, Frequency<Rep, Period> f2)
		{
			return Frequency<Rep, Period>{f1._cycles + f2._cycles};
		}

		constexpr friend Frequency<Rep, Period> operator -(Frequency<Rep, Period> f1, Frequency<Rep, Period> f2)
		{
			return Frequency<Rep, Period>{f1._cycles - f2._cycles};
		}

		constexpr friend Frequency<Rep, Period> operator *(const Rep& factor, Frequency<Rep, Period> f)
		{
			return Frequency<Rep, Period>{factor * f._cycles};
		}
		constexpr friend Frequency<Rep, Period> operator *(Frequency<Rep, Period> f, const Rep& factor)
		{
			return Frequency<Rep, Period>{factor * f._cycles};
		}
		constexpr friend Rep operator *(Frequency<Rep, Period> f, const std::chrono::duration<Rep, Period> d)
		{
			return f._cycles * d.count();
		}
		constexpr friend Rep operator *(const std::chrono::duration<Rep, Period> d, Frequency<Rep, Period> f)
		{
			return f._cycles * d.count();
		}

		constexpr friend std::chrono::duration<Rep, Period> operator /(const Rep& dividend, Frequency<Rep, Period> f)
		{
			return std::chrono::duration<Rep, Period>{dividend / f._cycles};
		}
		constexpr friend Frequency<Rep, Period> operator /(Frequency<Rep, Period> f, const Rep& divisor)
		{
			return Frequency<Rep, Period>{f._cycles / divisor};
		}
		constexpr friend Frequency<Rep, Period> operator /(const Rep& dividend, const std::chrono::duration<Rep, Period> d)
		{
			return Frequency<Rep, Period>{dividend / d.count()};
		}

		Frequency<Rep, Period>& operator +=(Frequency<Rep, Period> f)
		{
			_cycles += f._cycles;
			return *this;
		}

		Frequency<Rep, Period>& operator -=(Frequency<Rep, Period> f)
		{
			_cycles -= f._cycles;
			return *this;
		}

		Frequency<Rep, Period>& operator *=(double factor)
		{
			_cycles *= factor;
			return *this;
		}

		Frequency<Rep, Period>& operator /=(double factor)
		{
			_cycles /= factor;
			return *this;
		}
	private:
		Rep _cycles; ///> The number of cycles per period.
	};

	template<typename ToFrequency, typename Rep, typename Period>
	ToFrequency frequency_cast(Frequency<Rep, Period> f)
	{
		ToFrequency ret;
		std::ratio r = std::ratio_divide < ToFrequency::period, Period > ;
		ret._cycles = static_cast<ToFrequency::rep>{f.cycles * r::num / r::den};
		return ret;
	}

	/// Frequency type representing cycles per second.
	using Hertz = Frequency<double>;
	/// Frequency type representing thousounds of cycles per second or cycles per millisecond.
	using kiloHertz = Frequency<double, std::milli>;

	constexpr Hertz operator ""_Hz(long double cycles) { return Hertz{static_cast<double>(cycles)}; }
	constexpr kiloHertz operator ""_kHz(long double cycles) { return kiloHertz{static_cast<double>(cycles)}; }
}

#endif
