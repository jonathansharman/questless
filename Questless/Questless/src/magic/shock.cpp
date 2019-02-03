//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "magic/shock.hpp"

#include "agents/agent.hpp"
#include "agents/queries/magnitude.hpp"
#include "agents/queries/message.hpp"
#include "agents/queries/tile.hpp"
#include "effects/effect.hpp"
#include "entities/beings/being.hpp"
#include "game.hpp"
#include "magic/charge_cost.hpp"
#include "utility/random.hpp"

//! @todo Body part stuff - remove if not used.
#include "entities/beings/body_part.hpp"
#include <deque>
#include <type_traits>

namespace ql::magic {
	complete shock::cast(being& caster, gatestone& gatestone, action::cont cont) {
		return caster.agent().query_tile(queries::tile::shock_target{},
			caster.coords,
			action::tile_in_range_predicate(caster, 3_span),
			[&caster, &gatestone, cont = std::move(cont)](std::optional<region_tile::point> opt_tile_coords) {
				if (!opt_tile_coords) { return cont(action::result::aborted); }
				region_tile::point tile_coords = *opt_tile_coords;
				return caster.agent().query_magnitude(queries::magnitude::shock{},
					20.0,
					0.0,
					std::nullopt,
					[&caster, &gatestone, cont = std::move(cont), tile_coords](std::optional<double> opt_magnitude) {
						if (!opt_magnitude) { return cont(action::result::aborted); }
						dmg::shock damage{*opt_magnitude};
						constexpr auto cost_factor = 0.2_mp / 1.0_shock / 1.0_shock;
						return charge_cost{gatestone, cost_factor * damage * damage}.check_and_incur(
							caster, [&caster, &gatestone, cont = std::move(cont), tile_coords, damage] {
								return caster.agent().get_shock_quality(tile_coords,
									[&caster, &gatestone, cont = std::move(cont), tile_coords, damage](double quality) {
										caster.region->add_effect(smake<lightning_bolt_effect>(tile_coords));
										if (being* target = caster.region->being_at(tile_coords)) {
											//! @todo Experimental body part stuff here... Delete or fix.

											// Pick a random strike path from the root to a leaf part.
											body_part* part = &target->body.root();
											std::vector<body_part*> struck_parts{part};
											while (!part->attachments.empty()) {
												part = &*part->attachments[uniform(std::size_t{0}, part->attachments.size() - 1)]
															 ->part;
												struck_parts.push_back(part);
											}
											for (auto struck_part : struck_parts) {
												dmg::group shock = damage * quality * uniform(0.5, 1.5) *
																   part->stats.a.vitality.value() /
																   (4.0_hp * struck_parts.size());
												struck_part->take_damage(shock, caster.id);
											}
										}
										return cont(action::result::success);
									});
							});
					});
			});
	}
}
