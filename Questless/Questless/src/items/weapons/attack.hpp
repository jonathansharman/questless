//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "agents/action.hpp"
#include "agents/cost.hpp"
#include "damage/group.hpp"
#include "utility/id.hpp"
#include "utility/reference.hpp"

namespace ql {
	class effect;
	class item;
	class weapon;

	//! An attack that a weapon can perform.
	class attack : public std::enable_shared_from_this<attack> {
	public:
		id<item> const weapon_id;

		attack(id<item> weapon_id) : weapon_id{weapon_id} {}
		virtual ~attack() = default;

		//! The name of the attack.
		virtual std::string name() const = 0;

		//! The attack's effective damage per hit, accounting for its weapon's integrity.
		dmg::group damage() const;

		//! The attack's base damage per hit.
		virtual dmg::group base_damage() const = 0;

		//! Busy time incurred before the attack connects.
		virtual tick wind_up() const = 0;

		//! Busy time incurred after the attack connects.
		virtual tick follow_through() const = 0;

		//! Time after using the attack before its weapon can be used again.
		virtual tick cooldown() const = 0;

		//! Integrity removed from the weapon per HP the attack removes.
		virtual decltype(ql::integrity{} / ql::health{}) wear_ratio() const = 0;

		//! The cost of performing the attack.
		virtual cost const& cost() const = 0;

		//! An action that launches the attack.
		virtual uptr<action> launch() = 0;
	protected:
		//! Creates an effect representing this attack.
		virtual sptr<effect> get_effect(region_tile::point source, region_tile::point target) = 0;
	};

	//! A close-range attack.
	class melee_attack : public attack {
	public:
		melee_attack(id<item> weapon_id) : attack{weapon_id} {}
		virtual ~melee_attack() = default;

		ql::cost const& cost() const override {
			static free free;
			return free;
		}

		uptr<action> launch() final {
			return launch::make(std::dynamic_pointer_cast<melee_attack>(shared_from_this()));
		}
	private:
		class launch : public action {
		public:
			launch(sptr<melee_attack> attack) : _attack{std::move(attack)} {}
			static uptr<launch> make(sptr<melee_attack> attack) { return umake<launch>(std::move(attack)); }
			std::string name() const final { return _attack->name(); }
			complete perform(being& actor, cont cont) final;
		private:
			sptr<melee_attack> _attack;
		};

		class finish : public action {
		public:
			finish(sptr<melee_attack> attack, region_tile::vector vector)
				: _attack{std::move(attack)}, _vector{vector}
			{}
			std::string name() const final { return _attack->name(); }
			complete perform(being& actor, cont cont) final;
		private:
			sptr<melee_attack> _attack;
			region_tile::vector _vector;
		};
	};

	//! A long-range attack.
	class ranged_attack : public attack {
	public:
		ranged_attack(id<item> weapon_id) : attack{weapon_id} {}
		virtual ~ranged_attack() = default;

		uptr<action> launch() final {
			return launch::make(std::dynamic_pointer_cast<ranged_attack>(shared_from_this()));
		}

		virtual span range() const = 0;
	private:
		class launch : public action {
		public:
			launch(sptr<ranged_attack> attack) : _attack{std::move(attack)} {}
			static uptr<launch> make(sptr<ranged_attack> attack) { return umake<launch>(std::move(attack)); }
			std::string name() const final { return _attack->name(); }
			complete perform(being& actor, cont cont) final;
		private:
			sptr<ranged_attack> _attack;
		};

		class finish : public action {
		public:
			finish(sptr<ranged_attack> attack) : _attack{std::move(attack)} {}
			static auto make(sptr<ranged_attack> attack) { return umake<finish>(attack); }
			std::string name() const final { return _attack->name(); }
			complete perform(being& actor, cont cont) final;
		private:
			sptr<ranged_attack> _attack;
		};
	};
}
