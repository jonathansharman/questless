/**
* @file    Damage.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The Damage type, which encapsulates a set of damage components inflicted together.
*/

#ifndef DAMAGE_H
#define DAMAGE_H

#include <vector>

namespace questless
{
	struct Damage
	{
		double slash;
		double pierce;
		double bludgeon;
		double burn;
		double freeze;
		double blight;

		Damage() : slash{0.0}, pierce{0.0}, bludgeon{0.0}, burn{0.0}, freeze{0.0}, blight{0.0} {}

		Damage(double slash, double pierce, double bludgeon, double burn, double freeze, double blight)
			: slash{slash}, pierce{pierce}, bludgeon{bludgeon}, burn{burn}, freeze{freeze}, blight{blight}
		{}

		static Damage from_slash(double slash) { return Damage{slash, 0.0, 0.0, 0.0, 0.0, 0.0}; }
		static Damage from_pierce(double pierce) { return Damage{0.0, pierce, 0.0, 0.0, 0.0, 0.0}; }
		static Damage from_bludgeon(double bludgeon) { return Damage{0.0, 0.0, bludgeon, 0.0, 0.0, 0.0}; }
		static Damage from_burn(double burn) { return Damage{0.0, 0.0, 0.0, burn, 0.0, 0.0}; }
		static Damage from_freeze(double freeze) { return Damage{0.0, 0.0, 0.0, 0.0, freeze, 0.0}; }
		static Damage from_blight(double blight) { return Damage{0.0, 0.0, 0.0, 0.0, 0.0, blight}; }

		friend Damage operator +(const Damage& d1, const Damage& addend2)
		{
			return Damage
				{ d1.slash + addend2.slash
				, d1.pierce + addend2.pierce
				, d1.bludgeon + addend2.bludgeon
				, d1.burn + addend2.burn
				, d1.freeze + addend2.freeze
				, d1.blight + addend2.blight
				};
		}
		friend Damage operator -(const Damage& minuend, const Damage& subtrahend)
		{
			Damage difference;
			difference.slash = max(0.0, minuend.slash - subtrahend.slash);
			difference.pierce = max(0.0, minuend.pierce - subtrahend.pierce);
			difference.bludgeon = max(0.0, minuend.bludgeon - subtrahend.bludgeon);
			difference.burn = max(0.0, minuend.burn - subtrahend.burn);
			difference.freeze = max(0.0, minuend.freeze - subtrahend.freeze);
			difference.blight = max(0.0, minuend.blight - subtrahend.blight);
			return difference;
		}
		friend Damage operator *(const Damage& d, double k)
		{
			return Damage{k * d.slash, k * d.pierce, k * d.bludgeon, k * d.burn, k * d.freeze, k * d.blight};
		}
		friend Damage operator *(double k, const Damage& d)
		{
			return Damage{k * d.slash, k * d.pierce, k * d.bludgeon, k * d.burn, k * d.freeze, k * d.blight};
		}
		friend Damage operator /(const Damage& d, double k)
		{
			return Damage{d.slash / k, d.pierce / k, d.bludgeon / k, d.burn / k, d.freeze / k, d.blight / k};
		}

		Damage& operator +=(const Damage& addend)
		{
			slash += addend.slash;
			pierce += addend.pierce;
			bludgeon += addend.bludgeon;
			burn += addend.burn;
			freeze += addend.freeze;
			blight += addend.blight;
			return *this;
		}
		Damage& operator -=(const Damage& subtrahend)
		{
			slash -= subtrahend.slash;
			pierce -= subtrahend.pierce;
			bludgeon -= subtrahend.bludgeon;
			burn -= subtrahend.burn;
			freeze -= subtrahend.freeze;
			blight -= subtrahend.blight;
			return *this;
		}
		Damage& operator *=(double k)
		{
			slash *= k;
			pierce *= k;
			bludgeon *= k;
			burn *= k;
			freeze *= k;
			blight *= k;
			return *this;
		}

		/// @return The sum of the damage components.
		double total() const { return slash + pierce + bludgeon + burn + freeze + blight; }
	};
}

#endif