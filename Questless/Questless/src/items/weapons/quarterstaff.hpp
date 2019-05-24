//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/weapons/modal_weapon.hpp"

namespace ql {
	struct quarterstaff : modal_weapon_base<quarterstaff> {
		quarterstaff(ql::id<item> id = ql::id<item>::make());

		std::string name() const final {
			return "Staff (" + current_form().name() + ')';
		}

		ql::mass mass() const final {
			return 10.0_mass;
		}

		tick equip_time() const final {
			return 15_tick;
		}
		tick unequip_time() const final {
			return 10_tick;
		}

		ql::integrity durability() const final {
			return 500.0_integrity;
		}

		tick switch_time() const final {
			return 3_tick;
		}

	private:
		struct standard_form : form {
			standard_form(modal_weapon& weapon)
				: form{weapon}, _strike{smake<strike>(weapon.id)}, _jab{smake<jab>(weapon.id)} {}

			std::string name() const final {
				return "Quarterstaff";
			}

			std::vector<uptr<action>> actions() final;

		private:
			struct strike : melee_attack {
				using melee_attack::melee_attack;
				std::string name() const final {
					return "Strike";
				}
				dmg::group base_damage() const final {
					return 24.0_bludgeon;
				}
				tick wind_up() const final {
					return 10_tick;
				}
				tick follow_through() const final {
					return 10_tick;
				}
				tick cooldown() const final {
					return 20_tick;
				}
				cancel::quotient_t<ql::integrity, ql::health> wear_ratio() const final {
					return 0.001_integrity / 1.0_hp;
				}

			private:
				sptr<effect> get_effect(region_tile::point /*source*/, region_tile::point /*target*/) final {
					return nullptr;
				} // @todo This.
			};
			struct jab : melee_attack {
				using melee_attack::melee_attack;
				std::string name() const final {
					return "Jab";
				}
				dmg::group base_damage() const final {
					return 12.0_bludgeon;
				}
				tick wind_up() const final {
					return 2_tick;
				}
				tick follow_through() const final {
					return 8_tick;
				}
				tick cooldown() const final {
					return 13_tick;
				}
				cancel::quotient_t<ql::integrity, ql::health> wear_ratio() const final {
					return 0.001_integrity / 1.0_hp;
				}

			private:
				sptr<effect> get_effect(region_tile::point /*source*/, region_tile::point /*target*/) final {
					return nullptr;
				} // @todo This.
			};

			sptr<strike> _strike;
			sptr<jab> _jab;
		};

		struct half_staff_form : form {
			half_staff_form(modal_weapon& weapon)
				: form{weapon}, _strike{smake<strike>(weapon.id)}, _jab{smake<jab>(weapon.id)} {}

			std::string name() const final {
				return "Half Staff";
			}

			std::vector<uptr<action>> actions() final;

		private:
			struct strike : melee_attack {
				using melee_attack::melee_attack;
				std::string name() const final {
					return "Strike";
				}
				dmg::group base_damage() const final {
					return dmg::group{dmg::bludgeon{18.0}, dmg::protect{6.0_pad} + dmg::protect{6.0_deflect}};
				}
				tick wind_up() const final {
					return 8_tick;
				}
				tick follow_through() const final {
					return 8_tick;
				}
				tick cooldown() const final {
					return 16_tick;
				}
				cancel::quotient_t<ql::integrity, ql::health> wear_ratio() const final {
					return 0.001_integrity / 1.0_hp;
				}

			private:
				sptr<effect> get_effect(region_tile::point /*source*/, region_tile::point /*target*/) final {
					return nullptr;
				} // @todo This.
			};
			struct jab : melee_attack {
				using melee_attack::melee_attack;
				std::string name() const final {
					return "Jab";
				}
				dmg::group base_damage() const final {
					return dmg::group{9.0_bludgeon, dmg::protect{6.0_pad} + dmg::protect{6.0_deflect}};
				}
				tick wind_up() const final {
					return 2_tick;
				}
				tick follow_through() const final {
					return 6_tick;
				}
				tick cooldown() const final {
					return 10_tick;
				}
				cancel::quotient_t<ql::integrity, ql::health> wear_ratio() const final {
					return 0.001_integrity / 1.0_hp;
				}

			private:
				sptr<effect> get_effect(region_tile::point /*source*/, region_tile::point /*target*/) final {
					return nullptr;
				} // @todo This.
			};

			sptr<strike> _strike;
			sptr<jab> _jab;
		};

		body_part_counts requirements() const final {
			return hands{2};
		}
	};
}
