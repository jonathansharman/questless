//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/body_part.hpp"

#include "entities/beings/being.hpp"
#include "game.hpp"

#include "utility/io.hpp"

using namespace units;

namespace ql {
	body_part::body_part(char const* filepath, ql::id<being> owner_id, ql::id<body_part> id)
		: id{id}, owner_id{owner_id}
	{
		from_json(nlohmann::json::parse(contents_of_file(filepath)), *this);
	}
	
	void body_part::update() {
		being& owner = the_game().beings.ref(owner_id);

		health += owner.stats.regen.value() * stats.a.vitality.value() * 1.0_tick;

		bleeding -= owner.stats.regen.value() * stats.a.vitality.value() * blood_per_vitality;
	}

	void body_part::take_damage(dmg::group& damage, std::optional<ql::id<being>> source_id) {
		if (being* owner = the_game().beings.ptr(owner_id)) {
			owner->take_damage(damage, *this, source_id);
		}
	}

	void body_part::heal(ql::health amount, std::optional<ql::id<being>> opt_source_id) {
		being& owner = the_game().beings.ref(owner_id);
		owner.heal(amount, *this, opt_source_id);
	}

	void body_part::generate_attached_parts() {
		for (auto& attachment : attachments) {
			attachment->part = attachment->make_default();
			attachment->part->generate_attached_parts();
		}
	}

	void to_json(nlohmann::json& j, body_part::tag const& tag) {
		switch (tag) {
			case body_part::tag::head: j = "head"; break;
			case body_part::tag::torso: j = "torso"; break;
			case body_part::tag::arm: j = "arm"; break;
			case body_part::tag::hand: j = "hand"; break;
			case body_part::tag::leg: j = "leg"; break;
			case body_part::tag::foot: j = "foot"; break;
			case body_part::tag::wing: j = "wing"; break;
			case body_part::tag::tail: j = "tail"; break;
		}
	}

	void from_json(nlohmann::json const& j, body_part::tag& tag) {
		std::string tag_name = j;
		if (tag_name == "head") {
			tag = body_part::tag::head;
		} else if (tag_name == "torso") {
			tag = body_part::tag::torso;
		} else if (tag_name == "arm") {
			tag = body_part::tag::arm;
		} else if (tag_name == "hand") {
			tag = body_part::tag::hand;
		} else if (tag_name == "leg") {
			tag = body_part::tag::leg;
		} else if (tag_name == "foot") {
			tag = body_part::tag::foot;
		} else if (tag_name == "wing") {
			tag = body_part::tag::wing;
		} else if (tag_name == "tail") {
			tag = body_part::tag::tail;
		} else {
			assert(false && "Unknown body part tag.");
		}
	}

	void to_json(nlohmann::json& j, body_part const& part) {
		j["name"] = part.name;
		nlohmann::adl_serializer<std::vector<body_part::tag>>::to_json(j["tags"], part.tags);
		to_json(j["stats"], part.stats);
		to_json(j["vital"], part.vital);
		to_json(j["enabled"], part.enabled);
		j["layer"] = part.layer;
		nlohmann::adl_serializer<view_space::polygon>::to_json(j["hitbox"], part.hitbox);
		nlohmann::adl_serializer<std::optional<id<item>>>::to_json(j["equipped_item_id"], part.equipped_item_id);
		nlohmann::adl_serializer<std::vector<ql::attachment>>::to_json(j["attachments"], part.attachments);
	}

	void from_json(nlohmann::json const& j, body_part& part) {
		part.name = j["name"];
		nlohmann::adl_serializer<std::vector<body_part::tag>>::from_json(j["tags"], part.tags);
		from_json(j["stats"], part.stats);
		from_json(j["vital"], part.vital);
		from_json(j["enabled"], part.enabled);
		part.layer = j["layer"];
		nlohmann::adl_serializer<std::vector<view_space::point>>::from_json(j["hitbox"], part.hitbox);
		nlohmann::adl_serializer<std::optional<id<item>>>::from_json(j["equipped_item_id"], part.equipped_item_id);
		//! @todo attachments
		//nlohmann::adl_serializer<std::vector<view_space::point>>::from_json(j["attachments"], part.attachments);
	}

	void to_json(nlohmann::json& j, attachment const& attachment) {
		nlohmann::adl_serializer<std::optional<body_part>>::to_json(j, attachment.part);
		nlohmann::adl_serializer<view_space::vector>::to_json(j["offset"], attachment.offset);
		nlohmann::adl_serializer<view_space::radians>::to_json(j["rotation"], attachment.rotation);
		j["default_part"] = attachment.default_part;
	}

	void from_json(nlohmann::json const& j, attachment& attachment) {
		nlohmann::adl_serializer<std::optional<body_part>>::from_json(j["part"], attachment.part);
		nlohmann::adl_serializer<ql::attachment>::from_json(j["offset"], attachment.offset);
		nlohmann::adl_serializer<view_space::radians>::from_json(j["rotation"], attachment.rotation);
		nlohmann::adl_serializer<body_part>::from_json(j["default_part"], attachment.default_part);
	}
}
