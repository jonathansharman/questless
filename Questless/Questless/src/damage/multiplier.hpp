//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "damage.hpp"

#include "utility/static_bounded.hpp"
#include "utility/tagged_type.hpp"
#include "utility/utility.hpp"

namespace ql::dmg {
	//! Base type for resist and vuln, using CRTP.
	template <typename Derived>
	class multiplier {
	public:
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
			: _slash{slash}
			, _pierce{pierce}
			, _cleave{cleave}
			, _bludgeon{bludgeon}
			, _burn{burn}
			, _freeze{freeze}
			, _blight{blight}
			, _poison{poison}
			, _shock{shock}
		{}

		constexpr multiplier(factor factor) {
			std::visit(overloaded
				( [this](slash_factor slash_factor) { _slash = slash_factor; }
				, [this](pierce_factor pierce_factor) { _pierce = pierce_factor; }
				, [this](cleave_factor cleave_factor) { _cleave = cleave_factor; }
				, [this](bludgeon_factor bludgeon_factor) { _bludgeon = bludgeon_factor; }
				, [this](burn_factor burn_factor) { _burn = burn_factor; }
				, [this](freeze_factor freeze_factor) { _freeze = freeze_factor; }
				, [this](blight_factor blight_factor) { _blight = blight_factor; }
				, [this](poison_factor poison_factor) { _poison = poison_factor; }
				, [this](shock_factor shock_factor) { _shock = shock_factor; }
			), factor);
		}

		static constexpr Derived zero() { return Derived{}; }

		constexpr friend Derived operator +(Derived const& d1, Derived const& d2) {
			return Derived {
				slash_factor{d1._slash + d2._slash},
				pierce_factor{d1._pierce + d2._pierce},
				cleave_factor{d1._cleave + d2._cleave},
				bludgeon_factor{d1._bludgeon + d2._bludgeon},
				burn_factor{d1._burn + d2._burn},
				freeze_factor{d1._freeze + d2._freeze},
				blight_factor{d1._blight + d2._blight},
				poison_factor{d1._poison + d2._poison},
				shock_factor{d1._shock + d2._shock},
			};
		}
		friend constexpr Derived operator -(Derived const& d1, Derived const& d2) {
			Derived difference;
			difference.slash = d1._slash - d2._slash;
			difference.pierce = d1._pierce - d2._pierce;
			difference.cleave = d1._cleave - d2._cleave;
			difference.bludgeon = d1._bludgeon - d2._bludgeon;
			difference.burn = d1._burn - d2._burn;
			difference.freeze = d1._freeze - d2._freeze;
			difference.blight = d1._blight - d2._blight;
			difference.poison = d1._poison - d2._poison;
			difference.shock = d1._shock - d2._shock;
			return difference;
		}
		friend constexpr Derived operator *(Derived const& d, double k) {
			return Derived {
				slash_factor{d._slash * k},
				pierce_factor{d._pierce * k},
				cleave_factor{d._cleave * k},
				bludgeon_factor{d._bludgeon * k},
				burn_factor{d._burn * k},
				freeze_factor{d._freeze * k},
				blight_factor{d._blight * k},
				poison_factor{d._poison * k},
				shock_factor{d._shock * k},
			};
		}
		friend constexpr Derived operator *(double k, Derived const& d) {
			return Derived {
				slash_factor{k * d._slash},
				pierce_factor{k * d._pierce},
				cleave_factor{k * d._cleave},
				bludgeon_factor{k * d._bludgeon},
				burn_factor{k * d._burn},
				freeze_factor{k * d._freeze},
				blight_factor{k * d._blight},
				poison_factor{k * d._poison},
				shock_factor{k * d._shock},
			};
		}
		friend constexpr Derived operator /(Derived const& d, double k) {
			return Derived {
				d._slash / k,
				d._pierce / k,
				d._cleave / k,
				d._bludgeon / k,
				d._burn / k,
				d._freeze / k,
				d._blight / k,
				d._poison / k,
				d._shock / k,
			};
		}

		Derived& operator +=(Derived const& addend) {
			_slash += addend._slash;
			_pierce += addend._pierce;
			_cleave += addend._cleave;
			_bludgeon += addend._bludgeon;
			_burn += addend._burn;
			_freeze += addend._freeze;
			_blight += addend._blight;
			_poison += addend._poison;
			_shock += addend._shock;
			return static_cast<Derived&>(*this);
		}
		Derived& operator -=(Derived const& d) {
			_slash -= d._slash;
			_pierce -= d._pierce;
			_cleave -= d._cleave;
			_bludgeon -= d._bludgeon;
			_burn -= d._burn;
			_freeze -= d._freeze;
			_blight -= d._blight;
			_poison -= d._poison;
			_shock -= d._shock;
			return static_cast<Derived&>(*this);
		}
		Derived& operator *=(double k) {
			_slash *= k;
			_pierce *= k;
			_cleave *= k;
			_bludgeon *= k;
			_burn *= k;
			_freeze *= k;
			_blight *= k;
			_poison *= k;
			_shock *= k;
			return static_cast<Derived&>(*this);
		}

		constexpr slash_factor slash() const { return _slash; }
		constexpr pierce_factor pierce() const { return _pierce; }
		constexpr cleave_factor cleave() const { return _cleave; }
		constexpr bludgeon_factor bludgeon() const { return _bludgeon; }
		constexpr burn_factor burn() const { return _burn; }
		constexpr freeze_factor freeze() const { return _freeze; }
		constexpr blight_factor blight() const { return _blight; }
		constexpr poison_factor poison() const { return _poison; }
		constexpr shock_factor shock() const { return _shock; }
	private:
		slash_factor _slash{0.0};
		pierce_factor _pierce{0.0};
		cleave_factor _cleave{0.0};
		bludgeon_factor _bludgeon{0.0};
		burn_factor _burn{0.0};
		freeze_factor _freeze{0.0};
		blight_factor _blight{0.0};
		poison_factor _poison{0.0};
		shock_factor _shock{0.0};
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
}
