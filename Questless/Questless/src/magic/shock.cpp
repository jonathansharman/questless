//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "magic/shock.hpp"

#include "agents/agent.hpp"
#include "agents/queries/magnitude_query.hpp"
#include "agents/queries/message.hpp"
#include "agents/queries/tile_query.hpp"
#include "game.hpp"
#include "entities/beings/being.hpp"
#include "effects/effect.hpp"
#include "magic/charge_cost.hpp"
#include "utility/random.hpp"

//! @todo Body part stuff - remove if not used.
#include "entities/beings/body_part.hpp"
#include <deque>
#include <type_traits>

namespace ql::magic
{
	complete shock::perform_cast(being& caster, gatestone& gatestone, action::cont cont)
	{
		return caster.agent().query_tile(umake<tile_query_shock_target>(), caster.coords, action::tile_in_range_predicate(caster, _range),
			[&caster, &gatestone, cont = std::move(cont)](std::optional<region_tile::point> opt_tile_coords) {
				if (!opt_tile_coords) {
					return cont(action::result::aborted);
				}
				region_tile::point tile_coords = *opt_tile_coords;
				return caster.agent().query_magnitude(umake<magnitude_query_shock>(), 20.0, 0.0, std::nullopt,
					[&caster, &gatestone, cont = std::move(cont), tile_coords](std::optional<double> opt_magnitude) {
						if (!opt_magnitude) {
							return cont(action::result::aborted);
						}
						double magnitude = *opt_magnitude;
						return charge_cost{gatestone, _cost_factor * magnitude * log2(magnitude + _cost_log)}.check_and_incur(caster,
							[&caster, &gatestone, cont = std::move(cont), tile_coords, magnitude] {
								return caster.agent().get_shock_quality(tile_coords,
									[&caster, &gatestone, cont = std::move(cont), tile_coords, magnitude](double quality) {
										caster.region->add_effect(smake<lightning_bolt_effect>(tile_coords));
										if (being* target = caster.region->being_at(tile_coords)) {
											double shock_magnitude = magnitude * quality;

											//! @todo Experimental body part stuff here... Delete or fix.

											// Pick a random strike path from the root to a leaf part.
											body_part* part = &target->body.root();
											std::vector<body_part*> struck_parts{part};
											while (!part->attachments().empty()) {
												auto& attachments = part->attachments();
												part = attachments[uniform(std::size_t{0}, attachments.size() - 1)]->part.get();
												struck_parts.push_back(part);
											}
											for (auto struck_part : struck_parts) {
												dmg::group shock = dmg::shock{shock_magnitude * uniform(0.5, 1.5) * part->vitality / (4.0 * struck_parts.size())};
												struck_part->take_damage(shock, caster.id);
											}
										}
										return cont(action::result::success);
									}
								);
							}
						);
					}
				);
			}
		);
	}
}
