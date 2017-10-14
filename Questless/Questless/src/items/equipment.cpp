//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/equipment.h"

#include "game.h"
#include "entities/beings/being.h"

namespace ql
{
	complete equipment::equip::perform(being& actor, cont cont)
	{
		//! @todo Allow bearer to choose where to equip item.

		//! @todo Report failure to equip.

		auto fail = [&] {
			return actor.agent().send_message(std::make_unique<message_cannot_equip>(), [cont] { return cont(result::aborted); });
		};

		_equipment._bearer_id = actor.id;

		body_part_counts reqs = _equipment.requirements();

		// Equip to heads.
		for (int i = 0; i < reqs.heads(); ++i) {
			bool found_head = false;
			for (head& head : actor.body.heads()) {
				if (!head.equipped_item_id) {
					head.equipped_item_id = _equipment.id;
					_equipment._head_ids.push_back(head.id);
					found_head = true;
					break;
				}
			}
			if (!found_head) {
				_equipment.unequip();
				return fail();
			}
		}

		// Equip to torsos.
		for (int i = 0; i < reqs.torsos(); ++i) {
			bool found_torso = false;
			for (torso& torso : actor.body.torsos()) {
				if (!torso.equipped_item_id) {
					torso.equipped_item_id = _equipment.id;
					_equipment._torso_ids.push_back(torso.id);
					found_torso = true;
					break;
				}
			}
			if (!found_torso) {
				_equipment.unequip();
				return fail();
			}
		}

		// Equip to arms.
		for (int i = 0; i < reqs.arms(); ++i) {
			bool found_arm = false;
			for (arm& arm : actor.body.arms()) {
				if (!arm.equipped_item_id) {
					arm.equipped_item_id = _equipment.id;
					_equipment._arm_ids.push_back(arm.id);
					found_arm = true;
					break;
				}
			}
			if (!found_arm) {
				_equipment.unequip();
				return fail();
			}
		}

		// Equip to hands.
		for (int i = 0; i < reqs.hands(); ++i) {
			bool found_hand = false;
			for (hand& hand : actor.body.hands()) {
				if (!hand.equipped_item_id) {
					hand.equipped_item_id = _equipment.id;
					_equipment._hand_ids.push_back(hand.id);
					found_hand = true;
					break;
				}
			}
			if (!found_hand) {
				_equipment.unequip();
				return fail();
			}
		}

		// Equip to legs.
		for (int i = 0; i < reqs.legs(); ++i) {
			bool found_leg = false;
			for (leg& leg : actor.body.legs()) {
				if (!leg.equipped_item_id) {
					leg.equipped_item_id = _equipment.id;
					_equipment._leg_ids.push_back(leg.id);
					found_leg = true;
					break;
				}
			}
			if (!found_leg) {
				_equipment.unequip();
				return fail();
			}
		}

		// Equip to feet.
		for (int i = 0; i < reqs.feet(); ++i) {
			bool found_foot = false;
			for (foot& foot : actor.body.feet()) {
				if (!foot.equipped_item_id) {
					foot.equipped_item_id = _equipment.id;
					_equipment._foot_ids.push_back(foot.id);
					found_foot = true;
					break;
				}
			}
			if (!found_foot) {
				_equipment.unequip();
				return fail();
			}
		}

		// Equip to wings.
		for (int i = 0; i < reqs.wings(); ++i) {
			bool found_wing = false;
			for (wing& wing : actor.body.wings()) {
				if (!wing.equipped_item_id) {
					wing.equipped_item_id = _equipment.id;
					_equipment._wing_ids.push_back(wing.id);
					found_wing = true;
					break;
				}
			}
			if (!found_wing) {
				_equipment.unequip();
				return fail();
			}
		}

		// Equip to tails.
		for (int i = 0; i < reqs.tails(); ++i) {
			bool found_tail = false;
			for (tail& tail : actor.body.tails()) {
				if (!tail.equipped_item_id) {
					tail.equipped_item_id = _equipment.id;
					_equipment._tail_ids.push_back(tail.id);
					found_tail = true;
					break;
				}
			}
			if (!found_tail) {
				_equipment.unequip();
				return fail();
			}
		}
		
		actor.busy_time += _equipment.equip_time();

		return cont(result::success);
	}

	complete equipment::unequip::perform(being& actor, cont cont)
	{
		actor.busy_time += _equipment.unequip_time();

		_equipment.unequip();

		return cont(result::success);
	}

	void equipment::unequip()
	{
		if (_bearer_id) {
			if (being* bearer = the_game().beings.ptr(*_bearer_id)) {
				for (auto head_id : _head_ids) {
					if (head* head = bearer->body.find_head(head_id)) {
						head->equipped_item_id = std::nullopt;
					}
				}
				for (auto torso_id : _torso_ids) {
					if (torso* torso = bearer->body.find_torso(torso_id)) {
						torso->equipped_item_id = std::nullopt;
					}
				}
				for (auto arm_id : _arm_ids) {
					if (arm* arm = bearer->body.find_arm(arm_id)) {
						arm->equipped_item_id = std::nullopt;
					}
				}
				for (auto hand_id : _hand_ids) {
					if (hand* hand = bearer->body.find_hand(hand_id)) {
						hand->equipped_item_id = std::nullopt;
					}
				}
				for (auto leg_id : _leg_ids) {
					if (leg* leg = bearer->body.find_leg(leg_id)) {
						leg->equipped_item_id = std::nullopt;
					}
				}
				for (auto foot_id : _foot_ids) {
					if (foot* foot = bearer->body.find_foot(foot_id)) {
						foot->equipped_item_id = std::nullopt;
					}
				}
				for (auto wing_id : _wing_ids) {
					if (wing* wing = bearer->body.find_wing(wing_id)) {
						wing->equipped_item_id = std::nullopt;
					}
				}
				for (auto tail_id : _tail_ids) {
					if (tail* tail = bearer->body.find_tail(tail_id)) {
						tail->equipped_item_id = std::nullopt;
					}
				}
			}

			_bearer_id = std::nullopt;
			_head_ids.clear();
			_torso_ids.clear();
			_arm_ids.clear();
			_hand_ids.clear();
			_leg_ids.clear();
			_foot_ids.clear();
			_wing_ids.clear();
			_tail_ids.clear();
		}
	}
}
