//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <optional>

#include "items/item.hpp"
#include "utility/tagged_type.hpp"

namespace ql
{
	class body_part;

	//! Abstract base class for items that can be equipped to a being.
	class equipment : public virtual item
	{
	public:
		virtual ~equipment() = default;

		//! The amount of time required to equip the item.
		virtual double equip_time() const = 0;

		//! The amount of time required to unequip the item.
		virtual double unequip_time() const = 0;

		//! Whether the item is currently equipped to a being.
		bool equipped() const { return _opt_bearer_id.has_value(); }

		//! The ID of the being bearing this equipment or nullopt if it's currently unequipped.
		std::optional<ql::id<being>> opt_bearer_id() const { return _opt_bearer_id; }
	protected:
		struct heads : tagged_type<int> { using tagged_type::tagged_type; };
		struct torsos : tagged_type<int> { using tagged_type::tagged_type; };
		struct arms : tagged_type<int> { using tagged_type::tagged_type; };
		struct hands : tagged_type<int> { using tagged_type::tagged_type; };
		struct legs : tagged_type<int> { using tagged_type::tagged_type; };
		struct feet : tagged_type<int> { using tagged_type::tagged_type; };
		struct wings : tagged_type<int> { using tagged_type::tagged_type; };
		struct tails : tagged_type<int> { using tagged_type::tagged_type; };

		class body_part_counts
		{
		public:
			constexpr body_part_counts() : _heads{0}, _torsos{0}, _arms{0}, _hands{0}, _legs{0}, _feet{0}, _wings{0}, _tails{0} {}

			constexpr body_part_counts
				( heads heads
				, torsos torsos
				, arms arms
				, hands hands
				, legs legs
				, feet feet
				, wings wings
				, tails tails
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

			constexpr body_part_counts(heads heads) : _heads{std::move(heads)}, _torsos{0}, _arms{0}, _hands{0}, _legs{0}, _feet{0}, _wings{0}, _tails{0} {}
			constexpr body_part_counts(torsos torsos) : _heads{0}, _torsos{std::move(torsos)}, _arms{0}, _hands{0}, _legs{0}, _feet{0}, _wings{0}, _tails{0} {}
			constexpr body_part_counts(arms arms) : _heads{0}, _torsos{0}, _arms{std::move(arms)}, _hands{0}, _legs{0}, _feet{0}, _wings{0}, _tails{0} {}
			constexpr body_part_counts(hands hands) : _heads{0}, _torsos{0}, _arms{0}, _hands{std::move(hands)}, _legs{0}, _feet{0}, _wings{0}, _tails{0} {}
			constexpr body_part_counts(legs legs) : _heads{0}, _torsos{0}, _arms{0}, _hands{0}, _legs{std::move(legs)}, _feet{0}, _wings{0}, _tails{0} {}
			constexpr body_part_counts(feet feet) : _heads{0}, _torsos{0}, _arms{0}, _hands{0}, _legs{0}, _feet{std::move(feet)}, _wings{0}, _tails{0} {}
			constexpr body_part_counts(wings wings) : _heads{0}, _torsos{0}, _arms{0}, _hands{0}, _legs{0}, _feet{0}, _wings{std::move(wings)}, _tails{0} {}
			constexpr body_part_counts(tails tails) : _heads{0}, _torsos{0}, _arms{0}, _hands{0}, _legs{0}, _feet{0}, _wings{0}, _tails{std::move(tails)} {}

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

		friend class equip;
		class equip : public action
		{
		public:
			equip(equipment& equipment) : _equipment{equipment} {}
			static auto make(equipment& equipment) { return umake<equip>(equipment); }
			std::string name() const final { return "Equip"; }
			complete perform(being& actor, cont cont) final;
		private:
			equipment& _equipment;
		};

		friend class unequip;
		class unequip : public action
		{
		public:
			unequip(equipment& equipment) : _equipment{equipment} {}
			static auto make(equipment& equipment) { return umake<unequip>(equipment); }
			std::string name() const final { return "Unequip"; }
			complete perform(being& actor, cont cont) final;
		private:
			equipment& _equipment;
		};
	private:
		std::optional<ql::id<being>> _opt_bearer_id;
		std::vector<ql::id<body_part>> _part_ids;

		std::vector<ql::id<body_part>> _head_ids;
		std::vector<ql::id<body_part>> _torso_ids;
		std::vector<ql::id<body_part>> _arm_ids;
		std::vector<ql::id<body_part>> _hand_ids;
		std::vector<ql::id<body_part>> _leg_ids;
		std::vector<ql::id<body_part>> _foot_ids;
		std::vector<ql::id<body_part>> _wing_ids;
		std::vector<ql::id<body_part>> _tail_ids;

		//! The number of each body part type required to equip this item.
		virtual body_part_counts requirements() const = 0;

		//! Unequips the item from its bearer.
		void unequip();
	};

	DEFINE_ELEMENT_BASE_MAKE_CTOR(equipment, item)
}
