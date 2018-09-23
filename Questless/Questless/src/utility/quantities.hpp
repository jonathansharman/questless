//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines various units and quantities used throughout the game logic.

#pragma once

#include "meta/quantity.hpp"

namespace ql {
	// General quantities

	//! In-game time duration.
	using ticks = meta::quantity<double, meta::unit_t<struct tick_tag>>;
	constexpr ticks operator "" _tick(long double value) { return ticks{static_cast<double>(value)}; }

	//! In-game rate.
	using per_tick = decltype(1.0 / 1.0_tick);
	constexpr per_tick operator "" _per_tick(long double value) { return per_tick{static_cast<double>(value)}; }

	//! In-game temperature.
	using temperature = meta::quantity<double, meta::unit_t<struct temperature_tag>>;
	constexpr temperature operator "" _temp(long double value) { return temperature{static_cast<double>(value)}; }

	// Being quantities

	using health = meta::quantity<double, meta::unit_t<struct health_tag>>;
	constexpr health operator "" _hp(long double value) { return health{static_cast<double>(value)}; }

	using mana = meta::quantity<double, meta::unit_t<struct mana_tag>>;
	constexpr mana operator "" _mp(long double value) { return mana{static_cast<double>(value)}; }

	using strength = meta::quantity<double, meta::unit_t<struct strength_tag>>;
	constexpr strength operator "" _str(long double value) { return strength{static_cast<double>(value)}; }

	using toughness = meta::quantity<double, meta::unit_t<struct toughness_tag>>;
	constexpr toughness operator "" _tgh(long double value) { return toughness{static_cast<double>(value)}; }

	using energy = meta::quantity<double, meta::unit_t<struct energy_tag>>;
	constexpr energy operator "" _ep(long double value) { return energy{static_cast<double>(value)}; }

	using agility = meta::quantity<double, meta::unit_t<struct agility_tag>>;
	constexpr agility operator "" _agi(long double value) { return agility{static_cast<double>(value)}; }

	using dexterity = meta::quantity<double, meta::unit_t<struct dexterity_tag>>;
	constexpr dexterity operator "" _dex(long double value) { return dexterity{static_cast<double>(value)}; }

	using stealth = meta::quantity<double, meta::unit_t<struct stealth_tag>>;
	constexpr stealth operator "" _stl(long double value) { return stealth{static_cast<double>(value)}; }

	using hearing = meta::quantity<double, meta::unit_t<struct hearing_tag>>;
	constexpr hearing operator "" _hear(long double value) { return hearing{static_cast<double>(value)}; }

	using intellect = meta::quantity<double, meta::unit_t<struct intellect_tag>>;
	constexpr intellect operator "" _int(long double value) { return intellect{static_cast<double>(value)}; }

	using acuity = meta::quantity<double, meta::unit_t<struct acuity_tag>>;
	constexpr acuity operator "" _acuity(long double value) { return acuity{static_cast<double>(value)}; }

	using illuminance = meta::quantity<double, meta::unit_t<struct illuminance_tag>>;
	constexpr illuminance operator "" _illuminance(long double value) { return illuminance{static_cast<double>(value)}; }

	using darkness_tolerance = meta::quantity<double, meta::unit_t<struct darkness_tolerance_tag>>;
	constexpr darkness_tolerance operator "" _darkness_tolerance(long double value) { return darkness_tolerance{static_cast<double>(value)}; }

	using glare_tolerance = meta::quantity<double, meta::unit_t<struct glare_tolerance_tag>>;
	constexpr glare_tolerance operator "" _glare_tolerance(long double value) { return glare_tolerance{static_cast<double>(value)}; }

	using weight = meta::quantity<double, meta::unit_t<struct weight_tag>>;
	constexpr weight operator "" _wt(long double value) { return weight{static_cast<double>(value)}; }

	using mute = meta::quantity<bool, meta::unit_t<struct mute_tag>>;

	using health_per_tick = meta::quantity<double, meta::quotient_t<health::unit, ticks::unit>>;
	constexpr health_per_tick operator "" _hp_per_tick(long double value) { return health_per_tick{static_cast<double>(value)}; }

	using vital = meta::quantity<bool, meta::unit_t<struct vital_tag>>;

	using enabled = meta::quantity<bool, meta::unit_t<struct enabled_tag>>;

	using blood = meta::quantity<double, meta::unit_t<struct blood_tag>>;
	constexpr blood operator "" _blood(long double value) { return blood{static_cast<double>(value)}; }

	using blood_per_tick = meta::quantity<double, meta::quotient_t<typename blood::unit, typename ticks::unit>>;
	constexpr blood_per_tick operator "" _blood_per_tick(long double value) { return blood_per_tick{static_cast<double>(value)}; }

	using satiety = meta::quantity<double, meta::unit_t<struct satiety_tag>>;
	constexpr satiety operator "" _sat(long double value) { return satiety{static_cast<double>(value)}; }

	using alertness = meta::quantity<double, meta::unit_t<struct alertness_tag>>;
	constexpr alertness operator "" _alert(long double value) { return alertness{static_cast<double>(value)}; }

	using mood = meta::quantity<double, meta::unit_t<struct mood_tag>>;
	constexpr mood operator "" _mood(long double value) { return mood{static_cast<double>(value)}; }

	using corporeal = meta::quantity<bool, meta::unit_t<struct corporeal_tag>>;
}
