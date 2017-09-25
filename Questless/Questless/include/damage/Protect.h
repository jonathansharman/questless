//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <iostream>

#include "utility/TaggedType.h"
#include "utility/Bounded.h"

namespace questless::dmg
{
	struct Pad : TaggedType<double> { using TaggedType::TaggedType; };
	struct Deflect : TaggedType<double> { using TaggedType::TaggedType; };
	struct Fireproof : TaggedType<double> { using TaggedType::TaggedType; };
	struct Frostproof : TaggedType<double> { using TaggedType::TaggedType; };
	struct Fortify : TaggedType<double> { using TaggedType::TaggedType; };
	struct Immunize : TaggedType<double> { using TaggedType::TaggedType; };
	struct Insulate : TaggedType<double> { using TaggedType::TaggedType; };

	//! A fixed reduction to damage, by type.
	class Protect
	{
	public:
		constexpr Protect() = default;

		constexpr Protect(Pad pad, Deflect deflect, Fireproof fireproof, Frostproof frostproof, Fortify fortify, Immunize immunize, Insulate insulate)
			: _pad{std::move(pad)}
			, _deflect{std::move(deflect)}
			, _fireproof{std::move(fireproof)}
			, _frostproof{std::move(frostproof)}
			, _fortify{std::move(fortify)}
			, _immunize{std::move(immunize)}
			, _insulate{std::move(insulate)}
		{}

		constexpr Protect(Pad pad) : _pad{std::move(pad)} {}
		constexpr Protect(Deflect deflect) : _deflect{std::move(deflect)} {}
		constexpr Protect(Fireproof fireproof) : _fireproof{std::move(fireproof)} {}
		constexpr Protect(Frostproof frostproof) : _frostproof{std::move(frostproof)} {}
		constexpr Protect(Fortify fortify) : _fortify{std::move(fortify)} {}
		constexpr Protect(Immunize immunize) : _immunize{std::move(immunize)} {}
		constexpr Protect(Insulate insulate) : _insulate{std::move(insulate)} {}

		static constexpr Protect zero() { return Protect{}; }

		friend std::ostream& operator <<(std::ostream& out, Protect const& p)
		{
			out << p._pad.get() << ' '
				<< p._deflect.get() << ' '
				<< p._fireproof.get() << ' '
				<< p._frostproof.get() << ' '
				<< p._fortify.get() << ' '
				<< p._immunize << ' '
				<< p._insulate << ' ';
			return out;
		}

		friend std::istream& operator >> (std::istream& in, Protect& p)
		{
			in >> p._pad >> p._deflect >> p._fireproof >> p._frostproof >> p._fortify >> p._immunize >> p._insulate;
			return in;
		}
		
		friend Protect operator +(Protect const& p1, Protect const& p2)
		{
			return Protect
				{ Pad{p1._pad + p2._pad}
				, Deflect{p1._deflect + p2._deflect}
				, Fireproof{p1._fireproof + p2._fireproof}
				, Frostproof{p1._frostproof + p2._frostproof}
				, Fortify{p1._fortify + p2._fortify}
				, Immunize{p1._immunize + p2._immunize}
				, Insulate{p1._insulate + p2._insulate}
			};
		}
		friend Protect operator -(Protect const& p1, Protect const& p2)
		{
			return Protect
				{ Pad{p1._pad - p2._pad}
				, Deflect{p1._deflect - p2._deflect}
				, Fireproof{p1._fireproof - p2._fireproof}
				, Frostproof{p1._frostproof - p2._frostproof}
				, Fortify{p1._fortify - p2._fortify}
				, Immunize{p1._immunize - p2._immunize}
				, Insulate{p1._insulate - p2._insulate}
				};
		}
		friend Protect operator *(Protect const& p, double k)
		{
			return Protect
				{ Pad{k * p._pad}
				, Deflect{k * p._deflect}
				, Fireproof{k * p._fireproof}
				, Frostproof{k * p._frostproof}
				, Fortify{k * p._fortify}
				, Immunize{k * p._immunize}
				, Insulate{k * p._insulate}
				};
		}
		friend Protect operator *(double k, Protect const& p)
		{
			return Protect
				{ Pad{k * p._pad}
				, Deflect{k * p._deflect}
				, Fireproof{k * p._fireproof}
				, Frostproof{k * p._frostproof}
				, Fortify{k * p._fortify}
				, Immunize{k * p._immunize}
				, Insulate{k * p._insulate}
				};
		}
		friend Protect operator /(Protect const& p, double k)
		{
			return Protect
				{ Pad{p._pad / k}
				, Deflect{p._deflect / k}
				, Fireproof{p._fireproof / k}
				, Frostproof{p._frostproof / k}
				, Fortify{p._fortify / k}
				, Immunize{p._immunize / k}
				, Insulate{p._insulate / k}
				};
		}

		Protect& operator +=(Protect const& p)
		{
			_pad += p._pad;
			_deflect += p._deflect;
			_fireproof += p._fireproof;
			_frostproof += p._frostproof;
			_fortify += p._fortify;
			_immunize += p._immunize;
			_insulate += p._insulate;
			return *this;
		}
		Protect& operator -=(Protect const& p)
		{
			_pad -= p._pad;
			_deflect -= p._deflect;
			_fireproof -= p._fireproof;
			_frostproof -= p._frostproof;
			_fortify -= p._fortify;
			_immunize -= p._immunize;
			_insulate -= p._insulate;
			return *this;
		}
		Protect& operator *=(double k)
		{
			_pad *= k;
			_deflect *= k;
			_fireproof *= k;
			_frostproof *= k;
			_fortify *= k;
			_immunize *= k;
			_insulate *= k;
			return *this;
		}
		Protect& operator /=(double k)
		{
			_pad /= k;
			_deflect /= k;
			_fireproof /= k;
			_frostproof /= k;
			_fortify /= k;
			_immunize /= k;
			_insulate /= k;
			return *this;
		}

		constexpr double pad() const { return _pad; }
		constexpr double deflect() const { return _deflect; }
		constexpr double fireproof() const { return _fireproof; }
		constexpr double frostproof() const { return _frostproof; }
		constexpr double fortify() const { return _fortify; }
		constexpr double immunize() const { return _immunize; }
		constexpr double insulate() const { return _insulate; }
	private:
		static constexpr double minimum_value = 0.0;

		Bounded<double, minimum_value> _pad = 0.0;
		Bounded<double, minimum_value> _deflect = 0.0;
		Bounded<double, minimum_value> _fireproof = 0.0;
		Bounded<double, minimum_value> _frostproof = 0.0;
		Bounded<double, minimum_value> _fortify = 0.0;
		Bounded<double, minimum_value> _immunize = 0.0;
		Bounded<double, minimum_value> _insulate = 0.0;
	};
}
