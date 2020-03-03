//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "body.hpp"

#include "being.hpp"
#include "body_part.hpp"

#include "utility/utility.hpp"

#include <numeric>
#include <queue>

namespace ql {
	namespace {
		auto reset_stats(body& b) -> void {
			b.stats.reset();
			// Aggregate part stats.
			b.for_enabled_parts([&](body_part const& part) { b.stats.a.combine_with(part.stats.a); });
			// Apply condition effects.
			if (b.cond.weary()) { b.stats.a.strength.cur /= 2; }
			if (b.cond.sleepy()) { b.stats.a.intellect.cur /= 2; }
		}

		template <typename ConstQualifiedBodyPartType, typename F>
		auto for_parts_impl(reg& reg, body const& body, bool include_disabled, F const& f) -> void {
			std::queue<id> work_list;
			work_list.push(body.root_part_id);
			while (!work_list.empty()) {
				// Get the next part from the work list.
				id part_id = work_list.front();
				ConstQualifiedBodyPartType part = reg.get<body_part>(part_id);
				// Check ableness condition before proceeding into this branch.
				if (include_disabled || part.cond.enabled()) {
					// Apply the function.
					f(part);
					// Add any children of the current part to the work list.
					for (auto const& attachment : part.attachments) {
						if (attachment.o_part_id) { work_list.push(*attachment.o_part_id); }
					}
				}
				// Pop the current part from the work list.
				work_list.pop();
			}
		}
	}

	body::body(reg& reg, ql::id id, ql::id root_part_id, body_cond cond, stats::body stats)
		: _reg{&reg}
		, id{id}
		, root_part_id{root_part_id}
		, cond{std::move(cond)}
		, stats{std::move(stats)} //
	{
		reset_stats(*this);
	}

	auto body::for_all_parts(std::function<void(body_part const&)> const& f) const -> void {
		for_parts_impl<body_part const&>(*_reg, *this, true, f);
	}
	auto body::for_all_parts(std::function<void(body_part&)> const& f) -> void {
		for_parts_impl<body_part&>(*_reg, *this, true, f);
	}

	auto body::for_enabled_parts(std::function<void(body_part const&)> const& f) const -> void {
		for_parts_impl<body_part const&>(*_reg, *this, false, f);
	}
	auto body::for_enabled_parts(std::function<void(body_part&)> const& f) -> void {
		for_parts_impl<body_part&>(*_reg, *this, false, f);
	}

	auto body::update(tick elapsed) -> void {
		// Reset and aggregate stats.
		reset_stats(*this);

		// Update conditions.
		cond.satiety -= (cond.awake() ? 0.05_sat : 0.025_sat) / 1_tick * elapsed;
		cond.energy += (cond.awake() ? 1_ep : 3_ep) / 1_tick * elapsed;
		cond.alertness += (cond.awake() ? -0.1_alert : 0.2_alert) / 1_tick * elapsed;

		// Bleed and regenerate blood.
		for_all_parts([&](body_part const& part) {
			// Intermediate blood values may leave the valid range.
			cond.blood += (part.stats.blood_regen() - part.stats.bleeding.cur) * elapsed;
		});
		// After all blood contributions have been added, clamp blood into the valid region.
		cond.blood = std::clamp(cond.blood, 0.0_blood, stats.a.max_blood());

		{ // Handle blood loss.
			constexpr double stage_1_max = 0.15;
			constexpr double stage_2_max = 0.3;
			constexpr double stage_3_max = 0.4;

			double const pct_blood_lost = 1.0 - (cond.blood / stats.a.max_blood());

			// No effects for stage 1 blood loss.
			if (pct_blood_lost > stage_1_max) {
				// Stage 2: anxiety.

				// Joy loss as a factor of stage-2 blood loss.
				constexpr auto joy_loss_rate = 1.0_joy / 1_tick;
				cond.joy -= (pct_blood_lost - stage_1_max) / (1.0 - stage_1_max) * joy_loss_rate * elapsed;

				// Courage loss as a factor of stage-2 blood loss.
				constexpr auto courage_loss_rate = 1.0_courage / 1_tick;
				cond.courage -= (pct_blood_lost - stage_1_max) / (1.0 - stage_1_max) * courage_loss_rate * elapsed;
			}
			if (pct_blood_lost > stage_2_max) {
				// Stage 3: confusion.

				// Intellect reduction as a factor of stage-3 blood loss.
				constexpr auto intellect_loss_rate = 50_int;
				stats.a.intellect.cur -= cancel::quantity_cast<intellect>(
					(pct_blood_lost - stage_2_max) / (1.0 - stage_2_max) * intellect_loss_rate);
			}
			if (pct_blood_lost > stage_3_max) {
				// Stage 4: lethargy, loss of consciousness, damage.

				double const pct_stage_4_blood_lost = (pct_blood_lost - stage_3_max) / (1.0 - stage_3_max);

				// Energy loss as a factor of stage-4 blood loss.
				constexpr auto energy_loss_rate = 1_ep / 1_tick;
				cond.energy -= cancel::quantity_cast<energy>(pct_stage_4_blood_lost * energy_loss_rate * elapsed);

				// Alertness loss as a factor of stage-4 blood loss.
				constexpr auto alertness_loss_rate = 1.0_alert / 1_tick;
				cond.alertness -= pct_stage_4_blood_lost * alertness_loss_rate * elapsed;

				// Reduce vitality in proportion to stage-4 blood loss.
				constexpr auto decay_rate = 1_decay / 1_tick;
				for_all_parts([&](body_part& part) {
					part.stats.a.vitality.cur -= cancel::quantity_cast<health>(
						part.stats.a.vitality.base * pct_stage_4_blood_lost);
				});
			}
		}

		// Update mortality state.
		switch (cond.mortality) {
			case mortality::alive:
				if (stats.a.vitality.cur <= 0_hp) {
					// Alive with no vitality...
					cond.mortality = stats.a.undeath.cur > 0_undeath
						// ...with undeath -> undead.
						? mortality::undead
						// ...without undeath -> dead.
						: cond.mortality = mortality::dead;
				}
				break;
			case mortality::dead:
				// Dead with undeath -> undead.
				if (stats.a.undeath.cur > 0_undeath) { cond.mortality = mortality::undead; }
				break;
			case mortality::undead:
				// Undead without undeath -> dead.
				if (stats.a.undeath.cur <= 0_undeath) { cond.mortality = mortality::dead; }
				break;
			case mortality::immortal:
				// Immortal -> immortal.
				break;
		}
	}
}
