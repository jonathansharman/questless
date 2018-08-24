//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <iostream>

#include "utility/tagged_type.hpp"
#include "utility/static_bounded.hpp"

namespace ql::dmg {
	struct pad : tagged_type<double> { using tagged_type::tagged_type; };
	struct deflect : tagged_type<double> { using tagged_type::tagged_type; };
	struct fireproof : tagged_type<double> { using tagged_type::tagged_type; };
	struct frostproof : tagged_type<double> { using tagged_type::tagged_type; };
	struct fortify : tagged_type<double> { using tagged_type::tagged_type; };
	struct immunize : tagged_type<double> { using tagged_type::tagged_type; };
	struct insulate : tagged_type<double> { using tagged_type::tagged_type; };

	//! A fixed reduction to damage, by type.
	class protect {
	public:
		constexpr protect() = default;

		constexpr protect(pad pad, deflect deflect, fireproof fireproof, frostproof frostproof, fortify fortify, immunize immunize, insulate insulate)
			: _pad{std::move(pad)}
			, _deflect{std::move(deflect)}
			, _fireproof{std::move(fireproof)}
			, _frostproof{std::move(frostproof)}
			, _fortify{std::move(fortify)}
			, _immunize{std::move(immunize)}
			, _insulate{std::move(insulate)}
		{}

		constexpr protect(pad pad) : _pad{std::move(pad)} {}
		constexpr protect(deflect deflect) : _deflect{std::move(deflect)} {}
		constexpr protect(fireproof fireproof) : _fireproof{std::move(fireproof)} {}
		constexpr protect(frostproof frostproof) : _frostproof{std::move(frostproof)} {}
		constexpr protect(fortify fortify) : _fortify{std::move(fortify)} {}
		constexpr protect(immunize immunize) : _immunize{std::move(immunize)} {}
		constexpr protect(insulate insulate) : _insulate{std::move(insulate)} {}

		static constexpr protect zero() { return protect{}; }

		friend std::ostream& operator <<(std::ostream& out, protect const& p) {
			out << p._pad.value() << ' '
				<< p._deflect.value() << ' '
				<< p._fireproof.value() << ' '
				<< p._frostproof.value() << ' '
				<< p._fortify.value() << ' '
				<< p._immunize << ' '
				<< p._insulate << ' ';
			return out;
		}

		friend std::istream& operator >> (std::istream& in, protect& p) {
			in >> p._pad >> p._deflect >> p._fireproof >> p._frostproof >> p._fortify >> p._immunize >> p._insulate;
			return in;
		}
		
		friend protect operator +(protect const& p1, protect const& p2) {
			return protect {
				dmg::pad{p1._pad + p2._pad},
				dmg::deflect{p1._deflect + p2._deflect},
				dmg::fireproof{p1._fireproof + p2._fireproof},
				dmg::frostproof{p1._frostproof + p2._frostproof},
				dmg::fortify{p1._fortify + p2._fortify},
				dmg::immunize{p1._immunize + p2._immunize},
				dmg::insulate{p1._insulate + p2._insulate},
			};
		}
		friend protect operator -(protect const& p1, protect const& p2) {
			return protect {
				dmg::pad{p1._pad - p2._pad},
				dmg::deflect{p1._deflect - p2._deflect},
				dmg::fireproof{p1._fireproof - p2._fireproof},
				dmg::frostproof{p1._frostproof - p2._frostproof},
				dmg::fortify{p1._fortify - p2._fortify},
				dmg::immunize{p1._immunize - p2._immunize},
				dmg::insulate{p1._insulate - p2._insulate},
			};
		}
		friend protect operator *(protect const& p, double k) {
			return protect {
				dmg::pad{k * p._pad},
				dmg::deflect{k * p._deflect},
				dmg::fireproof{k * p._fireproof},
				dmg::frostproof{k * p._frostproof},
				dmg::fortify{k * p._fortify},
				dmg::immunize{k * p._immunize},
				dmg::insulate{k * p._insulate},
			};
		}
		friend protect operator *(double k, protect const& p) {
			return protect {
				dmg::pad{k * p._pad},
				dmg::deflect{k * p._deflect},
				dmg::fireproof{k * p._fireproof},
				dmg::frostproof{k * p._frostproof},
				dmg::fortify{k * p._fortify},
				dmg::immunize{k * p._immunize},
				dmg::insulate{k * p._insulate},
			};
		}
		friend protect operator /(protect const& p, double k) {
			return protect {
				dmg::pad{p._pad / k},
				dmg::deflect{p._deflect / k},
				dmg::fireproof{p._fireproof / k},
				dmg::frostproof{p._frostproof / k},
				dmg::fortify{p._fortify / k},
				dmg::immunize{p._immunize / k},
				dmg::insulate{p._insulate / k},
			};
		}

		protect& operator +=(protect const& p) {
			_pad += p._pad;
			_deflect += p._deflect;
			_fireproof += p._fireproof;
			_frostproof += p._frostproof;
			_fortify += p._fortify;
			_immunize += p._immunize;
			_insulate += p._insulate;
			return *this;
		}
		protect& operator -=(protect const& p) {
			_pad -= p._pad;
			_deflect -= p._deflect;
			_fireproof -= p._fireproof;
			_frostproof -= p._frostproof;
			_fortify -= p._fortify;
			_immunize -= p._immunize;
			_insulate -= p._insulate;
			return *this;
		}
		protect& operator *=(double k) {
			_pad *= k;
			_deflect *= k;
			_fireproof *= k;
			_frostproof *= k;
			_fortify *= k;
			_immunize *= k;
			_insulate *= k;
			return *this;
		}
		protect& operator /=(double k) {
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

		static_bounded<double, minimum_value> _pad = 0.0;
		static_bounded<double, minimum_value> _deflect = 0.0;
		static_bounded<double, minimum_value> _fireproof = 0.0;
		static_bounded<double, minimum_value> _frostproof = 0.0;
		static_bounded<double, minimum_value> _fortify = 0.0;
		static_bounded<double, minimum_value> _immunize = 0.0;
		static_bounded<double, minimum_value> _insulate = 0.0;
	};
}
