//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "body_part_visitor.hpp"
#include "damage/group.hpp"
#include "items/item.hpp"
#include "stats/modifier.hpp"
#include "units/view_space.hpp"
#include "utility/id.hpp"
#include "utility/lazy_bounded.hpp"

namespace ql
{
	class being;
	class body_part;

	struct dexterity : tagged_type<double> { using tagged_type::tagged_type; };
	struct vitality : tagged_type<double> { using tagged_type::tagged_type; };

	//! An attachment point on a body part.
	class attachment
	{
	public:
		virtual ~attachment() = default;

		//! The body part currently attached here or nullptr if there is no attached part.
		uptr<body_part> part;

		attachment(uptr<body_part> part) : part{std::move(part)} {}

		//! The offset from the parent part's origin to the child part's origin.
		virtual units::view_space::vector offset() const = 0;

		//! The rotation of the attached part about the attachment point.
		virtual units::view_space::radians rotation() const = 0;

		//! Creates an instance of the body part that attaches here by default.
		//! @param owner_id The ID of the being that owns the new body part.
		virtual uptr<body_part> default_part(ql::id<being> owner_id) const = 0;
	};

	//! A being's body part.
	class body_part : public element<body_part_subtype_list>
	{
	public:
		static constexpr double minimum_vitality = 0.0;

		id<body_part> const id;

		//! Determines maximum health.
		static_bounded<double, minimum_vitality> vitality;

		lazy_bounded<double> health;

		//! The amount this body part is bleeding, in the range [0, vitality].
		lazy_bounded<double> bleeding;

		//! The ID of the item equipped to this body or nullopt if none.
		std::optional<ql::id<item>> equipped_item_id;

		virtual ~body_part() = default;

		//! The player-visisble name of the body part.
		virtual std::string const& name() const = 0;

		//! Whether the body part is vital to its being. If true, the being dies when this body part is disabled.
		virtual bool vital() const = 0;

		//! Stat modifiers to apply to the part's owner.
		virtual std::vector<uptr<modifier>> modifiers() const = 0;

		//! A list of actions that this body part enables its owner to perform.
		virtual std::vector<uptr<action>> abilities() const = 0; //! @todo Is this useful?

		//! The body parts attached to this body part.
		std::vector<uptr<attachment>> const& attachments() const { return _attachments; }

		//! The region this body part occupies, for collision and display.
		virtual units::view_space::polygon const& hitbox() const = 0;

		//! The body part's contribution to its owner's weight.
		double weight() const { return _weight; }

		//! The body part's protection stat.
		virtual dmg::protect const& protection() const { return dmg::protect::zero(); }

		//! The body part's resistance stat.
		virtual dmg::resist const& resistance() const { return dmg::resist::zero(); }

		//! The body part's vulnerability stat.
		virtual dmg::vuln const& vulnerability() const { return dmg::vuln::zero(); }

		//! The draw layer, with smaller-numbered layers drawn first (i.e. in the background).
		virtual int layer() const = 0;

		//! Advances the body part one time unit.
		void update();

		//! Causes the body part to take damage from the specified source being.
		//! @param damage Damage group to be applied to this being.
		//! @param opt_source_id The ID of the being which caused the damage, if any.
		void take_damage(dmg::group& damage, std::optional<ql::id<being>> opt_source_id);

		//! Causes the body part to be healed by the specified source being.
		//! @param amount Health to be restored to this being.
		//! @param opt_source_id The ID of the being which caused the healing, if any.
		void heal(double amount, std::optional<ql::id<being>> opt_source_id);

		//! Recursively generates each missing attached body part using default parts.
		void generate_attached_parts();
	protected:
		//! @param owner_id The id of the being that owns this body part.
		//! @param vitality The body part's vitality, which determines its maximum health.
		//! @param weight The body part's contribution to its owner's weight.
		//! @param attachments The list of attachments to this body part.
		//! @param id The body part's unique ID.
		body_part
			( ql::id<being> owner_id
			, ql::vitality vitality
			, ql::weight weight
			, std::vector<uptr<attachment>> attachments
			, ql::id<body_part> id = ql::id<body_part>::make()
			);
	private:
		ql::id<being> _owner_id;

		std::vector<uptr<attachment>> _attachments;

		bool _enabled;
		double _weight;
	};

	DEFINE_ELEMENT_BASE(body_part, body_part)

	// body_part Subtypes

	class head : public body_part_base<head>
	{
	public:
		head
			( ql::id<being> owner_id
			, ql::vitality vitality
			, ql::weight weight
			, intellect intellect
			, spirit spirit
			, mute mute
			, std::vector<uptr<attachment>> attachments
			)
			: body_part_base<head>
				{ owner_id
				, vitality
				, weight
				, std::move(attachments)
				}
			, _intellect{intellect}
			, _spirit{spirit}
			, _mute{mute}
		{}

		std::string const& name() const override { return "Head"; }

		bool vital() const override { return true; }

		std::vector<uptr<modifier>> modifiers() const override
		{
			return make_uptr_vector<modifier>
				( umake<intellect_modifier>(_intellect)
				, umake<spirit_modifier>(_spirit)
				, umake<mute_modifier>(_mute)
				, umake<weight_modifier>(weight())
				);
		}

		virtual std::vector<uptr<action>> abilities() const { return {}; }
	private:
		double _intellect;
		double _spirit;
		bool _mute;
	};

	class torso : public body_part_base<torso>
	{
	public:
		torso
			( ql::id<being> owner_id
			, ql::vitality vitality
			, ql::weight weight
			, strength strength
			, std::vector<uptr<attachment>> attachments
			)
			: body_part_base<torso>
				{ owner_id
				, vitality
				, weight
				, std::move(attachments)
				}
			, _strength{strength}
		{}

		std::string const& name() const override { return "Torso"; }

		bool vital() const override { return true; }

		std::vector<uptr<modifier>> modifiers() const override
		{
			return make_uptr_vector<modifier>
				( umake<strength_modifier>(_strength)
				, umake<weight_modifier>(weight())
				);
		}

		virtual std::vector<uptr<action>> abilities() const { return {}; }

		double strength() const { return _strength; }
	private:
		double _strength;
	};

	class arm : public body_part_base<arm>
	{
	public:
		arm
			( ql::id<being> owner_id
			, ql::vitality vitality
			, ql::weight weight
			, strength strength
			, std::vector<uptr<attachment>> attachments
			)
			: body_part_base<arm>
				{ owner_id
				, vitality
				, weight
				, std::move(attachments)
				}
			, _strength{strength}
		{}

		std::string const& name() const override { return "Arm"; }

		bool vital() const override { return false; }

		std::vector<uptr<modifier>> modifiers() const override
		{
			return make_uptr_vector<modifier>
				( umake<strength_modifier>(_strength)
				, umake<weight_modifier>(weight())
				);
		}

		virtual std::vector<uptr<action>> abilities() const { return {}; }

		double strength() const { return _strength; }
	private:
		double _strength;
	};

	class hand : public body_part_base<hand>
	{
	public:
		hand
			( ql::id<being> owner_id
			, ql::vitality vitality
			, ql::weight weight
			, dexterity dexterity
			, std::vector<uptr<attachment>> attachments
			)
			: body_part_base<hand>
				{ owner_id
				, vitality
				, weight
				, std::move(attachments)
				}
			, _dexterity{dexterity}
		{}

		std::string const& name() const override { return "Hand"; }

		bool vital() const override { return false; }

		std::vector<uptr<modifier>> modifiers() const override
		{
			return make_uptr_vector<modifier>(umake<weight_modifier>(weight()));
		}

		virtual std::vector<uptr<action>> abilities() const { return {}; }

		double dexterity() const { return _dexterity; }
	private:
		double _dexterity;
	};

	class leg : public body_part_base<leg>
	{
	public:
		leg
			( ql::id<being> owner_id
			, ql::vitality vitality
			, ql::weight weight
			, agility agility
			, strength strength
			, std::vector<uptr<attachment>> attachments
			)
			: body_part_base<leg>
				{ owner_id
				, vitality
				, weight
				, std::move(attachments)
				}
			, _agility{agility}
			, _strength{strength}
		{}

		std::string const& name() const override { return "Leg"; }

		bool vital() const override { return false; }

		std::vector<uptr<modifier>> modifiers() const override
		{
			return make_uptr_vector<modifier>
				( umake<agility_modifier>(_agility)
				, umake<strength_modifier>(_strength)
				, umake<weight_modifier>(weight())
				);
		}

		virtual std::vector<uptr<action>> abilities() const { return {}; }

		double agility() const { return _agility; }
		double strength() const { return _strength; }
	private:
		double _agility;
		double _strength;
	};

	class foot : public body_part_base<foot>
	{
	public:
		foot
			( ql::id<being> owner_id
			, ql::vitality vitality
			, ql::weight weight
			, agility agility
			, std::vector<uptr<attachment>> attachments
			)
			: body_part_base<foot>
				{ owner_id
				, vitality
				, weight
				, std::move(attachments)
				}
			, _agility{agility}
		{}

		std::string const& name() const override { return "Foot"; }

		bool vital() const override { return false; }

		std::vector<uptr<modifier>> modifiers() const override
		{
			return make_uptr_vector<modifier>
				( umake<agility_modifier>(_agility)
				, umake<weight_modifier>(weight())
				);
		}

		virtual std::vector<uptr<action>> abilities() const { return {}; }

		double agility() const { return _agility; }
	private:
		double _agility;
	};

	class wing : public body_part_base<wing>
	{
	public:
		wing
			( ql::id<being> owner_id
			, ql::vitality vitality
			, ql::weight weight
			, std::vector<uptr<attachment>> attachments
			)
			: body_part_base<wing>
				{ owner_id
				, vitality
				, weight
				, std::move(attachments)
				}
			, _weight{weight}
		{}

		std::string const& name() const override { return "Wing"; }

		bool vital() const override { return false; }

		std::vector<uptr<modifier>> modifiers() const override
		{
			return make_uptr_vector<modifier>(umake<weight_modifier>(weight()));
		}

		virtual std::vector<uptr<action>> abilities() const { return {}; }

		double weight() const { return _weight; }
	private:
		double _weight;
	};

	class tail : public body_part_base<tail>
	{
	public:
		using body_part_base<tail>::body_part_base;

		std::string const& name() const override { return "Tail"; }

		bool vital() const override { return false; }

		std::vector<uptr<modifier>> modifiers() const override
		{
			return make_uptr_vector<modifier>(umake<weight_modifier>(weight()));
		}

		virtual std::vector<uptr<action>> abilities() const { return {}; }
	};
}
