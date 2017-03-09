/**
* @file    Weapon.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Weapon abstract base class.
*/

#pragma once

#include "items/Equipment.h"
#include "items/Breakable.h"
#include "entities/beings/Damage.h"

namespace questless
{
	class Weapon : public virtual Equipment, public virtual Breakable
	{
	public:
		using ptr = std::unique_ptr<Weapon>;
		using ref = std::reference_wrapper<Weapon>;
		using cref = std::reference_wrapper<Weapon const>;

		virtual ~Weapon() = default;

		/// @return The weapon's effective damage per hit, accounting for wear.
		////
		Damage damage() const { return base_damage() * (1.0 + integrity / durability() / 2.0); }

		/// @return The weapon's base damage per hit.
		////
		virtual Damage base_damage() const = 0;

		/// @return Delay after the weapon is used before it deals damage.
		////
		virtual double wind_up() const = 0;

		/// @return Busy time incurred after using the weapon.
		////
		virtual double follow_through() const = 0;

		/// @return Time after using the weapon before it can be used again.
		////
		virtual double cooldown() const = 0;

		/// @return The proportion of dealt damage to be applied as wear.
		////
		virtual double wear_ratio() const = 0;

		/// Time left before the weapon can be used again.
		////
		double active_cooldown = 0.0;
	protected:
		class BeginMeleeAttack : public Action
		{
		public:
			BeginMeleeAttack(Weapon& weapon, std::string name = "Attack") : _weapon{weapon}, _name{std::move(name)} {}
			static ptr make(Weapon& weapon, std::string name = "Attack") { return std::make_unique<BeginMeleeAttack>(weapon, name); }
			std::string name() const override { return _name; }
			Action::Complete perform(Being& actor, cont_t cont) override;
		private:
			Weapon& _weapon;
			std::string const _name;
		};

		class Block : public Action
		{
		public:
			Block(Weapon& weapon, std::string name = "Block") : _weapon{weapon}, _name{std::move(name)} {}
			static ptr make(Weapon& weapon, std::string name = "Block") { return std::make_unique<Block>(weapon, name); }
			std::string name() const override { return _name; }
			Action::Complete perform(Being& actor, cont_t cont) override;
		private:
			Weapon& _weapon;
			std::string const _name;
		};
	private:
		class CompleteMeleeAttack : public Action
		{
		public:
			CompleteMeleeAttack(Weapon& weapon, RegionTileCoords coords) : _weapon{weapon}, _coords(coords) {}
			static ptr make(Weapon& weapon, RegionTileCoords coords) { return std::make_unique<CompleteMeleeAttack>(weapon, coords); }
			std::string name() const override { return ""; }
			Action::Complete perform(Being& actor, cont_t cont) override;
		private:
			Weapon& _weapon;
			RegionTileCoords _coords;
		};
	};
}
