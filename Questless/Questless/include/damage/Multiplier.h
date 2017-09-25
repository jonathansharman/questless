//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Damage.h"
#include "utility/TaggedType.h"
#include "utility/Bounded.h"

namespace questless::dmg
{
	//! Base type for Resist and Vuln, using CRTP.
	template <typename Derived>
	class Multiplier
	{
	public:
		constexpr Multiplier() = default;

		constexpr Multiplier
			( SlashFactor slash
			, PierceFactor pierce
			, CleaveFactor cleave
			, BludgeonFactor bludgeon
			, BurnFactor burn
			, FreezeFactor freeze
			, BlightFactor blight
			, PoisonFactor poison
			, ShockFactor shock
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

		constexpr Multiplier(Factor factor)
		{
			Multiplier& multiplier;

			friend struct PartFactorAssigner
			{
				void operator ()(SlashFactor slash_factor) { multiplier._slash = slash_factor; }
				void operator ()(PierceFactor pierce_factor) { multiplier._pierce = pierce_factor; }
				void operator ()(CleaveFactor cleave_factor) { multiplier._cleave = cleave_factor; }
				void operator ()(BludgeonFactor bludgeon_factor) { multiplier._bludgeon = bludgeon_factor; }
				void operator ()(BurnFactor burn_factor) { multiplier._burn = burn_factor; }
				void operator ()(FreezeFactor freeze_factor) { multiplier._freeze = freeze_factor; }
				void operator ()(BlightFactor blight_factor) { multiplier._blight = blight_factor; }
				void operator ()(PoisonFactor poison_factor) { multiplier._poison = poison_factor; }
				void operator ()(ShockFactor shock_factor) { multiplier._shock = shock_factor; }
			};
			std::visit(PartFactorAssigner{*this}, factor);
		}

		static constexpr Derived zero() { return Derived{}; }

		constexpr friend Derived operator +(Derived const& d1, Derived const& d2)
		{
			return Derived
				{ SlashFactor{d1._slash + d2._slash}
				, PierceFactor{d1._pierce + d2._pierce}
				, CleaveFactor{d1._cleave + d2._cleave}
				, BludgeonFactor{d1._bludgeon + d2._bludgeon}
				, BurnFactor{d1._burn + d2._burn}
				, FreezeFactor{d1._freeze + d2._freeze}
				, BlightFactor{d1._blight + d2._blight}
				, PoisonFactor{d1._poison + d2._poison}
				, ShockFactor{d1._shock + d2._shock}
				};
		}
		friend constexpr Derived operator -(Derived const& d1, Derived const& d2)
		{
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
		friend constexpr Derived operator *(Derived const& d, double k)
		{
			return Derived
				{ SlashFactor{d._slash * k}
				, PierceFactor{d._pierce * k}
				, CleaveFactor{d._cleave * k}
				, BludgeonFactor{d._bludgeon * k}
				, BurnFactor{d._burn * k}
				, FreezeFactor{d._freeze * k}
				, BlightFactor{d._blight * k}
				, PoisonFactor{d._poison * k}
				, ShockFactor{d._shock * k}
				};
		}
		friend constexpr Derived operator *(double k, Derived const& d)
		{
			return Derived
				{ SlashFactor{k * d._slash}
				, PierceFactor{k * d._pierce}
				, CleaveFactor{k * d._cleave}
				, BludgeonFactor{k * d._bludgeon}
				, BurnFactor{k * d._burn}
				, FreezeFactor{k * d._freeze}
				, BlightFactor{k * d._blight}
				, PoisonFactor{k * d._poison}
				, ShockFactor{k * d._shock}
				};
		}
		friend constexpr Derived operator /(Derived const& d, double k)
		{
			return Derived
				{ d._slash / k
				, d._pierce / k
				, d._cleave / k
				, d._bludgeon / k
				, d._burn / k
				, d._freeze / k
				, d._blight / k
				, d._poison / k
				, d._shock / k
				};
		}

		Derived& operator +=(Derived const& addend)
		{
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
		Derived& operator -=(Derived const& d)
		{
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
		Derived& operator *=(double k)
		{
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

		constexpr SlashFactor slash() const { return _slash; }
		constexpr PierceFactor pierce() const { return _pierce; }
		constexpr CleaveFactor cleave() const { return _cleave; }
		constexpr BludgeonFactor bludgeon() const { return _bludgeon; }
		constexpr BurnFactor burn() const { return _burn; }
		constexpr FreezeFactor freeze() const { return _freeze; }
		constexpr BlightFactor blight() const { return _blight; }
		constexpr PoisonFactor poison() const { return _poison; }
		constexpr ShockFactor shock() const { return _shock; }
	private:
		SlashFactor _slash{0.0};
		PierceFactor _pierce{0.0};
		CleaveFactor _cleave{0.0};
		BludgeonFactor _bludgeon{0.0};
		BurnFactor _burn{0.0};
		FreezeFactor _freeze{0.0};
		BlightFactor _blight{0.0};
		PoisonFactor _poison{0.0};
		ShockFactor _shock{0.0};
	};

	//! Reduces the percentage damage taken
	class Resist : public Multiplier<Resist>
	{
	public:
		using Multiplier<Resist>::Multiplier;
	};

	//! Increases the percentage of damage taken.
	class Vuln : public Multiplier<Vuln>
	{
	public:
		using Multiplier<Vuln>::Multiplier;
	};
}
