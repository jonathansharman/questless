//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines various units and quantities used throughout the game logic.

#pragma once

#include "meta/quantity.hpp"

namespace ql {
	// General

	//! In-game distance, defined as the distance between adjacent tiles.
	using span = meta::quantity<int, meta::unit_t<struct span_tag>>;
	constexpr span operator "" _span(unsigned long long value) { return span{static_cast<int>(value)}; }

	//! Distance between sections.
	using section_span = meta::quantity<int, meta::unit_t<struct section_span_tag>>;
	constexpr section_span operator "" _section_span(unsigned long long value) { return section_span{static_cast<int>(value)}; }

	//! In-game time duration.
	using tick = meta::quantity<int, meta::unit_t<struct tick_tag>>;
	constexpr tick operator "" _tick(unsigned long long value) { return tick{static_cast<int>(value)}; }

	//! In-game mass.
	using load = meta::quantity<double, meta::unit_t<struct load_tag>>;
	constexpr load operator "" _load(long double value) { return load{static_cast<double>(value)}; }

	//! In-game rate.
	using per_tick = meta::inverse_t<tick>;
	constexpr per_tick operator "" _per_tick(unsigned long long value) { return per_tick{static_cast<int>(value)}; }

	//! In-game temperature.
	using temperature = meta::quantity<double, meta::unit_t<struct temperature_tag>>;
	constexpr temperature operator "" _temp(long double value) { return temperature{static_cast<double>(value)}; }

	//! In-game unit of illuminance.
	using lum = meta::quantity<double, meta::unit_t<struct illuminance_tag>>;
	constexpr lum operator "" _lum(long double value) { return lum{static_cast<double>(value)}; }

	// Beings

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

	using speech = meta::quantity<double, meta::unit_t<struct speech_tag>>;
	constexpr speech operator "" _speech(long double value) { return speech{static_cast<double>(value)}; }

	using intellect = meta::quantity<double, meta::unit_t<struct intellect_tag>>;
	constexpr intellect operator "" _int(long double value) { return intellect{static_cast<double>(value)}; }

	using acuity = meta::quantity<double, meta::unit_t<struct acuity_tag>>;
	constexpr acuity operator "" _acuity(long double value) { return acuity{static_cast<double>(value)}; }

	using acuity_per_lum = meta::quotient_t<acuity, lum>;

	using health_per_tick = meta::quantity<double, meta::quotient_t<health::unit, tick::unit>>;
	constexpr health_per_tick operator "" _hp_per_tick(long double value) { return health_per_tick{static_cast<double>(value)}; }

	using vital = meta::quantity<bool, meta::unit_t<struct vital_tag>>;

	using enabled = meta::quantity<bool, meta::unit_t<struct enabled_tag>>;

	using blood = meta::quantity<double, meta::unit_t<struct blood_tag>>;
	constexpr blood operator "" _blood(long double value) { return blood{static_cast<double>(value)}; }

	using blood_per_tick = meta::quantity<double, meta::quotient_t<typename blood::unit, typename tick::unit>>;
	constexpr blood_per_tick operator "" _blood_per_tick(long double value) { return blood_per_tick{static_cast<double>(value)}; }

	using satiety = meta::quantity<double, meta::unit_t<struct satiety_tag>>;
	constexpr satiety operator "" _sat(long double value) { return satiety{static_cast<double>(value)}; }

	using alertness = meta::quantity<double, meta::unit_t<struct alertness_tag>>;
	constexpr alertness operator "" _alert(long double value) { return alertness{static_cast<double>(value)}; }

	using mood = meta::quantity<double, meta::unit_t<struct mood_tag>>;
	constexpr mood operator "" _mood(long double value) { return mood{static_cast<double>(value)}; }

	using awake = meta::quantity<bool, meta::unit_t<struct awake_tag>>;

	using corporeal = meta::quantity<bool, meta::unit_t<struct corporeal_tag>>;

	// Items

	using integrity = meta::quantity<double, meta::unit_t<struct integrity_tag>>;
	constexpr integrity operator "" _integrity(long double value) { return integrity{static_cast<double>(value)}; }
}
