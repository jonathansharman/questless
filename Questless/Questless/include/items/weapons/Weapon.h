/**
* @file    Weapon.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Weapon abstract base class.
*/

#ifndef WEAPON_H
#define WEAPON_H

#include "items/Equipable.h"
#include "items/Breakable.h"
#include "entities/beings/Damage.h"

namespace questless
{
	class Weapon : public virtual Equipable, public virtual Breakable
	{
	public:
		using ptr = std::unique_ptr<Weapon>;

		virtual ~Weapon() = default;

		/// @return The weapon's effective damage per hit, accounting for wear.
		virtual Damage damage() const { return base_damage() * (1.0 + integrity() / durability() / 2.0); }

		/// @return The weapon's base damage per hit.
		virtual Damage base_damage() const = 0;

		/// @return Busy time incurred making the weapon ready.
		virtual double wind_up() const = 0;

		/// @return Busy time incurred after using the weapon.
		virtual double follow_through() const = 0;

		/// @return Time after using the weapon before it can be used again.
		virtual double cooldown() const = 0;

		/// @return The proportion of dealt damage to be applied as wear.
		virtual double wear_ratio() const = 0;

		/// @return Time left before the weapon can be used again.
		double active_cooldown() const { return _active_cooldown; }

		/// Sets the time left before the weapon can be used again to the given value.
		void active_cooldown(double value) { _active_cooldown = value; }

		/// @return Whether the weapon is ready.
		bool ready() const { return _ready; }
	protected:
		class Ready : public Action
		{
		public:
			Ready(Weapon& weapon, std::string name = "Ready") : _weapon{weapon}, _name{std::move(name)} {}

			static ptr make(Weapon& weapon, std::string name = "Ready") { return std::make_unique<Ready>(weapon, name); }

			std::string name() const override { return _name; }

			void perform(Being& actor, cont_t cont) override;
		private:
			Weapon& _weapon;
			const std::string _name;
		};

		class MeleeAttack : public Action
		{
		public:
			MeleeAttack(Weapon& weapon, std::string name = "Attack") : _weapon{weapon}, _name{std::move(name)} {}

			static ptr make(Weapon& weapon, std::string name = "Attack") { return std::make_unique<MeleeAttack>(weapon, name); }

			std::string name() const override { return _name; }

			void perform(Being& actor, cont_t cont) override;
		private:
			Weapon& _weapon;
			const std::string _name;
		};

		class Block : public Action
		{
		public:
			Block(Weapon& weapon, std::string name = "Block") : _weapon{weapon}, _name{std::move(name)} {}

			static ptr make(Weapon& weapon, std::string name = "Block") { return std::make_unique<Block>(weapon, name); }

			std::string name() const override { return _name; }

			void perform(Being& actor, cont_t cont) override;
		private:
			Weapon& _weapon;
			const std::string _name;
		};
	private:
		bool _ready = false;
		double _active_cooldown = 0.0;
	};
}

#endif
