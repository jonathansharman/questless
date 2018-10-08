//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/equipment.hpp"

#include "agents/agent.hpp"
#include "agents/queries/message.hpp"
#include "entities/beings/being.hpp"
#include "game.hpp"

namespace ql {
	complete equipment::equip::perform(being& actor, cont cont) {
		//! @todo Allow bearer to choose where to equip item.

		//! @todo Report failure to equip.

		auto fail = [&] {
			return actor.agent().send_message(queries::message::cannot_equip{}, [cont] { return cont(result::aborted); });
		};

		_equipment._opt_bearer_id = actor.id;

		body_part_counts reqs = _equipment.requirements();

		// Equip to heads.
		for (int i = 0; i < reqs.heads.value; ++i) {
			bool found_head = false;
			for (body_part& part : actor.body.parts()) {
				if (part.tags.count(body_part::tag::head) && !part.equipped_item_id) {
					part.equipped_item_id = _equipment.id;
					_equipment._head_ids.push_back(part.id);
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
		for (int i = 0; i < reqs.torsos.value; ++i) {
			bool found_torso = false;
			for (body_part& part : actor.body.parts()) {
				if (part.tags.count(body_part::tag::torso) && !part.equipped_item_id) {
					part.equipped_item_id = _equipment.id;
					_equipment._torso_ids.push_back(part.id);
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
		for (int i = 0; i < reqs.arms.value; ++i) {
			bool found_arm = false;
			for (body_part& part : actor.body.parts()) {
				if (part.tags.count(body_part::tag::arm) && !part.equipped_item_id) {
					part.equipped_item_id = _equipment.id;
					_equipment._arm_ids.push_back(part.id);
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
		for (int i = 0; i < reqs.hands.value; ++i) {
			bool found_hand = false;
			for (body_part& part : actor.body.parts()) {
				if (part.tags.count(body_part::tag::head) && !part.equipped_item_id) {
					part.equipped_item_id = _equipment.id;
					_equipment._hand_ids.push_back(part.id);
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
		for (int i = 0; i < reqs.legs.value; ++i) {
			bool found_leg = false;
			for (body_part& part : actor.body.parts()) {
				if (part.tags.count(body_part::tag::leg) && !part.equipped_item_id) {
					part.equipped_item_id = _equipment.id;
					_equipment._leg_ids.push_back(part.id);
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
		for (int i = 0; i < reqs.feet.value; ++i) {
			bool found_foot = false;
			for (body_part& part : actor.body.parts()) {
				if (part.tags.count(body_part::tag::foot) && !part.equipped_item_id) {
					part.equipped_item_id = _equipment.id;
					_equipment._foot_ids.push_back(part.id);
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
		for (int i = 0; i < reqs.wings.value; ++i) {
			bool found_wing = false;
			for (body_part& part : actor.body.parts()) {
				if (!part.equipped_item_id) {
					part.equipped_item_id = _equipment.id;
					_equipment._wing_ids.push_back(part.id);
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
		for (int i = 0; i < reqs.tails.value; ++i) {
			bool found_tail = false;
			for (body_part& part : actor.body.parts()) {
				if (part.tags.count(body_part::tag::tail) && !part.equipped_item_id) {
					part.equipped_item_id = _equipment.id;
					_equipment._tail_ids.push_back(part.id);
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

	complete equipment::unequip::perform(being& actor, cont cont) {
		actor.busy_time += _equipment.unequip_time();

		_equipment.unequip();

		return cont(result::success);
	}

	void equipment::unequip() {
		if (_opt_bearer_id) {
			if (being* bearer = the_game().beings.ptr(*_opt_bearer_id)) {
				for (auto head_id : _head_ids) {
					if (body_part* head = bearer->body.find_part(head_id)) {
						head->equipped_item_id = std::nullopt;
					}
				}
				for (auto torso_id : _torso_ids) {
					if (body_part* torso = bearer->body.find_part(torso_id)) {
						torso->equipped_item_id = std::nullopt;
					}
				}
				for (auto arm_id : _arm_ids) {
					if (body_part* arm = bearer->body.find_part(arm_id)) {
						arm->equipped_item_id = std::nullopt;
					}
				}
				for (auto hand_id : _hand_ids) {
					if (body_part* hand = bearer->body.find_part(hand_id)) {
						hand->equipped_item_id = std::nullopt;
					}
				}
				for (auto leg_id : _leg_ids) {
					if (body_part* leg = bearer->body.find_part(leg_id)) {
						leg->equipped_item_id = std::nullopt;
					}
				}
				for (auto foot_id : _foot_ids) {
					if (body_part* foot = bearer->body.find_part(foot_id)) {
						foot->equipped_item_id = std::nullopt;
					}
				}
				for (auto wing_id : _wing_ids) {
					if (body_part* wing = bearer->body.find_part(wing_id)) {
						wing->equipped_item_id = std::nullopt;
					}
				}
				for (auto tail_id : _tail_ids) {
					if (body_part* tail = bearer->body.find_part(tail_id)) {
						tail->equipped_item_id = std::nullopt;
					}
				}
			}

			_opt_bearer_id = std::nullopt;
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
