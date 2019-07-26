//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "body_part_status_set.hpp"

#include "body_part.hpp"

#include "entities/beings/stats/aggregate.hpp"
#include "entities/beings/stats/body.hpp"
#include "utility/visitation.hpp"

namespace ql {
	namespace {
		void apply_status(body_part& part, body_part_status const& status, tick elapsed) {
			match(
				status,
				[&](healing const& h) { part.stats.regen_factor.cur += h.additional_regen_factor; },
				[&](poisoned const& p) { part.cond.poisoning += p.rate * elapsed; });
		}

		void apply_wound(body_part& part, wound const& w, tick /*elapsed*/) {
			match(
				w,
				[&](laceration const& l) {
					part.stats.a.vitality.cur -= l * 1_hp / 1_laceration;
					part.stats.bleeding.cur += l * 0.1_blood_per_tick / 1_laceration;
				},
				[&](puncture const& p) {
					part.stats.a.vitality.cur -= 1_hp / 1_puncture * p;
					part.stats.bleeding.cur += 0.2_blood_per_tick / 1_puncture * p;
				},
				[&](bruise const& b) { part.stats.a.vitality.cur -= 1_hp / 1_bruise * b; },
				[&](fracture const& f) { part.stats.a.vitality.cur -= 1_hp / 1_fracture * f; },
				[&](burn const& b) { part.stats.a.vitality.cur -= 1_hp / 1_burn * b; },
				[&](frostbite const& f) { part.stats.a.vitality.cur -= 1_hp / 1_frostbite * f; });
		}
	}

	void body_part_status_set::apply(tick elapsed) {
		auto& part = reg.get<body_part>(id);

		// Apply permanent effects.
		for (auto const& status : permanent) {
			apply_status(part, status, elapsed);
		}

		// Apply semipermanent effects.
		for (auto const& status : semipermanent) {
			apply_status(part, status, elapsed);
		}

		// Apply timed effects.
		for (auto it = timed.begin(); it != timed.end(); ++it) {
			// Check for expiration.
			if (it->duration <= 0_tick) {
				timed.erase(it);
				--it;
				continue;
			}
			// Compute the duration over which to apply the status.
			auto effective_elapsed = std::min(it->duration, elapsed);
			// Apply status.
			apply_status(part, it->status, effective_elapsed);
			// Decrease status duration.
			it->duration -= elapsed;
		}

		// Apply wound effects.
		for (auto it = wounds.begin(); it != wounds.end(); ++it) {
			// Check if the wound is healed.
			if (match(*it, [](auto const& w) { return w.value <= 0; })) {
				wounds.erase(it);
				--it;
			}
			// Apply wound.
			apply_wound(part, *it, elapsed);
			// Heal wound.
			match(
				*it,
				[&](laceration& l) { l -= 1_laceration * elapsed / 1_tick; },
				[&](puncture& p) { p -= 1_puncture * elapsed / 1_tick; },
				[&](bruise& b) { b -= 1_bruise * elapsed / 1_tick; },
				[&](fracture& f) { f -= 1_fracture * elapsed / 1_tick; },
				[&](burn& b) { b -= 1_burn * elapsed / 1_tick; },
				[&](frostbite& f) { f -= 1_frostbite * elapsed / 1_tick; });
		}

		// Cap bleeding here, after aggregating all sources of bleeding.
		part.stats.bleeding.cur = std::min(part.stats.bleeding.cur, part.stats.a.max_bleeding());
	}
}
