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
	std::string name(spell const& spell) {
		return match(spell, [](auto const& spell) { return spell.name; });
	}

	magic::color spell_color(spell const& spell) {
		return match(spell, [](auto const& spell) { return spell.color; });
	}

	tick cooldown(spell const& spell) {
		return match(spell, [](auto const& spell) { return spell.cooldown; });
	}

	tick base_incant_time(spell const& spell) {
		return match(spell, [](auto const& spell) { return spell.base_incant_time; });
	}

	complete cast(spell const& spell, being& actor, gatestone& gatestone, action::cont cont) {
		return match(spell, [&](auto const& spell) { return spell.cast(actor, gatestone, cont); });
	}

	uptr<action> cast_action(spell const& spell, id<item> gatestone_id) {
		struct cast : action {
			cast(magic::spell spell, id<item> gatestone_id) : _spell{std::move(spell)}, _gatestone_id{gatestone_id} {}

			std::string name() const final {
				return "cast " + magic::name(_spell);
			}

			complete perform(being& actor, cont cont) final {
				if (auto gatestone = the_game().items.ptr_as<ql::gatestone>(_gatestone_id)) {
					return perform_cast(_spell, actor, *gatestone, std::move(cont));
				} else {
					return actor.agent().send_message(
						queries::message::gatestone_missing{}, [cont] { return cont(result::aborted); });
				}
			}

		private:
			magic::spell _spell;
			id<item> _gatestone_id;
		};

		return umake<cast>(std::move(spell), gatestone_id);
	}

	tick incant_time(spell const& spell, being& caster) {
		return cancel::quantity_cast<tick>(
			base_incant_time(spell) / (1.0 + (caster.stats.a.speech.value() / 100.0_speech).value));
	}
}
