//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <optional>

#include "items/Item.h"
#include "utility/TaggedType.h"

namespace questless
{
	class BodyPart;

	//! Abstract base class for items that can be equipped to a being.
	class Equipment : public virtual Item
	{
	public:
		virtual ~Equipment() = default;

		//! The amount of time required to equip the item.
		virtual double equip_time() const = 0;

		//! The amount of time required to unequip the item.
		virtual double unequip_time() const = 0;

		//! Whether the item is currently equipped to a being.
		bool equipped() const { return _bearer_id.has_value(); }

		//! The ID of the being bearing this equipment or nullopt if it's currently unequipped.
		std::optional<Id<Being>> bearer_id() const { return _bearer_id; }
	protected:
		struct Heads : TaggedType<int> { using TaggedType::TaggedType; };
		struct Torsos : TaggedType<int> { using TaggedType::TaggedType; };
		struct Arms : TaggedType<int> { using TaggedType::TaggedType; };
		struct Hands : TaggedType<int> { using TaggedType::TaggedType; };
		struct Legs : TaggedType<int> { using TaggedType::TaggedType; };
		struct Feet : TaggedType<int> { using TaggedType::TaggedType; };
		struct Wings : TaggedType<int> { using TaggedType::TaggedType; };
		struct Tails : TaggedType<int> { using TaggedType::TaggedType; };

		class Requirements
		{
		public:
			constexpr Requirements() : _heads{0}, _torsos{0}, _arms{0}, _hands{0}, _legs{0}, _feet{0}, _wings{0}, _tails{0} {}

			constexpr Requirements
				( Heads heads
				, Torsos torsos
				, Arms arms
				, Hands hands
				, Legs legs
				, Feet feet
				, Wings wings
				, Tails tails
				)
				: _heads{std::move(heads)}
				, _torsos{std::move(torsos)}
				, _arms{std::move(arms)}
				, _hands{std::move(hands)}
				, _legs{std::move(legs)}
				, _feet{std::move(feet)}
				, _wings{std::move(wings)}
				, _tails{std::move(tails)}
			{}

			constexpr Requirements(Heads heads) : _heads{std::move(heads)}, _torsos{0}, _arms{0}, _hands{0}, _legs{0}, _feet{0}, _wings{0}, _tails{0} {}
			constexpr Requirements(Torsos torsos) : _heads{0}, _torsos{std::move(torsos)}, _arms{0}, _hands{0}, _legs{0}, _feet{0}, _wings{0}, _tails{0} {}
			constexpr Requirements(Arms arms) : _heads{0}, _torsos{0}, _arms{std::move(arms)}, _hands{0}, _legs{0}, _feet{0}, _wings{0}, _tails{0} {}
			constexpr Requirements(Hands hands) : _heads{0}, _torsos{0}, _arms{0}, _hands{std::move(hands)}, _legs{0}, _feet{0}, _wings{0}, _tails{0} {}
			constexpr Requirements(Legs legs) : _heads{0}, _torsos{0}, _arms{0}, _hands{0}, _legs{std::move(legs)}, _feet{0}, _wings{0}, _tails{0} {}
			constexpr Requirements(Feet feet) : _heads{0}, _torsos{0}, _arms{0}, _hands{0}, _legs{0}, _feet{std::move(feet)}, _wings{0}, _tails{0} {}
			constexpr Requirements(Wings wings) : _heads{0}, _torsos{0}, _arms{0}, _hands{0}, _legs{0}, _feet{0}, _wings{std::move(wings)}, _tails{0} {}
			constexpr Requirements(Tails tails) : _heads{0}, _torsos{0}, _arms{0}, _hands{0}, _legs{0}, _feet{0}, _wings{0}, _tails{std::move(tails)} {}

			constexpr int heads() const { return _heads; }
			constexpr int torsos() const { return _torsos; }
			constexpr int arms() const { return _arms; }
			constexpr int hands() const { return _hands; }
			constexpr int legs() const { return _legs; }
			constexpr int feet() const { return _feet; }
			constexpr int wings() const { return _wings; }
			constexpr int tails() const { return _tails; }
		private:
			int _heads;
			int _torsos;
			int _arms;
			int _hands;
			int _legs;
			int _feet;
			int _wings;
			int _tails;
		};

		class Equip : public Action
		{
		public:
			Equip(Equipment& equipment) : _equipment{equipment} {}
			static auto make(Equipment& equipment) { return std::make_unique<Equip>(equipment); }
			std::string name() const override { return "Equip"; }
			Complete perform(Being& actor, cont_t cont) override;
		private:
			Equipment& _equipment;
		};
		friend class Equip;

		class Unequip : public Action
		{
		public:
			Unequip(Equipment& equipment) : _equipment{equipment} {}
			static auto make(Equipment& equipment) { return std::make_unique<Unequip>(equipment); }
			std::string name() const override { return "Unequip"; }
			Complete perform(Being& actor, cont_t cont) override;
		private:
			Equipment& _equipment;
		};
		friend class Unequip;
	private:
		std::optional<Id<Being>> _bearer_id;
		std::vector<Id<BodyPart>> _part_ids;

		std::vector<Id<BodyPart>> _head_ids;
		std::vector<Id<BodyPart>> _torso_ids;
		std::vector<Id<BodyPart>> _arm_ids;
		std::vector<Id<BodyPart>> _hand_ids;
		std::vector<Id<BodyPart>> _leg_ids;
		std::vector<Id<BodyPart>> _foot_ids;
		std::vector<Id<BodyPart>> _wing_ids;
		std::vector<Id<BodyPart>> _tail_ids;

		//! The number of each body part type required to equip this item.
		virtual Requirements requirements() const = 0;

		//! Unequips the item from its bearer.
		void unequip();
	};

	DEFINE_CONST_ELEMENT_BASE_MAKE_CTOR(Equipment, Item)
}
