/**
* @file    HealSpell.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the HealSpell class.
*/

#include "spell/Heal.h"
#include "entities/beings/Being.h"
#include "agents/Agent.h"

/// @todo double check all the captures to make sure they're the proper type.

namespace questless::spell
{
	Complete Heal::perform_cast(Being& caster, Action::cont_t cont)
	{
		return caster.agent().query_being(std::make_unique<BeingQueryHealTarget>(), Action::being_in_range_predicate(caster, _range),
			[this, &caster, cont](std::optional<Being*> opt_target) {
				if (!opt_target) {
					return cont(Action::Result::aborted);
				}
				Being* target = *opt_target;
				return caster.agent().query_magnitude(std::make_unique<MagnitudeQueryHeal>(), 100.0, 0.0, std::nullopt,
					[this, &caster, &cont, target](std::optional<double> opt_magnitude) {
						if (!opt_magnitude) {
							return cont(Action::Result::aborted);
						}
						double magnitude = *opt_magnitude;
						double cost = _cost_factor * magnitude * log2(magnitude + _cost_log);
						if (caster.mana < cost) {
							return caster.agent().send_message(std::make_unique<MessageNotEnoughMana>(cost - caster.mana), [cont] { return cont(Action::Result::aborted); });
						}
						active_cooldown(cooldown());
						discharge();
						caster.mana -= cost;
						double healing = magnitude * caster.stats.magic.white;
						target->heal(healing, nullptr, caster.id);
						return cont(Action::Result::success);
					}
				);
			}
		);
	}
}
