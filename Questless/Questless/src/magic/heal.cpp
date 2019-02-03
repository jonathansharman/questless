//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "magic/heal.hpp"

#include "agents/agent.hpp"
#include "agents/queries/being.hpp"
#include "agents/queries/magnitude.hpp"
#include "agents/queries/message.hpp"
#include "entities/beings/being.hpp"
#include "magic/charge_cost.hpp"

//! @todo double check all the captures to make sure they're the proper type.

namespace ql::magic {
	complete heal::cast(being& caster, gatestone& gatestone, action::cont cont) {
		return caster.agent().query_being(queries::being::heal_target{},
			action::being_in_range_predicate(caster, 10_span),
			[&caster, &gatestone, cont = std::move(cont)](std::optional<being*> opt_target) {
				if (!opt_target) { return cont(action::result::aborted); }
				being* target = *opt_target;
				return caster.agent().query_magnitude( //
					queries::magnitude::heal{},
					100.0,
					0.0,
					std::nullopt,
					[&caster, &gatestone, cont = std::move(cont), target](std::optional<double> opt_magnitude) {
						if (!opt_magnitude) { return cont(action::result::aborted); }
						health healing{*opt_magnitude};
						constexpr auto cost_factor = 1.0_mp / 1.0_hp / 1.0_hp;
						return charge_cost{gatestone, healing * healing * cost_factor}.check_and_incur(
							caster, [&caster, cont = std::move(cont), target, healing] {
								//! @todo Part targeting. For now, just heal everything.
								for (body_part& part : target->body.parts()) {
									target->heal(healing, part, caster.id);
								}
								return cont(action::result::success);
							});
					});
			});
	}
}
