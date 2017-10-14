//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.h"

namespace ql
{
	struct message_arrow_miss;
	struct message_cannot_equip;
	struct message_arrow_miss;
	struct message_cannot_equip;
	struct message_entity_in_the_way;
	struct message_incant_failed_mute;
	struct message_incant_gatestone_not_enough_mana;
	struct message_melee_miss;
	struct message_not_enough_ammo;
	struct message_not_enough_mana;

	using message_subtype_list = type_list::of_t
		< message_arrow_miss
		, message_cannot_equip
		, message_arrow_miss
		, message_cannot_equip
		, message_entity_in_the_way
		, message_incant_failed_mute
		, message_incant_gatestone_not_enough_mana
		, message_melee_miss
		, message_not_enough_ammo
		, message_not_enough_mana
		>;

	DEFINE_VISITORS(message, message_subtype_list)

	//! A message that can be sent to an agent.
	struct message : public element<message_subtype_list>
	{
		virtual ~message() = default;
	};

	DEFINE_ELEMENT_BASE(message, message)

	struct message_arrow_miss : message_base<message_arrow_miss> {};
	struct message_cannot_equip : message_base<message_cannot_equip> {};
	struct message_entity_in_the_way : message_base<message_entity_in_the_way> {};
	struct message_incant_failed_mute : message_base<message_incant_failed_mute> {};
	struct message_incant_gatestone_not_enough_mana : message_base<message_incant_gatestone_not_enough_mana>
	{
		double mana_deficit;
		message_incant_gatestone_not_enough_mana(double mana_deficit) : mana_deficit{mana_deficit} {}
	};
	struct message_melee_miss : message_base<message_melee_miss> {};
	struct message_not_enough_ammo : message_base<message_not_enough_ammo> {};
	struct message_not_enough_mana : message_base<message_not_enough_mana>
	{
		double mana_deficit;
		message_not_enough_mana(double mana_deficit) : mana_deficit{mana_deficit} {}
	};
}
