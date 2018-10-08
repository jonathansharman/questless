//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "magic/spell.hpp"

#include "agents/agent.hpp"
#include "agents/queries/message.hpp"
#include "entities/beings/being.hpp"
#include "game.hpp"

namespace ql::magic {
	uptr<action> spell::cast(uptr<spell> spell, id<item> gatestone_id) {
		class cast : public action {
		public:
			cast(uptr<magic::spell> spell, id<item> gatestone_id)
				: _spell{std::move(spell)}, _gatestone_id{gatestone_id}
			{}
			std::string name() const final { return "cast " + _spell->name(); }
			complete perform(being& actor, cont cont) final {
				if (auto gatestone = the_game().items.ptr_as<ql::gatestone>(_gatestone_id)) {
					return _spell->perform_cast(actor, *gatestone, std::move(cont));
				} else {
					return actor.agent().send_message(queries::message::gatestone_missing{}, [cont] { return cont(result::aborted); });
				}
			}
		private:
			uptr<magic::spell> _spell;
			id<item> _gatestone_id;
		};

		return umake<cast>(std::move(spell), gatestone_id);
	}
	
	tick spell::incant_time(being& caster) const {
		return meta::quantity_cast<tick>(base_incant_time() / (1.0 + (caster.stats.a.speech.value() / 100.0_speech).value));
	}
}
