//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>

#include "agents/Action.h"
#include "agents/Cost.h"
#include "entities/beings/Damage.h"
#include "utility/Id.h"

namespace questless
{
	class Item;
	class Weapon;

	//! An attack that a weapon can perform.
	class Attack : public std::enable_shared_from_this<Attack>
	{
	public:
		Id<Item> const weapon_id;

		Attack(Id<Item> weapon_id) : weapon_id{weapon_id} {}
		virtual ~Attack() = default;

		//! The name of the attack.
		virtual std::string name() const = 0;

		//! The attack's effective damage per hit, accounting for its weapon's integrity.
		Damage damage() const;

		//! The attack's base damage per hit.
		virtual Damage base_damage() const = 0;

		//! Busy time incurred before the attack connects.
		virtual double wind_up() const = 0;

		//! Busy time incurred after the attack connects.
		virtual double follow_through() const = 0;

		//! Time after using the attack before its weapon can be used again.
		virtual double cooldown() const = 0;

		//! The proportion of attack damage to be applied to the attack weapon as wear.
		virtual double wear_ratio() const = 0;

		//! The cost of performing the attack.
		virtual Cost const& cost() const = 0;

		//! An action that launches the attack.
		virtual Action::uptr launch() = 0;
	};

	//! A close-range attack.
	class MeleeAttack : public Attack
	{
	public:
		MeleeAttack(Id<Item> weapon_id) : Attack{weapon_id} {}
		virtual ~MeleeAttack() = default;

		Cost const& cost() const override
		{
			static Free free;
			return free;
		}

		Action::uptr launch() override
		{
			return Launch::make(std::dynamic_pointer_cast<MeleeAttack>(shared_from_this()));
		}
	private:
		class Launch : public Action
		{
		public:
			Launch(std::shared_ptr<MeleeAttack> attack) : _attack{std::move(attack)} {}
			static uptr make(std::shared_ptr<MeleeAttack> attack) { return std::make_unique<Launch>(std::move(attack)); }
			std::string name() const override { return _attack->name(); }
			Complete perform(Being& actor, cont_t cont) override;
		private:
			std::shared_ptr<MeleeAttack> _attack;
		};

		class Finish : public Action
		{
		public:
			Finish(std::shared_ptr<MeleeAttack> attack, RegionTileCoords::Direction direction)
				: _attack{std::move(attack)}, _direction{direction}
			{}
			std::string name() const override { return _attack->name(); }
			Complete perform(Being& actor, cont_t cont) override;
		private:
			std::shared_ptr<MeleeAttack> _attack;
			RegionTileCoords::Direction _direction;
		};
	};

	//! A long-range attack.
	class RangedAttack : public Attack
	{
	public:
		RangedAttack(Id<Item> weapon_id) : Attack{weapon_id} {}
		virtual ~RangedAttack() = default;

		Action::uptr launch() override
		{
			return Launch::make(std::dynamic_pointer_cast<RangedAttack>(shared_from_this()));
		}

		virtual int range() const = 0;
	private:
		class Launch : public Action
		{
		public:
			Launch(std::shared_ptr<RangedAttack> attack) : _attack{std::move(attack)} {}
			static uptr make(std::shared_ptr<RangedAttack> attack) { return std::make_unique<Launch>(std::move(attack)); }
			std::string name() const override { return _attack->name(); }
			Complete perform(Being& actor, cont_t cont) override;
		private:
			std::shared_ptr<RangedAttack> _attack;
		};

		class Finish : public Action
		{
		public:
			Finish(std::shared_ptr<RangedAttack> attack) : _attack{std::move(attack)} {}
			static uptr make(std::shared_ptr<RangedAttack> attack) { return std::make_unique<Finish>(attack); }
			std::string name() const override { return _attack->name(); }
			Complete perform(Being& actor, cont_t cont) override;
		private:
			std::shared_ptr<RangedAttack> _attack;
		};
	};
}
