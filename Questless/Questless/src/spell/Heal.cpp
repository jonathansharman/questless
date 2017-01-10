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
	Action::Complete Heal::perform_cast(Being& caster, Action::cont_t cont)
	{
		return caster.agent().query_being("Heal Target", "Select a being to be healed.", Action::being_in_range_predicate(caster, _range),
			[this, &caster, cont](boost::optional<Being*> opt_target) {
				if (!opt_target) {
					return cont(Action::Result::aborted);
				}
				Being* target = *opt_target;
				return caster.agent().query_magnitude("Heal Amount", "Choose how much health to restore.", 100.0, [](double amount) { return amount >= 0.0; },
					[this, &caster, &cont, target](boost::optional<double> opt_magnitude) {
						if (!opt_magnitude) {
							return cont(Action::Result::aborted);
						}
						double magnitude = *opt_magnitude;
						double cost = _cost_factor * magnitude * log2(magnitude + _cost_log);
						if (caster.mana() < cost) {
							return caster.agent().message("Not enough mana!", "You need " + std::to_string(cost - caster.mana()) + " more mana to cast this.", [cont] { return cont(Action::Result::aborted); });
						}
						active_cooldown(cooldown());
						discharge();
						caster.lose_mana(cost);
						double healing = magnitude * caster.magic_power<Color::white>();
						target->heal(healing, nullptr, caster.id());
						return cont(Action::Result::success);
					}
				);
			}
		);
	}
}
