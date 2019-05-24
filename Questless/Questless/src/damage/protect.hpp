//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/nonnegative.hpp"
#include "utility/static_bounded.hpp"

#include "cancel/quantity.hpp"

#include <cereal/cereal.hpp>

namespace ql {
	namespace dmg {
		using pad = cancel::quantity<double, cancel::unit_t<struct pad_tag>>;
		using deflect = cancel::quantity<double, cancel::unit_t<struct deflect_tag>>;
		using fireproof = cancel::quantity<double, cancel::unit_t<struct fireproof_tag>>;
		using frostproof = cancel::quantity<double, cancel::unit_t<struct frostproof_tag>>;
		using fortify = cancel::quantity<double, cancel::unit_t<struct fortify_tag>>;
		using immunize = cancel::quantity<double, cancel::unit_t<struct immunize_tag>>;
		using insulate = cancel::quantity<double, cancel::unit_t<struct insulate_tag>>;

		using coverage = cancel::quantity<double, cancel::unit_t<struct coverage_tag>>;
		constexpr coverage min_coverage = dmg::coverage{0.0};
		constexpr coverage max_coverage = dmg::coverage{100.0};

		//! A fixed reduction to damage, by type.
		struct protect {
			nonnegative<dmg::pad> pad = dmg::pad{0.0};
			nonnegative<dmg::deflect> deflect = dmg::deflect{0.0};
			nonnegative<dmg::fireproof> fireproof = dmg::fireproof{0.0};
			nonnegative<dmg::frostproof> frostproof = dmg::frostproof{0.0};
			nonnegative<dmg::fortify> fortify = dmg::fortify{0.0};
			nonnegative<dmg::immunize> immunize = dmg::immunize{0.0};
			nonnegative<dmg::insulate> insulate = dmg::insulate{0.0};

			static_bounded<dmg::coverage, min_coverage, max_coverage> coverage = dmg::coverage{0.0};

			constexpr protect() noexcept = default;

			constexpr protect(protect&&) noexcept = default;
			constexpr protect(protect const&) = default;

			constexpr protect& operator=(protect&&) noexcept = default;
			constexpr protect& operator=(protect const&) = default;

			constexpr protect(dmg::pad pad, dmg::coverage coverage) : pad{pad}, coverage{coverage} {}
			constexpr protect(dmg::deflect deflect, dmg::coverage coverage) : deflect{deflect}, coverage{coverage} {}
			constexpr protect(dmg::fireproof fireproof, dmg::coverage coverage)
				: fireproof{fireproof}, coverage{coverage} {}
			constexpr protect(dmg::frostproof frostproof, dmg::coverage coverage)
				: frostproof{frostproof}, coverage{coverage} {}
			constexpr protect(dmg::fortify fortify, dmg::coverage coverage) : fortify{fortify}, coverage{coverage} {}
			constexpr protect(dmg::immunize immunize, dmg::coverage coverage)
				: immunize{immunize}, coverage{coverage} {}
			constexpr protect(dmg::insulate insulate, dmg::coverage coverage)
				: insulate{insulate}, coverage{coverage} {}

			template <typename Archive>
			void save(Archive& archive) const {
				archive(CEREAL_NVP(pad),
					CEREAL_NVP(deflect),
					CEREAL_NVP(fireproof),
					CEREAL_NVP(frostproof),
					CEREAL_NVP(fortify),
					CEREAL_NVP(immunize),
					CEREAL_NVP(insulate),
					CEREAL_NVP(coverage));
			}

			template <typename Archive>
			void load(Archive& archive) {
				archive(CEREAL_NVP(pad),
					CEREAL_NVP(deflect),
					CEREAL_NVP(fireproof),
					CEREAL_NVP(frostproof),
					CEREAL_NVP(fortify),
					CEREAL_NVP(immunize),
					CEREAL_NVP(insulate),
					CEREAL_NVP(coverage));
			}

			protect& operator+=(protect const& p) {
				pad += p.pad;
				deflect += p.deflect;
				fireproof += p.fireproof;
				frostproof += p.frostproof;
				fortify += p.fortify;
				immunize += p.immunize;
				insulate += p.insulate;
				return *this;
			}
			protect& operator-=(protect const& p) {
				pad -= p.pad;
				deflect -= p.deflect;
				fireproof -= p.fireproof;
				frostproof -= p.frostproof;
				fortify -= p.fortify;
				immunize -= p.immunize;
				insulate -= p.insulate;
				return *this;
			}
			protect& operator*=(double k) {
				pad *= k;
				deflect *= k;
				fireproof *= k;
				frostproof *= k;
				fortify *= k;
				immunize *= k;
				insulate *= k;
				return *this;
			}
			protect& operator/=(double k) {
				pad /= k;
				deflect /= k;
				fireproof /= k;
				frostproof /= k;
				fortify /= k;
				immunize /= k;
				insulate /= k;
				return *this;
			}
		};

		protect operator+(protect p1, protect const& p2) {
			p1 += p2;
			return p1;
		}
		protect operator-(protect p1, protect const& p2) {
			p1 -= p2;
			return p1;
		}
		protect operator*(protect p, double k) {
			p *= k;
			return p;
		}
		protect operator*(double k, protect const& p) {
			// Multiplication of double is commutative, so it's okay to delegate to p * k.
			return p * k;
		}
		protect operator/(protect p, double k) {
			p /= k;
			return p;
		}
	}

	constexpr dmg::pad operator"" _pad(long double value) {
		return dmg::pad{static_cast<double>(value)};
	}
	constexpr dmg::deflect operator"" _deflect(long double value) {
		return dmg::deflect{static_cast<double>(value)};
	}
	constexpr dmg::fireproof operator"" _fireproof(long double value) {
		return dmg::fireproof{static_cast<double>(value)};
	}
	constexpr dmg::frostproof operator"" _frostproof(long double value) {
		return dmg::frostproof{static_cast<double>(value)};
	}
	constexpr dmg::fortify operator"" _fortify(long double value) {
		return dmg::fortify{static_cast<double>(value)};
	}
	constexpr dmg::immunize operator"" _immunize(long double value) {
		return dmg::immunize{static_cast<double>(value)};
	}
	constexpr dmg::insulate operator"" _insulate(long double value) {
		return dmg::insulate{static_cast<double>(value)};
	}
	constexpr dmg::coverage operator"" _coverage(long double value) {
		return dmg::coverage{static_cast<double>(value)};
	}
}
