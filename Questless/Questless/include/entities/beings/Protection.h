/**
* @file    Protection.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Protection reduces damage by a fixed amount.
*/

#pragma once

#include "Damage.h"

namespace questless
{
	struct Pad : TaggedType<double> { using TaggedType::TaggedType; };
	struct Deflect : TaggedType<double> { using TaggedType::TaggedType; };
	struct Fireproof : TaggedType<double> { using TaggedType::TaggedType; };
	struct Frostproof : TaggedType<double> { using TaggedType::TaggedType; };
	struct Cleanse : TaggedType<double> { using TaggedType::TaggedType; };

	class Protection
	{
	public:
		static constexpr double percent_pad_to_slash = 0.50;
		static constexpr double percent_pad_to_pierce = 0.25;
		static constexpr double percent_pad_to_bludgeon = 1.00;

		static constexpr double percent_deflect_to_slash = 1.00;
		static constexpr double percent_deflect_to_pierce = 0.50;
		static constexpr double percent_deflect_to_bludgeon = 0.25;

		static constexpr double percent_fireproof_to_burn = 1.00;
		static constexpr double percent_frostproof_to_freeze = 1.00;

		static constexpr double percent_cleanse_to_blight = 1.00;

		constexpr Protection() = default;

		constexpr Protection(Pad pad, Deflect deflect, Fireproof fireproof, Frostproof frostproof, Cleanse cleanse)
			: _pad{std::move(pad)}, _deflect{std::move(deflect)}, _fireproof{std::move(fireproof)}, _frostproof{std::move(frostproof)}, _cleanse{std::move(cleanse)}
		{}

		constexpr Protection(Pad pad) : _pad{std::move(pad)}, _deflect{0.0}, _fireproof{0.0}, _frostproof{0.0}, _cleanse{0.0} {}
		constexpr Protection(Deflect deflect) : _pad{0.0}, _deflect{std::move(deflect)}, _fireproof{0.0}, _frostproof{0.0}, _cleanse{0.0} {}
		constexpr Protection(Fireproof fireproof) : _pad{0.0}, _deflect{0.0}, _fireproof{std::move(fireproof)}, _frostproof{0.0}, _cleanse{0.0} {}
		constexpr Protection(Frostproof frostproof) : _pad{0.0}, _deflect{0.0}, _fireproof{0.0}, _frostproof{std::move(frostproof)}, _cleanse{0.0} {}
		constexpr Protection(Cleanse cleanse) : _pad{0.0}, _deflect{0.0}, _fireproof{0.0}, _frostproof{0.0}, _cleanse{std::move(cleanse)} {}

		static constexpr Protection zero() { return Protection{}; }

		friend std::ostream& operator <<(std::ostream& out, const Protection& p)
		{
			out << p._pad << ' ' << p._deflect << ' ' << p._fireproof << ' ' << p._frostproof << ' ' << p._cleanse;
			return out;
		}

		friend std::istream& operator >> (std::istream& in, Protection& p)
		{
			in >> p._pad >> p._deflect >> p._fireproof >> p._frostproof >> p._cleanse;
			return in;
		}

		friend Protection operator +(const Protection& p1, const Protection& p2)
		{
			return Protection
				{ Pad{p1._pad + p2._pad}
				, Deflect{p1._deflect + p2._deflect}
				, Fireproof{p1._fireproof + p2._fireproof}
				, Frostproof{p1._frostproof + p2._frostproof}
				, Cleanse{p1._cleanse + p2._cleanse}
				};
		}
		friend Protection operator *(const Protection& p, double k)
		{
			return Protection
				{ Pad{k * p._pad}
				, Deflect{k * p._deflect}
				, Fireproof{k * p._fireproof}
				, Frostproof{k * p._frostproof}
				, Cleanse{k * p._cleanse}
				};
		}
		friend Protection operator *(double k, const Protection& p)
		{
			return Protection
				{ Pad{k * p._pad}
				, Deflect{k * p._deflect}
				, Fireproof{k * p._fireproof}
				, Frostproof{k * p._frostproof}
				, Cleanse{k * p._cleanse}
				};
		}
		friend Protection operator /(const Protection& p, double k)
		{
			return Protection
				{ Pad{p._pad / k}
				, Deflect{p._deflect / k}
				, Fireproof{p._fireproof / k}
				, Frostproof{p._frostproof / k}
				, Cleanse{p._cleanse / k}
				};
		}

		Protection& operator +=(const Protection& p)
		{
			_pad += p._pad;
			_deflect += p._deflect;
			_fireproof += p._fireproof;
			_frostproof += p._frostproof;
			_cleanse += p._cleanse;
			return *this;
		}
		Protection& operator *=(double k)
		{
			_pad *= k;
			_deflect *= k;
			_fireproof *= k;
			_frostproof *= k;
			_cleanse *= k;
			return *this;
		}

		constexpr double pad() const { return _pad; }
		constexpr double deflect() const { return _deflect; }
		constexpr double fireproof() const { return _fireproof; }
		constexpr double frostproof() const { return _frostproof; }
		constexpr double cleanse() const { return _cleanse; }

		/// @return The amount of damage by which this protection reduces incoming damage.
		Damage reduction() const
		{
			return Damage
				{ Slash{_pad * percent_pad_to_slash + _deflect * percent_deflect_to_slash}
				, Pierce{_pad * percent_pad_to_pierce + _deflect * percent_deflect_to_pierce}
				, Bludgeon{_pad * percent_pad_to_bludgeon + _deflect * percent_deflect_to_bludgeon}
				, Burn{_fireproof * percent_fireproof_to_burn}
				, Freeze{_frostproof * percent_frostproof_to_freeze}
				, Blight{_cleanse * percent_cleanse_to_blight}
				};
		}
	private:
		double _pad = 0.0;
		double _deflect = 0.0;
		double _fireproof = 0.0;
		double _frostproof = 0.0;
		double _cleanse = 0.0;
	};
}
