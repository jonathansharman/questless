//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.hpp"

namespace ql {
	#define MESSAGE_FIRST \
		X(message_arrow_miss)
	#define MESSAGE_REST \
		X(message_cannot_equip) \
		X(message_arrow_miss) \
		X(message_cannot_equip) \
		X(message_entity_in_the_way) \
		X(message_incant_failed_mute) \
		X(message_melee_miss) \
		X(message_not_enough_ammo) \
		X(message_not_enough_charge)

	#define X(subtype) struct subtype;
	MESSAGE_FIRST
	MESSAGE_REST
	#undef X

	using message_subtype_list = type_list::of_t
		#define X(subtype) < subtype
		MESSAGE_FIRST
		#undef X
		#define X(subtype) , subtype
		MESSAGE_REST
		>;
		#undef X

	DEFINE_VISITORS(message, message_subtype_list)

	//! A message that can be sent to an agent.
	struct message : public element<message_subtype_list> {
		virtual ~message() = default;
	};

	DEFINE_ELEMENT_BASE(message, message)

	struct message_arrow_miss : message_base<message_arrow_miss> {};
	struct message_cannot_equip : message_base<message_cannot_equip> {};
	struct message_entity_in_the_way : message_base<message_entity_in_the_way> {};
	struct message_incant_failed_mute : message_base<message_incant_failed_mute> {};
	struct message_melee_miss : message_base<message_melee_miss> {};
	struct message_not_enough_ammo : message_base<message_not_enough_ammo> {};
	struct message_not_enough_charge : message_base<message_not_enough_charge> {
		double charge_deficit;
		message_not_enough_charge(double charge_deficit) : charge_deficit{charge_deficit} {}
	};
}
