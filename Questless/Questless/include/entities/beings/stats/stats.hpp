//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>
#include <memory>

#include "damage/protect.hpp"
#include "damage/multiplier.hpp"
#include "utility/static_bounded.hpp"
#include "utility/tagged_type.hpp"
#include "vision.hpp"

namespace ql
{
	struct spirit : tagged_type<double> { using tagged_type::tagged_type; };
	struct health_regen : tagged_type<double> { using tagged_type::tagged_type; };
	struct strength : tagged_type<double> { using tagged_type::tagged_type; };
	struct endurance : tagged_type<double> { using tagged_type::tagged_type; };
	struct stamina : tagged_type<double> { using tagged_type::tagged_type; };
	struct agility : tagged_type<double> { using tagged_type::tagged_type; };
	struct stealth : tagged_type<double> { using tagged_type::tagged_type; };
	struct hearing : tagged_type<double> { using tagged_type::tagged_type; };
	struct intellect : tagged_type<double> { using tagged_type::tagged_type; };
	struct weight : tagged_type<double> { using tagged_type::tagged_type; };
	struct min_temp : tagged_type<double> { using tagged_type::tagged_type; };
	struct max_temp : tagged_type<double> { using tagged_type::tagged_type; };
	struct mute : tagged_type<bool> { using tagged_type::tagged_type; };

	//! Represents the stats of a being.
	struct stats
	{
		static constexpr double minimum_value = 0.0;

		static_bounded<double, minimum_value> spirit = 0.0;
		static_bounded<double, minimum_value> health_regen = 0.0;
		static_bounded<double, minimum_value> strength = 0.0;
		static_bounded<double, minimum_value> endurance = 0.0;
		static_bounded<double, minimum_value> stamina = 0.0;
		static_bounded<double, minimum_value> agility = 0.0;
		static_bounded<double, minimum_value> stealth = 0.0;
		vision vision;
		static_bounded<double, minimum_value> hearing = 0.0;
		static_bounded<double, minimum_value> intellect = 0.0;
		double weight = 0.0;
		static_bounded<double, minimum_value> min_temp = 0.0;
		static_bounded<double, minimum_value> max_temp = 0.0;
		bool mute = false;

		dmg::protect protection = dmg::protect::zero();
		dmg::resist resistance = dmg::resist::zero();
		dmg::vuln vulnerability = dmg::vuln::zero();

		constexpr stats() = default;

		constexpr stats
			( ql::spirit spirit
			, ql::health_regen health_regen
			, ql::strength strength
			, ql::endurance endurance
			, ql::stamina stamina
			, ql::agility agility
			, ql::stealth stealth
			, ql::vision vision
			, ql::hearing hearing
			, ql::intellect intellect
			, ql::weight weight
			, ql::min_temp min_temp
			, ql::max_temp max_temp
			, ql::mute mute
			, dmg::protect protection
			, dmg::resist resistance
			, dmg::vuln vulnerability
			)
			: spirit{std::move(spirit)}
			, health_regen{std::move(health_regen)}
			, strength{std::move(strength)}
			, endurance{std::move(endurance)}
			, stamina{std::move(stamina)}
			, agility{std::move(agility)}
			, stealth{std::move(stealth)}
			, vision{std::move(vision)}
			, hearing{std::move(hearing)}
			, intellect{std::move(intellect)}
			, weight{std::move(weight)}
			, min_temp{std::move(min_temp)}
			, max_temp{std::move(max_temp)}
			, mute{std::move(mute)}
			, protection{std::move(protection)}
			, resistance{std::move(resistance)}
			, vulnerability{std::move(vulnerability)}
		{}

		friend std::ostream& operator <<(std::ostream& out, stats const& a)
		{
			out << a.spirit << ' ' << a.health_regen << ' ' << a.strength << ' ' << a.endurance << ' '
				<< a.stamina << ' ' << a.agility << ' ' << a.stealth << ' ' << a.vision << ' ' << a.hearing << ' '
				<< a.intellect << ' ' << a.weight << ' ' << a.min_temp << ' ' << a.max_temp << ' ' << a.mute << ' ';
			return out;
		}

		friend std::istream& operator >>(std::istream& in, stats& a)
		{
			in >> a.spirit >> a.health_regen >> a.strength >> a.endurance >> a.stamina >> a.agility
				>> a.stealth >> a.vision >> a.hearing >> a.intellect >> a.weight >> a.min_temp >> a.max_temp >> a.mute;
			return in;
		}
	};
}
