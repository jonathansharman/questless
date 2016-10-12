/**
* @file    DamageMultiplier.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Resistance reduces damage by some multiple. Vulnerability increases damage by some multiple.
*/

#ifndef DAMAGE_MULTIPLIER_H
#define DAMAGE_MULTIPLIER_H

namespace questless
{
	template <typename Derived>
	struct DamageMultiplier
	{
		double slash;
		double pierce;
		double bludgeon;
		double burn;
		double freeze;
		double blight;

		constexpr DamageMultiplier() : slash{0.0}, pierce{0.0}, bludgeon{0.0}, burn{0.0}, freeze{0.0}, blight{0.0} {}

		constexpr DamageMultiplier(double slash, double pierce, double bludgeon, double burn, double freeze, double blight)
			: slash{slash}, pierce{pierce}, bludgeon{bludgeon}, burn{burn}, freeze{freeze}, blight{blight}
		{}

		static constexpr Derived zero() { return Derived{}; }
		static constexpr Derived from_slash(double slash) { return Derived{slash, 0.0, 0.0, 0.0, 0.0, 0.0}; }
		static constexpr Derived from_pierce(double pierce) { return Derived{0.0, pierce, 0.0, 0.0, 0.0, 0.0}; }
		static constexpr Derived from_bludgeon(double bludgeon) { return Derived{0.0, 0.0, bludgeon, 0.0, 0.0, 0.0}; }
		static constexpr Derived from_burn(double burn) { return Derived{0.0, 0.0, 0.0, burn, 0.0, 0.0}; }
		static constexpr Derived from_freeze(double freeze) { return Derived{0.0, 0.0, 0.0, 0.0, freeze, 0.0}; }
		static constexpr Derived from_blight(double blight) { return Derived{0.0, 0.0, 0.0, 0.0, 0.0, blight}; }

		constexpr friend Derived operator +(const Derived& d1, const Derived& addend2)
		{
			return Derived
				{ d1.slash + addend2.slash
				, d1.pierce + addend2.pierce
				, d1.bludgeon + addend2.bludgeon
				, d1.burn + addend2.burn
				, d1.freeze + addend2.freeze
				, d1.blight + addend2.blight
				};
		}
		friend constexpr Derived operator -(const Derived& minuend, const Derived& subtrahend)
		{
			Derived difference;
			difference.slash = max(0.0, minuend.slash - subtrahend.slash);
			difference.pierce = max(0.0, minuend.pierce - subtrahend.pierce);
			difference.bludgeon = max(0.0, minuend.bludgeon - subtrahend.bludgeon);
			difference.burn = max(0.0, minuend.burn - subtrahend.burn);
			difference.freeze = max(0.0, minuend.freeze - subtrahend.freeze);
			difference.blight = max(0.0, minuend.blight - subtrahend.blight);
			return difference;
		}
		friend constexpr Derived operator *(const Derived& d, double k)
		{
			return Derived{k * d.slash, k * d.pierce, k * d.bludgeon, k * d.burn, k * d.freeze, k * d.blight};
		}
		friend constexpr Derived operator *(double k, const Derived& d)
		{
			return Derived{k * d.slash, k * d.pierce, k * d.bludgeon, k * d.burn, k * d.freeze, k * d.blight};
		}
		friend constexpr Derived operator /(const Derived& d, double k)
		{
			return Derived{d.slash / k, d.pierce / k, d.bludgeon / k, d.burn / k, d.freeze / k, d.blight / k};
		}

		Derived& operator +=(const Derived& addend)
		{
			slash += addend.slash;
			pierce += addend.pierce;
			bludgeon += addend.bludgeon;
			burn += addend.burn;
			freeze += addend.freeze;
			blight += addend.blight;
			return static_cast<Derived&>(*this);
		}
		Derived& operator -=(const Derived& subtrahend)
		{
			slash -= subtrahend.slash;
			pierce -= subtrahend.pierce;
			bludgeon -= subtrahend.bludgeon;
			burn -= subtrahend.burn;
			freeze -= subtrahend.freeze;
			blight -= subtrahend.blight;
			return static_cast<Derived&>(*this);
		}
		Derived& operator *=(double k)
		{
			slash *= k;
			pierce *= k;
			bludgeon *= k;
			burn *= k;
			freeze *= k;
			blight *= k;
			return static_cast<Derived&>(*this);
		}
	};

	struct Resistance : public DamageMultiplier<Resistance>
	{
		using DamageMultiplier<Resistance>::DamageMultiplier;

		/// @return The base resistance of the given Being subclass.
		template <typename BeingType>
		static constexpr Resistance of()
		{
			using T = BeingType;
			return Resistance
				{ T::slash_resistance
				, T::pierce_resistance
				, T::bludgeon_resistance
				, T::burn_resistance
				, T::freeze_resistance
				, T::blight_resistance
				};
		}
	};

	struct Vulnerability : public DamageMultiplier<Vulnerability>
	{
		using DamageMultiplier<Vulnerability>::DamageMultiplier;

		/// @return The base vulnerability of the given Being subclass.
		template <typename BeingType>
		static constexpr Vulnerability of()
		{
			using T = BeingType;
			return Vulnerability
				{ T::slash_vulnerability
				, T::pierce_vulnerability
				, T::bludgeon_vulnerability
				, T::burn_vulnerability
				, T::freeze_vulnerability
				, T::blight_vulnerability
				};
		}
	};
}

#endif
