//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "damage.hpp"

#include "bounded/nonnegative.hpp"
#include "utility/utility.hpp"

namespace ql::dmg {
	//! Base type for resist and vuln, using CRTP.
	template <typename Derived>
	struct multiplier {
		nonnegative<slash_factor> slash = 0_slash_factor;
		nonnegative<pierce_factor> pierce = 0_pierce_factor;
		nonnegative<cleave_factor> cleave = 0_cleave_factor;
		nonnegative<bludgeon_factor> bludgeon = 0_bludgeon_factor;
		nonnegative<scorch_factor> scorch = 0_scorch_factor;
		nonnegative<freeze_factor> freeze = 0_freeze_factor;
		nonnegative<shock_factor> shock = 0_shock_factor;
		nonnegative<poison_factor> poison = 0_poison_factor;
		nonnegative<rot_factor> rot = 0_rot_factor;

		constexpr multiplier() = default;

		constexpr multiplier( //
			slash_factor slash,
			pierce_factor pierce,
			cleave_factor cleave,
			bludgeon_factor bludgeon,
			scorch_factor scorch,
			freeze_factor freeze,
			shock_factor shock,
			poison_factor poison,
			rot_factor rot)
			: slash{slash}
			, pierce{pierce}
			, cleave{cleave}
			, bludgeon{bludgeon}
			, scorch{scorch}
			, freeze{freeze}
			, shock{shock}
			, poison{poison}
			, rot{rot} {}

		constexpr multiplier(factor factor) {
			match(
				factor,
				[this](slash_factor f) { slash = f; },
				[this](pierce_factor f) { pierce = f; },
				[this](cleave_factor f) { cleave = f; },
				[this](bludgeon_factor f) { bludgeon = f; },
				[this](scorch_factor f) { scorch = f; },
				[this](freeze_factor f) { freeze = f; },
				[this](shock_factor f) { shock = f; },
				[this](poison_factor f) { poison = f; },
				[this](rot_factor f) { rot = f; });
		}

		auto& operator+=(Derived const& d) {
			slash += d.slash;
			pierce += d.pierce;
			cleave += d.cleave;
			bludgeon += d.bludgeon;
			scorch += d.scorch;
			freeze += d.freeze;
			shock += d.shock;
			poison += d.poison;
			rot += d.rot;
			return static_cast<Derived&>(*this);
		}
		auto& operator-=(Derived const& d) {
			slash -= d.slash;
			pierce -= d.pierce;
			cleave -= d.cleave;
			bludgeon -= d.bludgeon;
			scorch -= d.scorch;
			freeze -= d.freeze;
			shock -= d.shock;
			poison -= d.poison;
			rot -= d.rot;
			return static_cast<Derived&>(*this);
		}
		auto& operator*=(int k) {
			slash *= k;
			pierce *= k;
			cleave *= k;
			bludgeon *= k;
			scorch *= k;
			freeze *= k;
			shock *= k;
			poison *= k;
			rot *= k;
			return static_cast<Derived&>(*this);
		}
		auto& operator/=(int k) {
			slash /= k;
			pierce /= k;
			cleave /= k;
			bludgeon /= k;
			scorch /= k;
			freeze /= k;
			shock /= k;
			poison /= k;
			rot /= k;
			return static_cast<Derived&>(*this);
		}

		friend constexpr auto operator+(Derived d1, Derived const& d2) {
			d1 += d2;
			return d1;
		}
		friend constexpr auto operator-(Derived d1, Derived const& d2) {
			d1 -= d2;
			return d1;
		}
		friend constexpr auto operator*(Derived d, int k) {
			d *= k;
			return d;
		}
		friend constexpr auto operator*(int k, Derived d) {
			// int multiplication is commutative.
			d *= k;
			return d;
		}
		friend constexpr auto operator/(Derived d, int k) {
			d /= k;
			return d;
		}
	};

	//! Reduces the percentage damage taken
	struct resist : multiplier<resist> {
		using multiplier<resist>::multiplier;
	};

	//! Increases the percentage of damage taken.
	struct vuln : multiplier<vuln> {
		using multiplier<vuln>::multiplier;
	};
}
