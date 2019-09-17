//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "body_part_generator.hpp"

#include "body_part.hpp"

using namespace vecx::literals;

namespace ql::generators {
	enum class side { left, right };

	// Generic body part template:
	/*
		auto ::make(id owner_id) const -> id {
			id id = reg.create();
			auto& part = reg.assign<body_part>(id);

			part.id = id;
			part.owner_id = owner_id;
			part.name = "PART NAME";
			part.tags = {body_part::tag::TAGS};
			part.cond.action = {_ap, _ap};

			part.stats.a.vitality = _hp;
			part.stats.a.spirit = _mp;
			part.stats.a.strength = _str;
			part.stats.a.stamina = _ep;
			part.stats.a.hearing = _hear;
			part.stats.a.speech = _speech;
			part.stats.a.intellect = _int;
			part.stats.a.mass = _mass;
			part.stats.a.undeath = _undeath;
			part.stats.a.vision_sources = {{stats::vision{
				_perception, // acuity
				_lum, // min_illuminance
				_lum, // max_illuminance
				_perception / 1_lum, // darkness_penalty
				_perception / 1_lum // glare_penalty
			}}};

			part.stats.regen_factor = ;
			part.stats.bleeding = _blood_per_tick;
			part.stats.min_temp = _temp;
			part.stats.max_temp = _temp;
			part.stats.armor = {};

			part.layer = 0;
			part.attachments = {{
				owner_id, // owner_id
				std::nullopt, // o_part_id
				sf::Vector2f{}, // offset
				_rad, // rotation
				{} // default_generator
			}};

			return id;
		}
	*/

	namespace human {
		auto make_body_stats() -> stats::body {
			return {};
		}

		auto torso::make(id owner_id) const -> id {
			id id = reg.create();
			auto& part = reg.assign<body_part>(id, id, owner_id);

			part.name = "human torso";
			part.tags = {body_part::tag::torso};
			part.cond.action = {10_ap, 10_ap};

			part.stats.a.vitality = 10_hp;
			part.stats.a.stamina = 100_ep;
			part.stats.a.mass = 50.0_mass;

			part.stats.regen_factor = 5;
			part.stats.bleeding = 0.0_blood_per_tick;
			part.stats.min_temp = -100_temp;
			part.stats.max_temp = 100_temp;

			part.layer = 0;
			part.attachments = {//
				{
					// Head
					owner_id, // owner_id
					std::nullopt, // o_part_id
					{human::head{}} // default_generator
				},
				{
					// Left arm
					owner_id, // owner_id
					std::nullopt, // o_part_id
					{human::left_arm{}} // default_generator
				},
				{
					// Right arm
					owner_id, // owner_id
					std::nullopt, // o_part_id
					{human::right_arm{}} // default_generator
				},
				{
					// Left leg
					owner_id, // owner_id
					std::nullopt, // o_part_id
					{human::left_leg{}} // default_generator
				},
				{
					// Right leg
					owner_id, // owner_id
					std::nullopt, // o_part_id
					{human::right_leg{}} // default_generator
				}};

			return id;
		}

		auto head::make(id owner_id) const -> id {
			id id = reg.create();
			auto& part = reg.assign<body_part>(id, id, owner_id);

			part.name = "human head";
			part.tags = {body_part::tag::head};
			part.cond.action = {10_ap, 10_ap};

			part.stats.a.vitality = 10_hp;
			part.stats.a.spirit = 100_mp;
			part.stats.a.stamina = 100_ep;
			part.stats.a.hearing = 100_hear;
			part.stats.a.speech = 100_speech;
			part.stats.a.intellect = 100_int;
			part.stats.a.mass = 10.0_mass;
			part.stats.a.vision_sources = {{stats::vision{
				100_perception, // acuity
				80_lum, // min_illuminance
				120_lum, // max_illuminance
				2_perception / 1_lum, // darkness_penalty
				1_perception / 1_lum // glare_penalty
			}}};

			part.stats.regen_factor = 5;
			part.stats.min_temp = -100_temp;
			part.stats.max_temp = 100_temp;

			part.layer = 0;

			return id;
		}

		auto make_arm(id owner_id, side side) -> id {
			id id = reg.create();
			auto& part = reg.assign<body_part>(id, id, owner_id);

			part.name = side == side::left ? "human left arm" : "human right arm";
			part.tags = {body_part::tag::arm};
			part.cond.action = {10_ap, 10_ap};

			part.stats.a.vitality = 30_hp;
			part.stats.a.strength = 80_str;
			part.stats.a.stamina = 100_ep;
			part.stats.a.mass = 25.0_mass;

			part.stats.regen_factor = 5;
			part.stats.min_temp = -100_temp;
			part.stats.max_temp = 100_temp;

			part.layer = 0;
			part.attachments = {{
				// Hand
				owner_id, // owner_id
				std::nullopt, // o_part_id
				side == side::left ? generator{human::left_hand{}} : generator{human::right_hand{}} // default_generator
			}};

			return id;
		}

		auto left_arm::make(id owner_id) const -> id {
			return make_arm(owner_id, side::left);
		}

		auto right_arm::make(id owner_id) const -> id {
			return make_arm(owner_id, side::right);
		}

		auto make_hand(id owner_id, side side) -> id {
			id id = reg.create();
			auto& part = reg.assign<body_part>(id, id, owner_id);

			part.name = side == side::left ? "human left hand" : "human right hand";
			part.tags = {body_part::tag::hand};
			part.cond.action = {10_ap, 10_ap};

			part.stats.a.vitality = 15_hp;
			part.stats.a.strength = 20_str;
			part.stats.a.stamina = 100_ep;
			part.stats.a.mass = 10.0_mass;

			part.stats.regen_factor = 5;
			part.stats.min_temp = -100_temp;
			part.stats.max_temp = 100_temp;

			part.layer = 0;

			return id;
		}

		auto left_hand::make(id owner_id) const -> id {
			return make_hand(owner_id, side::left);
		}

		auto right_hand::make(id owner_id) const -> id {
			return make_hand(owner_id, side::right);
		}

		auto make_leg(id owner_id, side side) -> id {
			id id = reg.create();
			auto& part = reg.assign<body_part>(id, id, owner_id);

			part.name = side == side::left ? "human left leg" : "human right leg";
			part.tags = {body_part::tag::leg};
			part.cond.action = {10_ap, 10_ap};

			part.stats.a.vitality = 40_hp;
			part.stats.a.strength = 100_str;
			part.stats.a.stamina = 100_ep;
			part.stats.a.mass = 35.0_mass;

			part.stats.regen_factor = 5;
			part.stats.min_temp = -100_temp;
			part.stats.max_temp = 100_temp;

			part.layer = 0;
			part.attachments = {{
				// Foot
				owner_id, // owner_id
				std::nullopt, // o_part_id
				side == side::left ? generator{human::left_foot{}} : generator{human::right_foot{}} // default_generator
			}};

			return id;
		}

		auto left_leg::make(id owner_id) const -> id {
			return make_leg(owner_id, side::left);
		}

		auto right_leg::make(id owner_id) const -> id {
			return make_leg(owner_id, side::right);
		}

		auto make_foot(id owner_id, side side) -> id {
			id id = reg.create();
			auto& part = reg.assign<body_part>(id, id, owner_id);

			part.name = side == side::left ? "human left foot" : "human right foot";
			part.tags = {body_part::tag::foot};
			part.cond.action = {10_ap, 10_ap};

			part.stats.a.vitality = 15_hp;
			part.stats.a.strength = 20_str;
			part.stats.a.stamina = 100_ep;
			part.stats.a.mass = 15.0_mass;

			part.stats.regen_factor = 5;
			part.stats.min_temp = -100_temp;
			part.stats.max_temp = 100_temp;

			part.layer = 0;

			return id;
		}

		auto left_foot::make(id owner_id) const -> id {
			return make_foot(owner_id, side::left);
		}

		auto right_foot::make(id owner_id) const -> id {
			return make_foot(owner_id, side::right);
		}
	}

	auto generator::make(id owner_id) const -> id {
		return match(value, [owner_id](auto const& value) { //
			return value.make(owner_id);
		});
	}
}
