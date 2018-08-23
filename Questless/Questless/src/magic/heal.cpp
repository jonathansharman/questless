//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "magic/heal.hpp"

#include "agents/agent.hpp"
#include "agents/queries/being_query.hpp"
#include "agents/queries/magnitude_query.hpp"
#include "agents/queries/message.hpp"
#include "entities/beings/being.hpp"
#include "magic/charge_cost.hpp"

//! @todo double check all the captures to make sure they're the proper type.

namespace ql::magic {
	complete heal::perform_cast(being& caster, gatestone& gatestone, action::cont cont) {
		return caster.agent().query_being(umake<being_query_heal_target>(), action::being_in_range_predicate(caster, _range),
			[&caster, &gatestone, cont = std::move(cont)](std::optional<being*> opt_target) {
				if (!opt_target) {
					return cont(action::result::aborted);
				}
				being* target = *opt_target;
				return caster.agent().query_magnitude(umake<magnitude_query_heal>(), 100.0, 0.0, std::nullopt,
					[&caster, &gatestone, cont = std::move(cont), target](std::optional<double> opt_magnitude) {
						if (!opt_magnitude) {
							return cont(action::result::aborted);
						}
						double magnitude = *opt_magnitude;
						return charge_cost{gatestone, _cost_factor * magnitude * log2(magnitude + _cost_log)}.check_and_incur(caster,
							[&caster, cont = std::move(cont), target, magnitude] {
								//! @todo Part targeting. For now, just heal everything.
								for (body_part& part : target->body.parts()) {
									target->heal(magnitude, part, caster.id);
								}
								return cont(action::result::success);
							}
						);
					}
				);
			}
		);
	}
}
