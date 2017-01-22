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

namespace questless
{
	struct Slash : TaggedType<double> { using TaggedType::TaggedType; };
	struct Pierce : TaggedType<double> { using TaggedType::TaggedType; };
	struct Bludgeon : TaggedType<double> { using TaggedType::TaggedType; };
	struct Burn : TaggedType<double> { using TaggedType::TaggedType; };
	struct Freeze : TaggedType<double> { using TaggedType::TaggedType; };
	struct Blight : TaggedType<double> { using TaggedType::TaggedType; };

	template <typename Derived>
	class DamageMultiplier
	{
	public:
		constexpr DamageMultiplier() = default;

		constexpr DamageMultiplier(Slash slash, Pierce pierce, Bludgeon bludgeon, Burn burn, Freeze freeze, Blight blight)
			: _slash{slash}, _pierce{pierce}, _bludgeon{bludgeon}, _burn{burn}, _freeze{freeze}, _blight{blight}
		{}

		constexpr DamageMultiplier(Slash slash) : _slash{slash}, _pierce{0.0}, _bludgeon{0.0}, _burn{0.0}, _freeze{0.0}, _blight{0.0} {}
		constexpr DamageMultiplier(Pierce pierce) : slash{0.0}, pierce{pierce}, bludgeon{0.0}, burn{0.0}, freeze{0.0}, blight{0.0} {}
		constexpr DamageMultiplier(Bludgeon bludgeon) : slash{0.0}, pierce{0.0}, bludgeon{bludgeon}, burn{0.0}, freeze{0.0}, blight{0.0} {}
		constexpr DamageMultiplier(Burn burn) : slash{0.0}, pierce{0.0}, bludgeon{0.0}, burn{burn}, freeze{0.0}, blight{0.0} {}
		constexpr DamageMultiplier(Freeze freeze) : slash{0.0}, pierce{0.0}, bludgeon{0.0}, burn{0.0}, freeze{freeze}, blight{0.0} {}
		constexpr DamageMultiplier(Blight blight) : slash{0.0}, pierce{0.0}, bludgeon{0.0}, burn{0.0}, freeze{0.0}, blight{blight} {}

		static constexpr Derived zero() { return Derived{}; }

		constexpr friend Derived operator +(Derived const& d1, Derived const& d2)
		{
			return Derived
				{ d1.slash + d2.slash
				, d1.pierce + d2.pierce
				, d1.bludgeon + d2.bludgeon
				, d1.burn + d2.burn
				, d1.freeze + d2.freeze
				, d1.blight + d2.blight
				};
		}
		friend constexpr Derived operator -(Derived const& d1, Derived const& d2)
		{
			Derived difference;
			difference.slash = max(0.0, d1.slash - d2.slash);
			difference.pierce = max(0.0, d1.pierce - d2.pierce);
			difference.bludgeon = max(0.0, d1.bludgeon - d2.bludgeon);
			difference.burn = max(0.0, d1.burn - d2.burn);
			difference.freeze = max(0.0, d1.freeze - d2.freeze);
			difference.blight = max(0.0, d1.blight - d2.blight);
			return difference;
		}
		friend constexpr Derived operator *(Derived const& d, double k)
		{
			return Derived
				{ Slash{k * d._slash}
				, Pierce{k * d._pierce}
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
				, Bludgeon{k * d._bludgeon}
				, Burn{k * d._burn}
				, Freeze{k * d._freeze}
				, Blight{k * d._blight}
				};
		}
		friend constexpr Derived operator /(Derived const& d, double k)
		{
			return Derived{d.slash / k, d.pierce / k, d.bludgeon / k, d.burn / k, d.freeze / k, d.blight / k};
		}

		Derived& operator +=(Derived const& addend)
		{
			_slash += addend._slash;
			_pierce += addend._pierce;
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
			_bludgeon *= k;
			_burn *= k;
			_freeze *= k;
			_blight *= k;
			return static_cast<Derived&>(*this);
		}

		constexpr double slash() const { return _slash; }
		constexpr double pierce() const { return _pierce; }
		constexpr double bludgeon() const { return _bludgeon; }
		constexpr double burn() const { return _burn; }
		constexpr double freeze() const { return _freeze; }
		constexpr double blight() const { return _blight; }
	private:
		double _slash = 0.0;
		double _pierce = 0.0;
		double _bludgeon = 0.0;
		double _burn = 0.0;
		double _freeze = 0.0;
		double _blight = 0.0;
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
