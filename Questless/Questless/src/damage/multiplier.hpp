//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "damage.hpp"

#include "utility/io.hpp"
#include "utility/nonnegative.hpp"
#include "utility/utility.hpp"

namespace ql::dmg {
	//! Base type for resist and vuln, using CRTP.
	template <typename Derived>
	struct multiplier {
		nonnegative<slash_factor> slash = 0.0_slash_factor;
		nonnegative<pierce_factor> pierce = 0.0_pierce_factor;
		nonnegative<cleave_factor> cleave = 0.0_cleave_factor;
		nonnegative<bludgeon_factor> bludgeon = 0.0_bludgeon_factor;
		nonnegative<burn_factor> burn = 0.0_burn_factor;
		nonnegative<freeze_factor> freeze = 0.0_freeze_factor;
		nonnegative<blight_factor> blight = 0.0_blight_factor;
		nonnegative<poison_factor> poison = 0.0_poison_factor;
		nonnegative<shock_factor> shock = 0.0_shock_factor;

		constexpr multiplier() = default;

		constexpr multiplier
			( slash_factor slash
			, pierce_factor pierce
			, cleave_factor cleave
			, bludgeon_factor bludgeon
			, burn_factor burn
			, freeze_factor freeze
			, blight_factor blight
			, poison_factor poison
			, shock_factor shock
		)
			: slash{slash}
			, pierce{pierce}
			, cleave{cleave}
			, bludgeon{bludgeon}
			, burn{burn}
			, freeze{freeze}
			, blight{blight}
			, poison{poison}
			, shock{shock}
		{}

		constexpr multiplier(factor factor) {
			std::visit(overloaded
			([this](slash_factor slash_factor) { slash = slash_factor; }
			, [this](pierce_factor pierce_factor) { pierce = pierce_factor; }
			, [this](cleave_factor cleave_factor) { cleave = cleave_factor; }
			, [this](bludgeon_factor bludgeon_factor) { bludgeon = bludgeon_factor; }
			, [this](burn_factor burn_factor) { burn = burn_factor; }
			, [this](freeze_factor freeze_factor) { freeze = freeze_factor; }
			, [this](blight_factor blight_factor) { blight = blight_factor; }
			, [this](poison_factor poison_factor) { poison = poison_factor; }
			, [this](shock_factor shock_factor) { shock = shock_factor; }
			), factor);
		}

		static constexpr Derived zero() { return Derived{}; }

		constexpr friend Derived operator +(Derived const& d1, Derived const& d2) {
			return Derived
			{d1.slash + d2.slash
			, d1.pierce + d2.pierce
			, d1.cleave + d2.cleave
			, d1.bludgeon + d2.bludgeon
			, d1.burn + d2.burn
			, d1.freeze + d2.freeze
			, d1.blight + d2.blight
			, d1.poison + d2.poison
			, d1.shock + d2.shock,
			};
		}
		friend constexpr Derived operator -(Derived const& d1, Derived const& d2) {
			Derived difference;
			difference.slash = d1.slash - d2.slash;
			difference.pierce = d1.pierce - d2.pierce;
			difference.cleave = d1.cleave - d2.cleave;
			difference.bludgeon = d1.bludgeon - d2.bludgeon;
			difference.burn = d1.burn - d2.burn;
			difference.freeze = d1.freeze - d2.freeze;
			difference.blight = d1.blight - d2.blight;
			difference.poison = d1.poison - d2.poison;
			difference.shock = d1.shock - d2.shock;
			return difference;
		}
		friend constexpr Derived operator *(Derived const& d, double k) {
			return Derived
			{d.slash * k
			, d.pierce * k
			, d.cleave * k
			, d.bludgeon * k
			, d.burn * k
			, d.freeze * k
			, d.blight * k
			, d.poison * k
			, d.shock * k
			};
		}
		friend constexpr Derived operator *(double k, Derived const& d) {
			return Derived{
				slash_factor{k * d.slash},
				pierce_factor{k * d.pierce},
				cleave_factor{k * d.cleave},
				bludgeon_factor{k * d.bludgeon},
				burn_factor{k * d.burn},
				freeze_factor{k * d.freeze},
				blight_factor{k * d.blight},
				poison_factor{k * d.poison},
				shock_factor{k * d.shock},
			};
		}
		friend constexpr Derived operator /(Derived const& d, double k) {
			return Derived{
				d.slash / k,
				d.pierce / k,
				d.cleave / k,
				d.bludgeon / k,
				d.burn / k,
				d.freeze / k,
				d.blight / k,
				d.poison / k,
				d.shock / k,
			};
		}

		Derived& operator +=(Derived const& addend) {
			slash += addend.slash;
			pierce += addend.pierce;
			cleave += addend.cleave;
			bludgeon += addend.bludgeon;
			burn += addend.burn;
			freeze += addend.freeze;
			blight += addend.blight;
			poison += addend.poison;
			shock += addend.shock;
			return static_cast<Derived&>(*this);
		}
		Derived& operator -=(Derived const& d) {
			slash -= d.slash;
			pierce -= d.pierce;
			cleave -= d.cleave;
			bludgeon -= d.bludgeon;
			burn -= d.burn;
			freeze -= d.freeze;
			blight -= d.blight;
			poison -= d.poison;
			shock -= d.shock;
			return static_cast<Derived&>(*this);
		}
		Derived& operator *=(double k) {
			slash *= k;
			pierce *= k;
			cleave *= k;
			bludgeon *= k;
			burn *= k;
			freeze *= k;
			blight *= k;
			poison *= k;
			shock *= k;
			return static_cast<Derived&>(*this);
		}
	};

	//! Reduces the percentage damage taken
	class resist : public multiplier<resist> {
	public:
		using multiplier<resist>::multiplier;
	};

	//! Increases the percentage of damage taken.
	class vuln : public multiplier<vuln> {
	public:
		using multiplier<vuln>::multiplier;
	};

	void to_json(nlohmann::json& j, resist const& resist) {
		if (resist.slash != 0.0_slash_factor) to_json(j["slash"], resist.slash);
		if (resist.pierce != 0.0_pierce_factor) to_json(j["pierce"], resist.pierce);
		if (resist.cleave != 0.0_cleave_factor) to_json(j["cleave"], resist.cleave);
		if (resist.bludgeon != 0.0_bludgeon_factor) to_json(j["bludgeon"], resist.bludgeon);
		if (resist.burn != 0.0_burn_factor) to_json(j["burn"], resist.burn);
		if (resist.freeze != 0.0_freeze_factor) to_json(j["freeze"], resist.freeze);
		if (resist.blight != 0.0_blight_factor) to_json(j["blight"], resist.blight);
		if (resist.poison != 0.0_poison_factor) to_json(j["poison"], resist.poison);
		if (resist.shock != 0.0_shock_factor) to_json(j["shock"], resist.shock);
	}

	void from_json(nlohmann::json const& j, resist& resist) {
		auto const slash = j.find("slash");
		if (slash != j.end()) from_json(slash.value(), resist.slash);

		auto const pierce = j.find("pierce");
		if (pierce != j.end()) from_json(pierce.value(), resist.pierce);

		auto const cleave = j.find("cleave");
		if (cleave != j.end()) from_json(cleave.value(), resist.cleave);

		auto const bludgeon = j.find("bludgeon");
		if (bludgeon != j.end()) from_json(bludgeon.value(), resist.bludgeon);

		auto const burn = j.find("burn");
		if (burn != j.end()) from_json(burn.value(), resist.burn);

		auto const freeze = j.find("freeze");
		if (freeze != j.end()) from_json(freeze.value(), resist.freeze);

		auto const blight = j.find("blight");
		if (blight != j.end()) from_json(blight.value(), resist.blight);

		auto const poison = j.find("poison");
		if (poison != j.end()) from_json(poison.value(), resist.poison);

		auto const shock = j.find("shock");
		if (shock != j.end()) from_json(shock.value(), resist.shock);
	}

	void to_json(nlohmann::json& j, vuln const& vuln) {
		if (vuln.slash != 0.0_slash_factor) to_json(j["slash"], vuln.slash);
		if (vuln.pierce != 0.0_pierce_factor) to_json(j["pierce"], vuln.pierce);
		if (vuln.cleave != 0.0_cleave_factor) to_json(j["cleave"], vuln.cleave);
		if (vuln.bludgeon != 0.0_bludgeon_factor) to_json(j["bludgeon"], vuln.bludgeon);
		if (vuln.burn != 0.0_burn_factor) to_json(j["burn"], vuln.burn);
		if (vuln.freeze != 0.0_freeze_factor) to_json(j["freeze"], vuln.freeze);
		if (vuln.blight != 0.0_blight_factor) to_json(j["blight"], vuln.blight);
		if (vuln.poison != 0.0_poison_factor) to_json(j["poison"], vuln.poison);
		if (vuln.shock != 0.0_shock_factor) to_json(j["shock"], vuln.shock);
	}

	void from_json(nlohmann::json const& j, vuln& vuln) {
		auto const slash = j.find("slash");
		if (slash != j.end()) from_json(slash.value(), vuln.slash);

		auto const pierce = j.find("pierce");
		if (pierce != j.end()) from_json(pierce.value(), vuln.pierce);

		auto const cleave = j.find("cleave");
		if (cleave != j.end()) from_json(cleave.value(), vuln.cleave);

		auto const bludgeon = j.find("bludgeon");
		if (bludgeon != j.end()) from_json(bludgeon.value(), vuln.bludgeon);

		auto const burn = j.find("burn");
		if (burn != j.end()) from_json(burn.value(), vuln.burn);

		auto const freeze = j.find("freeze");
		if (freeze != j.end()) from_json(freeze.value(), vuln.freeze);

		auto const blight = j.find("blight");
		if (blight != j.end()) from_json(blight.value(), vuln.blight);

		auto const poison = j.find("poison");
		if (poison != j.end()) from_json(poison.value(), vuln.poison);

		auto const shock = j.find("shock");
		if (shock != j.end()) from_json(shock.value(), vuln.shock);
	}
}
