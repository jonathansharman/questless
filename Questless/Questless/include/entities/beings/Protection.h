/**
* @file    Protection.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Protection reduces damage by a fixed amount.
*/

#ifndef PROTECTION_H
#define PROTECTION_H

#include "Damage.h"

namespace questless
{
	struct Protection
	{
		static constexpr double percent_pad_to_slash = 0.50;
		static constexpr double percent_pad_to_pierce = 0.25;
		static constexpr double percent_pad_to_bludgeon = 1.00;

		static constexpr double percent_deflect_to_slash = 1.00;
		static constexpr double percent_deflect_to_pierce = 0.50;
		static constexpr double percent_deflect_to_bludgeon = 0.25;

		static constexpr double percent_fireproof_to_burn = 1.00;
		static constexpr double percent_frostproof_to_freeze = 1.00;

		static constexpr double percent_cleanse_to_blight = 1.00;

		double pad;
		double deflect;
		double fireproof;
		double frostproof;
		double cleanse;

		constexpr Protection() : pad{0.0}, deflect{0.0}, fireproof{0.0}, frostproof{0.0}, cleanse{0.0} {}

		constexpr Protection(double pad, double deflect, double fireproof, double frostproof, double cleanse)
			: pad{pad}, deflect{deflect}, fireproof{fireproof}, frostproof{frostproof}, cleanse{cleanse}
		{}

		static constexpr Protection zero() { return Protection{}; }
		static constexpr Protection from_pad(double pad) { return Protection{pad, 0.0, 0.0, 0.0, 0.0}; }
		static constexpr Protection from_deflect(double deflect) { return Protection{0.0, deflect, 0.0, 0.0, 0.0}; }
		static constexpr Protection from_fireproof(double fireproof) { return Protection{0.0, 0.0, fireproof, 0.0, 0.0}; }
		static constexpr Protection from_frostproof(double frostproof) { return Protection{0.0, 0.0, 0.0, frostproof, 0.0}; }
		static constexpr Protection from_cleanse(double cleanse) { return Protection{0.0, 0.0, 0.0, 0.0, cleanse}; }

		/// @return The base protection of the given Being subclass.
		template <typename BeingType>
		static Protection of()
		{
			using T = BeingType;
			return Protection
				{ T::pad
				, T::deflect
				, T::fireproof
				, T::frostproof
				, T::cleanse
				};
		}

		friend std::ostream& operator <<(std::ostream& out, const Protection& p)
		{
			out << p.pad << ' ' << p.deflect << ' ' << p.fireproof << ' ' << p.frostproof << ' ' << p.cleanse;
			return out;
		}

		friend std::istream& operator >> (std::istream& in, Protection& p)
		{
			in >> p.pad >> p.deflect >> p.fireproof >> p.frostproof >> p.cleanse;
			return in;
		}

		friend Protection operator +(const Protection& p1, const Protection& p2)
		{
			return Protection
				{ p1.pad + p2.pad
				, p1.deflect + p2.deflect
				, p1.fireproof + p2.fireproof
				, p1.frostproof + p2.frostproof
				, p1.cleanse + p2.cleanse
				};
		}
		friend Protection operator *(const Protection& p, double k)
		{
			return Protection{k * p.pad, k * p.deflect, k * p.fireproof, k * p.frostproof, k * p.cleanse};
		}
		friend Protection operator *(double k, const Protection& p)
		{
			return Protection{k * p.pad, k * p.deflect, k * p.fireproof, k * p.frostproof, k * p.cleanse};
		}
		friend Protection operator /(const Protection& p, double k)
		{
			return Protection{p.pad / k, p.deflect / k, p.fireproof / k, p.frostproof / k, p.cleanse / k};
		}

		Protection& operator +=(const Protection& p)
		{
			pad += p.pad;
			deflect += p.deflect;
			fireproof += p.fireproof;
			frostproof += p.frostproof;
			cleanse += p.cleanse;
			return *this;
		}
		Protection& operator *=(double k)
		{
			pad *= k;
			deflect *= k;
			fireproof *= k;
			frostproof *= k;
			cleanse *= k;
			return *this;
		}

		/// @return The amount of damage by which this protection reduces incoming damage.
		Damage reduction() const
		{
			return Damage
				{ pad * percent_pad_to_slash + deflect * percent_deflect_to_slash // slash
				, pad * percent_pad_to_pierce + deflect * percent_deflect_to_pierce // pierce
				, pad * percent_pad_to_bludgeon + deflect * percent_deflect_to_bludgeon // bludgeon
				, fireproof * percent_fireproof_to_burn // burn
				, frostproof * percent_frostproof_to_freeze // freeze
				, cleanse * percent_cleanse_to_blight // blight
				};
		}
	};
}

#endif
