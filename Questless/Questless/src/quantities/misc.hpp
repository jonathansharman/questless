//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines various units and quantities used throughout the game logic.

#pragma once

#include "game_time.hpp"

#include "cancel/quantity.hpp"

#include "game_time.hpp"

namespace ql {
	// General

	//! In-game temperature.
	using temperature = cancel::quantity<int, cancel::unit_t<struct temperature_tag>>;
	constexpr auto operator"" _temp(unsigned long long value) {
		return temperature{static_cast<int>(value)};
	}

	//! In-game unit of illuminance.
	using lum = cancel::quantity<int, cancel::unit_t<struct illuminance_tag>>;
	constexpr auto operator"" _lum(unsigned long long value) {
		return lum{static_cast<int>(value)};
	}

	// Beings

	using health = cancel::quantity<int, cancel::unit_t<struct health_tag>>;
	constexpr auto operator"" _hp(unsigned long long value) {
		return health{static_cast<int>(value)};
	}

	using mana = cancel::quantity<int, cancel::unit_t<struct mana_tag>>;
	constexpr auto operator"" _mp(unsigned long long value) {
		return mana{static_cast<int>(value)};
	}

	using strength = cancel::quantity<int, cancel::unit_t<struct strength_tag>>;
	constexpr auto operator"" _str(unsigned long long value) {
		return strength{static_cast<int>(value)};
	}

	using energy = cancel::quantity<int, cancel::unit_t<struct energy_tag>>;
	constexpr auto operator"" _ep(unsigned long long value) {
		return energy{static_cast<int>(value)};
	}

	using action = cancel::quantity<int, cancel::unit_t<struct action_tag>>;
	constexpr auto operator"" _ap(unsigned long long value) {
		return action{static_cast<int>(value)};
	}

	using hearing = cancel::quantity<int, cancel::unit_t<struct hearing_tag>>;
	constexpr auto operator"" _hear(unsigned long long value) {
		return hearing{static_cast<int>(value)};
	}

	using speech = cancel::quantity<int, cancel::unit_t<struct speech_tag>>;
	constexpr auto operator"" _speech(unsigned long long value) {
		return speech{static_cast<int>(value)};
	}

	using intellect = cancel::quantity<int, cancel::unit_t<struct intellect_tag>>;
	constexpr auto operator"" _int(unsigned long long value) {
		return intellect{static_cast<int>(value)};
	}

	using perception = cancel::quantity<int, cancel::unit_t<struct perception_tag>>;
	constexpr auto operator"" _perception(unsigned long long value) {
		return perception{static_cast<int>(value)};
	}

	using perception_per_lum = cancel::quotient_t<perception, lum>;

	enum class ableness { enabled, disabled };

	using blood = cancel::quantity<double, cancel::unit_t<struct blood_tag>>;
	constexpr auto operator"" _blood(long double value) {
		return blood{static_cast<double>(value)};
	}

	using blood_per_tick = cancel::quotient_t<blood, tick>;
	constexpr auto operator"" _blood_per_tick(long double value) {
		return blood_per_tick{static_cast<double>(value)};
	}

	using poisoning = cancel::quantity<int, cancel::unit_t<struct poisoning_tag>>;
	constexpr auto operator"" _poisoning(unsigned long long value) {
		return poisoning{static_cast<int>(value)};
	}

	using decay = cancel::quantity<int, cancel::unit_t<struct decay_tag>>;
	constexpr auto operator"" _decay(unsigned long long value) {
		return decay{static_cast<int>(value)};
	}

	using hypovolemia_damage = cancel::quantity<int, cancel::unit_t<struct hypovolemia_damage_tag>>;
	constexpr auto operator"" _hypovolemia_damage(unsigned long long value) {
		return hypovolemia_damage{static_cast<int>(value)};
	}

	using undeath = cancel::quantity<int, cancel::unit_t<struct undeath_tag>>;
	constexpr auto operator"" _undeath(unsigned long long value) {
		return undeath{static_cast<int>(value)};
	}

	using satiety = cancel::quantity<double, cancel::unit_t<struct satiety_tag>>;
	constexpr auto operator"" _sat(long double value) {
		return satiety{static_cast<double>(value)};
	}

	using alertness = cancel::quantity<double, cancel::unit_t<struct alertness_tag>>;
	constexpr auto operator"" _alert(long double value) {
		return alertness{static_cast<double>(value)};
	}

	using joy = cancel::quantity<double, cancel::unit_t<struct joy_tag>>;
	constexpr auto operator"" _joy(long double value) {
		return joy{static_cast<double>(value)};
	}

	using courage = cancel::quantity<double, cancel::unit_t<struct courage_tag>>;
	constexpr auto operator"" _courage(long double value) {
		return courage{static_cast<double>(value)};
	}

	using serenity = cancel::quantity<double, cancel::unit_t<struct serenity_tag>>;
	constexpr auto operator"" _serenity(long double value) {
		return serenity{static_cast<double>(value)};
	}

	enum class awakeness { awake, asleep };

	// Items

	using durability = cancel::quantity<int, cancel::unit_t<struct integrity_tag>>;
	constexpr auto operator"" _durability(unsigned long long value) {
		return durability{static_cast<int>(value)};
	}
}
