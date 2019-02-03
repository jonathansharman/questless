//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/weapons/weapon.hpp"

namespace ql {
	struct bow : weapon_base<bow> {
		bow(ql::id<item> id = ql::id<item>::make())
			: item{id}, weapon_base<bow>{durability()}, _shoot{smake<shoot>(id)} {}

		std::string name() const final {
			return "Bow";
		}

		ql::mass mass() const final {
			return 3.5_mass;
		}

		tick equip_time() const final {
			return 120_tick;
		}
		tick unequip_time() const final {
			return 30_tick;
		}

		ql::integrity durability() const final {
			return 400.0_integrity;
		}

		std::vector<uptr<action>> actions() final;

	private:
		struct shoot : ranged_attack {
			shoot(ql::id<item> weapon_id) : ranged_attack{weapon_id} {}
			std::string name() const final {
				return "Shoot";
			}
			dmg::group base_damage() const final {
				return dmg::pierce{30.0};
			}
			tick wind_up() const final {
				return 50_tick;
			}
			tick follow_through() const final {
				return 5_tick;
			}
			tick cooldown() const final {
				return 10_tick;
			}
			cancel::quotient_t<ql::integrity, ql::health> wear_ratio() const final {
				return 0.002_integrity / 1.0_hp;
			};
			ql::cost const& cost() const final {
				return _cost;
			}
			span range() const final {
				return 7_span;
			}

		private:
			struct arrow_cost : cost {
				complete check(being& actor, cont cont) const final;
				void incur(being& actor) const final;
			};

			arrow_cost _cost;

			sptr<effect> get_effect(region_tile::point source, region_tile::point target) final;
		};

		sptr<shoot> _shoot;

		body_part_counts requirements() const final {
			return hands{2};
		}
	};
}