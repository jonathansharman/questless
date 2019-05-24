//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "spell.hpp"

#include "agents/agent.hpp"
#include "agents/queries/message.hpp"
#include "entities/beings/being.hpp"
#include "game.hpp"
#include "utility/visitation.hpp"

namespace ql::magic {
	std::string spell::name() const {
		return match(value, [](auto const& spell) { return spell.name; });
	}

	magic::color spell::color() const {
		return match(value, [](auto const& spell) { return spell.color; });
	}

	tick spell::cooldown() const {
		return match(value, [](auto const& spell) { return spell.cooldown; });
	}

	tick spell::base_incant_time() const {
		return match(value, [](auto const& spell) { return spell.base_incant_time; });
	}

	complete spell::cast(being& actor, gatestone& gatestone, action::cont cont) const {
		return match(value, [&](auto const& spell) { return spell.cast(actor, gatestone, cont); });
	}

	uptr<action> spell::cast_action(ent gatestone_id) const {
		struct cast : action {
			cast(magic::spell spell, ent gatestone_id) : _spell{std::move(spell)}, _gatestone_id{gatestone_id} {}

			std::string name() const final {
				return "cast " + _spell.name();
			}

			complete perform(being& actor, cont cont) final {
				if (reg.valid(_gatestone_id)) {
					return _spell.cast(actor, reg.get<gatestone>(_gatestone_id), std::move(cont));
				} else {
					return actor.agent().send_message(
						queries::message::gatestone_missing{}, [cont] { return cont(result::aborted); });
				}
			}

		private:
			magic::spell _spell;
			ent _gatestone_id;
		};

		return umake<cast>(*this, gatestone_id);
	}

	tick spell::incant_time(being const& caster) const {
		return cancel::quantity_cast<tick>(base_incant_time() / (1.0 + (caster.stats.a.speech.value() / 100.0_speech).value));
	}
}
