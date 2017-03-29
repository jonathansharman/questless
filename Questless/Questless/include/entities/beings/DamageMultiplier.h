/**
* @file    DamageMultiplier.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines Resistance and Vulnerability, which reduce and increase damage taken, respectively.
*/

#pragma once

#include "utility/TaggedType.h"
#include "utility/Bounded.h"

namespace questless
{
	struct Slash : TaggedType<double> { using TaggedType::TaggedType; };
	struct Pierce : TaggedType<double> { using TaggedType::TaggedType; };
	struct Cleave : TaggedType<double> { using TaggedType::TaggedType; };
	struct Bludgeon : TaggedType<double> { using TaggedType::TaggedType; };
	struct Burn : TaggedType<double> { using TaggedType::TaggedType; };
	struct Freeze : TaggedType<double> { using TaggedType::TaggedType; };
	struct Blight : TaggedType<double> { using TaggedType::TaggedType; };

	template <typename Derived>
	class DamageMultiplier
	{
	public:
		constexpr DamageMultiplier() = default;

		constexpr DamageMultiplier(Slash slash, Pierce pierce, Cleave cleave, Bludgeon bludgeon, Burn burn, Freeze freeze, Blight blight)
			: _slash{slash}, _pierce{pierce}, _cleave{cleave}, _bludgeon{bludgeon}, _burn{burn}, _freeze{freeze}, _blight{blight}
		{}

		constexpr DamageMultiplier(Slash slash) : _slash{slash}, _pierce{0.0}, _cleave{0.0}, _bludgeon{0.0}, _burn{0.0}, _freeze{0.0}, _blight{0.0} {}
		constexpr DamageMultiplier(Pierce pierce) : _slash{0.0}, _pierce{pierce}, _cleave{0.0}, _bludgeon{0.0}, _burn{0.0}, _freeze{0.0}, _blight{0.0} {}
		constexpr DamageMultiplier(Cleave cleave) : _slash{0.0}, _pierce{0.0}, _cleave{cleave}, _bludgeon{0.0}, _burn{0.0}, _freeze{0.0}, _blight{0.0} {}
		constexpr DamageMultiplier(Bludgeon bludgeon) : _slash{0.0}, _pierce{0.0}, _cleave{0.0}, _bludgeon{bludgeon}, _burn{0.0}, _freeze{0.0}, _blight{0.0} {}
		constexpr DamageMultiplier(Burn burn) : _slash{0.0}, _pierce{0.0}, _cleave{0.0}, _bludgeon{0.0}, _burn{burn}, _freeze{0.0}, _blight{0.0} {}
		constexpr DamageMultiplier(Freeze freeze) : _slash{0.0}, _pierce{0.0}, _cleave{0.0}, _bludgeon{0.0}, _burn{0.0}, _freeze{freeze}, _blight{0.0} {}
		constexpr DamageMultiplier(Blight blight) : _slash{0.0}, _pierce{0.0}, _cleave{0.0}, _bludgeon{0.0}, _burn{0.0}, _freeze{0.0}, _blight{blight} {}

		static constexpr Derived zero() { return Derived{}; }

		constexpr friend Derived operator +(Derived const& d1, Derived const& d2)
		{
			return Derived
				{ Slash{d1._slash + d2._slash}
				, Pierce{d1._pierce + d2._pierce}
				, Cleave{d1._cleave + d2._cleave}
				, Bludgeon{d1._bludgeon + d2._bludgeon}
				, Burn{d1._burn + d2._burn}
				, Freeze{d1._freeze + d2._freeze}
				, Blight{d1._blight + d2._blight}
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
			return difference;
		}
		friend constexpr Derived operator *(Derived const& d, double k)
		{
			return Derived
				{ Slash{k * d._slash}
				, Pierce{k * d._pierce}
				, Cleave{k * d._cleave}
				, Bludgeon{k * d._bludgeon}
				, Burn{k * d._burn}
				, Freeze{k * d._freeze}
				, Blight{k * d._blight}
				};
		}
		friend constexpr Derived operator *(double k, Derived const& d)
		{
			return Derived
				{ Slash{k * d._slash}
				, Pierce{k * d._pierce}
				, Cleave{k * d._cleave}
				, Bludgeon{k * d._bludgeon}
				, Burn{k * d._burn}
				, Freeze{k * d._freeze}
				, Blight{k * d._blight}
				};
		}
		friend constexpr Derived operator /(Derived const& d, double k)
		{
			return Derived{d._slash / k, d._pierce / k, d._cleave / k, d._bludgeon / k, d._burn / k, d._freeze / k, d._blight / k};
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
			return static_cast<Derived&>(*this);
		}

		constexpr double slash() const { return _slash; }
		constexpr double pierce() const { return _pierce; }
		constexpr double cleave() const { return _cleave; }
		constexpr double bludgeon() const { return _bludgeon; }
		constexpr double burn() const { return _burn; }
		constexpr double freeze() const { return _freeze; }
		constexpr double blight() const { return _blight; }
	private:
		static constexpr double minimum_value = 0.0;
		Bounded<double, minimum_value> _slash = 0.0;
		Bounded<double, minimum_value> _pierce = 0.0;
		Bounded<double, minimum_value> _cleave = 0.0;
		Bounded<double, minimum_value> _bludgeon = 0.0;
		Bounded<double, minimum_value> _burn = 0.0;
		Bounded<double, minimum_value> _freeze = 0.0;
		Bounded<double, minimum_value> _blight = 0.0;
	};

	class Resistance : public DamageMultiplier<Resistance>
	{
	public:
		using DamageMultiplier<Resistance>::DamageMultiplier;
	};

	class Vulnerability : public DamageMultiplier<Vulnerability>
	{
	public:
		using DamageMultiplier<Vulnerability>::DamageMultiplier;
	};
}
