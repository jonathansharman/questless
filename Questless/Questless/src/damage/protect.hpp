//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "bounded/nonnegative.hpp"
#include "bounded/static.hpp"

#include "cancel/quantity.hpp"

namespace ql {
	namespace dmg {
		//! A fixed reduction to damage, by type.
		struct protect {
			nonnegative<slash> slash = 0_slash;
			nonnegative<pierce> pierce = 0_pierce;
			nonnegative<cleave> cleave = 0_cleave;
			nonnegative<bludgeon> bludgeon = 0_bludgeon;
			nonnegative<scorch> scorch = 0_scorch;
			nonnegative<freeze> freeze = 0_freeze;
			nonnegative<shock> shock = 0_shock;
			nonnegative<poison> poison = 0_poison;
			nonnegative<rot> rot = 0_rot;

			constexpr protect() noexcept = default;

			constexpr protect(protect&&) noexcept = default;
			constexpr protect(protect const&) = default;

			constexpr protect& operator=(protect&&) noexcept = default;
			constexpr protect& operator=(protect const&) = default;

			constexpr protect(dmg::slash slash) : slash{slash} {}
			constexpr protect(dmg::pierce pierce) : pierce{pierce} {}
			constexpr protect(dmg::cleave cleave) : cleave{cleave} {}
			constexpr protect(dmg::bludgeon bludgeon) : bludgeon{bludgeon} {}
			constexpr protect(dmg::scorch scorch) : scorch{scorch} {}
			constexpr protect(dmg::freeze freeze) : freeze{freeze} {}
			constexpr protect(dmg::shock shock) : shock{shock} {}
			constexpr protect(dmg::poison poison) : poison{poison} {}
			constexpr protect(dmg::rot rot) : rot{rot} {}

			protect& operator+=(protect const& p) {
				slash += p.slash;
				pierce += p.pierce;
				cleave += p.cleave;
				bludgeon += p.bludgeon;
				scorch += p.scorch;
				freeze += p.freeze;
				shock += p.shock;
				poison += p.poison;
				rot += p.rot;
				return *this;
			}
			protect& operator-=(protect const& p) {
				slash -= p.slash;
				pierce -= p.pierce;
				cleave -= p.cleave;
				bludgeon -= p.bludgeon;
				scorch -= p.scorch;
				freeze -= p.freeze;
				shock -= p.shock;
				poison -= p.poison;
				rot -= p.rot;
				return *this;
			}
			protect& operator*=(int k) {
				slash *= k;
				pierce *= k;
				cleave *= k;
				bludgeon *= k;
				scorch *= k;
				freeze *= k;
				shock *= k;
				poison *= k;
				rot *= k;
				return *this;
			}
			protect& operator/=(int k) {
				slash /= k;
				pierce /= k;
				cleave /= k;
				bludgeon /= k;
				scorch /= k;
				freeze /= k;
				shock /= k;
				poison /= k;
				rot /= k;
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
		protect operator*(protect p, int k) {
			p *= k;
			return p;
		}
		protect operator*(int k, protect const& p) {
			// Multiplication of int is commutative, so it's okay to delegate to p * k.
			return p * k;
		}
		protect operator/(protect p, int k) {
			p /= k;
			return p;
		}
	}
}
