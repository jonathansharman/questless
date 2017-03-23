/**
* @file    Attack.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#pragma once

#include <string>

#include "agents/Action.h"
#include "entities/beings/Damage.h"
#include "utility/Id.h"

namespace questless
{
	class Item;
	class Weapon;

	/// An attack that a weapon can perform.
	////
	class Attack
	{
	public:
		using ptr = std::unique_ptr<Attack>;

		Weapon& weapon;

		Attack(Weapon& weapon) : weapon{weapon} {}
		virtual ~Attack() = default;

		/// @return The name of the attack.
		////
		virtual std::string name() const = 0;

		/// @return The attack's effective damage per hit, accounting for its weapon's integrity.
		////
		Damage damage() const;

		/// @return The attack's base damage per hit.
		////
		virtual Damage base_damage() const = 0;

		/// @return Busy time incurred before the attack connects.
		////
		virtual double wind_up() const = 0;

		/// @return Busy time incurred after the attack connects.
		////
		virtual double follow_through() const = 0;

		/// @return Time after using the attack before its weapon can be used again.
		////
		virtual double cooldown() const = 0;

		/// @return The proportion of attack damage to be applied to the attack weapon as wear.
		////
		virtual double wear_ratio() const = 0;

		/// @return An action that launches the attack.
		virtual Action::ptr launch() = 0;
	};

	/// A close-range attack.
	////
	class MeleeAttack : public Attack
	{
	public:
		MeleeAttack(Weapon& weapon) : Attack{weapon} {}
		virtual ~MeleeAttack() = default;

		Action::ptr launch() override { return Launch::make(*this); }
	private:
		class Launch : public Action
		{
		public:
			Launch(Attack& attack) : _attack{attack} {}
			static ptr make(Attack& attack) { return std::make_unique<Launch>(attack); }
			std::string name() const override { return _attack.name(); }
			Action::Complete perform(Being& actor, cont_t cont) override;
		private:
			Attack& _attack;
		};

		class Finish : public Action
		{
		public:
			Finish(Attack const& attack, RegionTileCoords coords);
			static ptr make(Attack const& attack, RegionTileCoords coords) { return std::make_unique<Finish>(attack, coords); }
			std::string name() const override { return _name; }
			Action::Complete perform(Being& actor, cont_t cont) override;
		private:
			Id<Item> _weapon_id;
			std::string _name;
			Damage _damage;
			double _follow_through;
			double _cooldown;
			double _wear_ratio;
			RegionTileCoords _coords;
		};
	};

	/// A long-range attack.
	////
	class RangedAttack : public Attack
	{
	public:
		RangedAttack(Weapon& weapon) : Attack{weapon} {}
		virtual ~RangedAttack() = default;

		Action::ptr launch() override { return Launch::make(*this); }

		virtual int range() const = 0;
	private:
		class Launch : public Action
		{
		public:
			Launch(RangedAttack& attack) : _attack{attack} {}
			static ptr make(RangedAttack& attack) { return std::make_unique<Launch>(attack); }
			std::string name() const override { return _attack.name(); }
			Action::Complete perform(Being& actor, cont_t cont) override;
		private:
			RangedAttack& _attack;
		};

		class Finish : public Action
		{
		public:
			Finish(RangedAttack const& attack);
			static ptr make(RangedAttack const& attack) { return std::make_unique<Finish>(attack); }
			std::string name() const override { return _name; }
			Action::Complete perform(Being& actor, cont_t cont) override;
		private:
			Id<Item> _weapon_id;
			std::string _name;
			Damage _damage;
			double _follow_through;
			double _cooldown;
			double _wear_ratio;
			int _range;
		};
	};
}
