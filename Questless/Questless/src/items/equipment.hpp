//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/item.hpp"

#include "cancel/quantity.hpp"

#include <optional>

namespace ql {
	class body_part;

	//! Abstract base class for items that can be equipped to a being.
	class equipment : public virtual item {
	public:
		virtual ~equipment() = default;

		//! The amount of time required to equip the item.
		virtual tick equip_time() const = 0;

		//! The amount of time required to unequip the item.
		virtual tick unequip_time() const = 0;

		//! Whether the item is currently equipped to a being.
		bool equipped() const { return _opt_bearer_id.has_value(); }

		//! The ID of the being bearing this equipment or nullopt if it's currently unequipped.
		std::optional<ql::id<being>> opt_bearer_id() const { return _opt_bearer_id; }
	protected:
		using heads = cancel::quantity<int, cancel::unit_t<struct heads_tag>>;
		using torsos = cancel::quantity<int, cancel::unit_t<struct torsos_tag>>;
		using arms = cancel::quantity<int, cancel::unit_t<struct arms_tag>>;
		using hands = cancel::quantity<int, cancel::unit_t<struct hands_tag>>;
		using legs = cancel::quantity<int, cancel::unit_t<struct legs_tag>>;
		using feet = cancel::quantity<int, cancel::unit_t<struct feet_tag>>;
		using wings = cancel::quantity<int, cancel::unit_t<struct wings_tag>>;
		using tails = cancel::quantity<int, cancel::unit_t<struct tails_tag>>;

		struct body_part_counts {
			heads heads{0};
			torsos torsos{0};
			arms arms{0};
			hands hands{0};
			legs legs{0};
			feet feet{0};
			wings wings{0};
			tails tails{0};

			constexpr body_part_counts(equipment::heads heads) : heads{heads} {}
			constexpr body_part_counts(equipment::torsos torsos) : torsos{torsos} {}
			constexpr body_part_counts(equipment::arms arms) : arms{arms} {}
			constexpr body_part_counts(equipment::hands hands) : hands{hands} {}
			constexpr body_part_counts(equipment::legs legs) : legs{legs} {}
			constexpr body_part_counts(equipment::feet feet) : feet{feet} {}
			constexpr body_part_counts(equipment::wings wings) : wings{wings} {}
			constexpr body_part_counts(equipment::tails tails) : tails{tails} {}
		};

		friend class equip;
		class equip : public action {
		public:
			equip(equipment& equipment) : _equipment{equipment} {}
			static auto make(equipment& equipment) { return umake<equip>(equipment); }
			std::string name() const final { return "Equip"; }
			complete perform(being& actor, cont cont) final;
		private:
			equipment& _equipment;
		};

		friend class unequip;
		class unequip : public action {
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
