/**
* @file    Damage.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The Damage type, which encapsulates a set of damage components inflicted together.
*/

#pragma once

#include <vector>
#include <cmath>

#include "DamageMultiplier.h"

namespace questless
{
	class Damage
	{
	public:
		constexpr Damage() = default;

		constexpr Damage(double slash, double pierce, double bludgeon, double burn, double freeze, double blight)
			: _slash{slash}, _pierce{pierce}, _bludgeon{bludgeon}, _burn{burn}, _freeze{freeze}, _blight{blight}
		{}

		constexpr Damage(Slash slash) : _slash{std::move(slash)}, _pierce{0.0}, _bludgeon{0.0}, _burn{0.0}, _freeze{0.0}, _blight{0.0} {}
		constexpr Damage(Pierce pierce) : _slash{0.0}, _pierce{std::move(pierce)}, _bludgeon{0.0}, _burn{0.0}, _freeze{0.0}, _blight{0.0} {}
		constexpr Damage(Bludgeon bludgeon) : _slash{0.0}, _pierce{0.0}, _bludgeon{std::move(bludgeon)}, _burn{0.0}, _freeze{0.0}, _blight{0.0} {}
		constexpr Damage(Burn burn) : _slash{0.0}, _pierce{0.0}, _bludgeon{0.0}, _burn{std::move(burn)}, _freeze{0.0}, _blight{0.0} {}
		constexpr Damage(Freeze freeze) : _slash{0.0}, _pierce{0.0}, _bludgeon{0.0}, _burn{0.0}, _freeze{std::move(freeze)}, _blight{0.0} {}
		constexpr Damage(Blight blight) : _slash{0.0}, _pierce{0.0}, _bludgeon{0.0}, _burn{0.0}, _freeze{0.0}, _blight{std::move(blight)} {}

		static constexpr Damage zero() { return Damage{}; }

		friend constexpr Damage operator +(Damage const& d1, Damage const& d2)
		{
			return Damage
				{ Slash{d1._slash + d2._slash}
				, Pierce{d1._pierce + d2._pierce}
				, Bludgeon{d1._bludgeon + d2._bludgeon}
				, Burn{d1._burn + d2._burn}
				, Freeze{d1._freeze + d2._freeze}
				, Blight{d1._blight + d2._blight}
				};
		}
		friend constexpr Damage operator -(Damage const& d1, Damage const& d2)
		{
			using std::max;
			return Damage
				{ Slash{max(0.0, d1._slash - d2._slash)}
				, Pierce{max(0.0, d1._pierce - d2._pierce)}
				, Bludgeon{max(0.0, d1._bludgeon - d2._bludgeon)}
				, Burn{max(0.0, d1._burn - d2._burn)}
				, Freeze{max(0.0, d1._freeze - d2._freeze)}
				, Blight{max(0.0, d1._blight - d2._blight)}
				};
		}
		friend constexpr Damage operator *(Damage const& d, double k)
		{
			return Damage{Slash{k * d._slash}, Pierce{k * d._pierce}, Bludgeon{k * d._bludgeon}, Burn{k * d._burn}, Freeze{k * d._freeze}, Blight{k * d._blight}};
		}
		friend constexpr Damage operator *(double k, Damage const& d)
		{
			return Damage{Slash{k * d._slash}, Pierce{k * d._pierce}, Bludgeon{k * d._bludgeon}, Burn{k * d._burn}, Freeze{k * d._freeze}, Blight{k * d._blight}};
		}
		friend constexpr Damage operator /(Damage const& d, double k)
		{
			return Damage{Slash{d._slash / k}, Pierce{d._pierce / k}, Bludgeon{d._bludgeon / k}, Burn{d._burn / k}, Freeze{d._freeze / k}, Blight{d._blight / k}};
		}

		Damage& operator +=(Damage const& d)
		{
			_slash += d._slash;
			_pierce += d._pierce;
			_bludgeon += d._bludgeon;
			_burn += d._burn;
			_freeze += d._freeze;
			_blight += d._blight;
			return *this;
		}
		Damage& operator -=(Damage const& d)
		{
			_slash -= d._slash;
			_pierce -= d._pierce;
			_bludgeon -= d._bludgeon;
			_burn -= d._burn;
			_freeze -= d._freeze;
			_blight -= d._blight;
			return *this;
		}
		Damage& operator *=(double k)
		{
			_slash *= k;
			_pierce *= k;
			_bludgeon *= k;
			_burn *= k;
			_freeze *= k;
			_blight *= k;
			return *this;
		}

		/// @return Damage adjusted by the given resistance and vulnerability.
		constexpr Damage with(Resistance const& resistance, Vulnerability const& vulnerability) const
		{
			return Damage
				{ Slash{std::max(0.0, _slash * (1.0 + (vulnerability.slash() - resistance.slash()) / 100.0))}
				, Pierce{std::max(0.0, _pierce * (1.0 + (vulnerability.pierce() - resistance.pierce()) / 100.0))}
				, Bludgeon{std::max(0.0, _bludgeon * (1.0 + (vulnerability.bludgeon() - resistance.bludgeon()) / 100.0))}
				, Burn{std::max(0.0, _burn * (1.0 + (vulnerability.burn() - resistance.burn()) / 100.0))}
				, Freeze{std::max(0.0, _freeze * (1.0 + (vulnerability.freeze() - resistance.freeze()) / 100.0))}
				, Blight{std::max(0.0, _blight * (1.0 + (vulnerability.blight() - resistance.blight()) / 100.0))}
				};
		}

		constexpr double slash() const { return _slash; }
		constexpr double pierce() const { return _pierce; }
		constexpr double bludgeon() const { return _bludgeon; }
		constexpr double burn() const { return _burn; }
		constexpr double freeze() const { return _freeze; }
		constexpr double blight() const { return _blight; }

		/// @return The sum of the damage components.
		constexpr double total() const { return _slash + _pierce + _bludgeon + _burn + _freeze + _blight; }
	private:
		double _slash = 0.0;
		double _pierce = 0.0;
		double _bludgeon = 0.0;
		double _burn = 0.0;
		double _freeze = 0.0;
		double _blight = 0.0;
	};
}
