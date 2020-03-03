//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "body_status_set.hpp"

#include "body.hpp"
#include "body_part.hpp"
#include "stats/aggregate.hpp"
#include "stats/body.hpp"

#include "utility/visitation.hpp"

namespace ql {
	namespace {
		auto apply_status(body& body, body_status status, tick /*elapsed*/) -> void {
			match(
				status,
				[&](blind const& b) {
					for (auto& v : body.stats.a.vision_sources.cur) {
						v.acuity -= b.acuity_reduction;
					}
				},
				[&](confused const& c) { body.stats.a.intellect.cur -= c.reduction; },
				[&](deaf const& d) { body.stats.a.hearing.cur -= d.reduction; },
				[&](mute const& m) { body.stats.a.speech.cur -= m.reduction; },
				[&](nauseated const& n) {
					body.stats.a.strength.cur += n.strength_penalty * n.magnitude;
					body.stats.a.stamina.cur += n.max_energy_penalty * n.magnitude;
				},
				[&](numb const&) {
					//! @todo Reduce action points.
					//! @todo Haven't decided how dexterity works yet.
					// stats.dexterity += numb::dexterity_penalty * n.magnitude;
				},
				[&](telescoped const& t) {
					for (auto& v : body.stats.a.vision_sources.cur) {
						v.acuity += t.acuity_bonus;
					}
				});
		}
	}

	auto body_status_set::apply(reg& reg, tick elapsed) -> void {
		auto& body = reg.get<ql::body>(id);

		// Apply permanent effects.
		for (auto const& status : permanent) {
			apply_status(body, status, elapsed);
		}

		// Apply semipermanent effects.
		for (auto const& status : semipermanent) {
			apply_status(body, status, elapsed);
		}

		// Remove expired timed effects.
		timed.erase( //
			std::remove_if( //
				timed.begin(),
				timed.end(),
				[](timed_body_status const& status) { return status.duration <= 0_tick; }),
			timed.end());
		// Apply unexpired timed status effects; decrement time left.
		for (auto& status : timed) {
			// Compute the duration over which to apply the status.
			auto const effective_elapsed = std::min(status.duration, elapsed);
			// Apply status.
			apply_status(body, status.status, effective_elapsed);
			// Decrease status duration.
			status.duration -= elapsed;
		}
	}
}
