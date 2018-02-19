//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "magic/heal.hpp"
#include "entities/beings/being.hpp"
#include "agents/agent.hpp"

//! @todo double check all the captures to make sure they're the proper type.

namespace ql::magic
{
	complete heal::perform_cast(being& caster, action::cont cont)
	{
		return caster.agent().query_being(umake<being_query_heal_target>(), action::being_in_range_predicate(caster, _range),
			[this, &caster, cont](std::optional<being*> opt_target) {
				if (!opt_target) {
					return cont(action::result::aborted);
				}
				being* target = *opt_target;
				return caster.agent().query_magnitude(umake<magnitude_query_heal>(), 100.0, 0.0, std::nullopt,
					[this, &caster, &cont, target](std::optional<double> opt_magnitude) {
						if (!opt_magnitude) {
							return cont(action::result::aborted);
						}
						double magnitude = *opt_magnitude;
						double cost = _cost_factor * magnitude * log2(magnitude + _cost_log);
						if (caster.mana < cost) {
							return caster.agent().send_message(umake<message_not_enough_mana>(cost - caster.mana), [cont] { return cont(action::result::aborted); });
						}
						caster.mana -= cost;
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
}
