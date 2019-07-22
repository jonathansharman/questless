//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "body_status_set.hpp"

#include "body.hpp"
#include "body_part.hpp"
#include "stats/aggregate.hpp"
#include "stats/body.hpp"

#include "utility/visitation.hpp"

namespace ql {
	namespace {
		void apply_single(body& body, body_status status, tick /*elapsed*/) {
			match(
				status,
				[&](blind const& b) {
					for (auto& v : body.stats.a.vision_sources) {
						v.acuity -= b.acuity_reduction;
					}
				},
				[&](confused const& c) { body.stats.a.intellect -= c.reduction; },
				[&](deaf const& d) { body.stats.a.hearing -= d.reduction; },
				[&](mute const& m) { body.stats.a.speech -= m.reduction; },
				[&](nauseated const& n) {
					body.stats.a.strength += n.strength_penalty * n.magnitude;
					body.stats.a.stamina += n.max_energy_penalty * n.magnitude;
				},
				[&](numb const&) {
					//! @todo Reduce action points.
					//! @todo Haven't decided how dexterity works yet.
					// stats.dexterity += numb::dexterity_penalty * n.magnitude;
				},
				[&](telescoped const& t) {
					for (auto& v : body.stats.a.vision_sources) {
						v.acuity += t.acuity_bonus;
					}
				});
		}
	}

	void body_status_set::apply(tick elapsed) {
		auto& body = reg.get<ql::body>(id);

		// Apply permanent effects.
		for (auto const& status : permanent) {
			apply_single(body, status, elapsed);
		}

		// Apply semipermanent effects.
		for (auto const& status : semipermanent) {
			apply_single(body, status, elapsed);
		}

		// Apply timed effects, decrease times, and remove any that expired.
		for (auto it = timed.begin(); it != timed.end(); ++it) {
			if (it->duration > elapsed) {
				// Will not expire this turn.
				apply_single(body, it->status, elapsed);
				it->duration -= elapsed;
			} else {
				// Expires after this turn.
				apply_single(body, it->status, it->duration);
				timed.erase(it);
				--it;
			}
		}
	}
}
