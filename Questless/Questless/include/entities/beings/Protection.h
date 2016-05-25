/**
* @file    Protection.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The Protection type, which encapsulates damage mitigation.
*/

#ifndef PROTECTION_H
#define PROTECTION_H

#include "Damage.h"

namespace questless
{
	namespace ProtectionK
	{
		const double percent_pad_to_slash = 0.50;
		const double percent_pad_to_pierce = 0.25;
		const double percent_pad_to_bludgeon = 1.00;

		const double percent_deflect_to_slash = 1.00;
		const double percent_deflect_to_pierce = 0.50;
		const double percent_deflect_to_bludgeon = 0.25;

		const double percent_insulate_to_burn = 1.00;
		const double percent_insulate_to_freeze = 1.00;

		const double percent_cleanse_to_blight = 1.00;
	}

	struct Protection
	{
		double pad;
		double deflect;
		double insulate;
		double cleanse;

		Protection(double pad = 0, double deflect = 0, double insulate = 0, double cleanse = 0) : pad{pad}, deflect{deflect}, insulate{insulate}, cleanse{cleanse} {}

		friend Protection operator +(const Protection& p1, const Protection& p2)
		{
			return Protection{p1.pad + p2.pad, p1.deflect + p2.deflect, p1.insulate + p2.insulate, p1.cleanse + p2.cleanse};
		}
		friend Protection operator *(const Protection& p, double k)
		{
			return Protection{k * p.pad, k * p.deflect, k * p.insulate, k * p.cleanse};
		}
		friend Protection operator *(double k, const Protection& p)
		{
			return Protection{k * p.pad, k * p.deflect, k * p.insulate, k * p.cleanse};
		}
		friend Protection operator /(const Protection& p, double k)
		{
			return Protection{p.pad / k, p.deflect / k, p.insulate / k, p.cleanse / k};
		}

		Protection& operator +=(const Protection& p)
		{
			pad += p.pad;
			deflect += p.deflect;
			insulate += p.insulate;
			cleanse += p.cleanse;
			return *this;
		}
		Protection& operator *=(double k)
		{
			pad *= k;
			deflect *= k;
			insulate *= k;
			cleanse *= k;
			return *this;
		}

		/// @return The amount of damage by which this protection reduces incoming damage.
		Damage reduction() const
		{
			return Damage
				{ pad * ProtectionK::percent_pad_to_slash + deflect * ProtectionK::percent_deflect_to_slash // slash
				, pad * ProtectionK::percent_pad_to_pierce + deflect * ProtectionK::percent_deflect_to_pierce // pierce
				, pad * ProtectionK::percent_pad_to_bludgeon + deflect * ProtectionK::percent_deflect_to_bludgeon // bludgeon
				, insulate * ProtectionK::percent_insulate_to_burn // burn
				, insulate * ProtectionK::percent_insulate_to_freeze // freeze
				, cleanse * ProtectionK::percent_cleanse_to_blight // blight
				};
		}
	};
}

#endif