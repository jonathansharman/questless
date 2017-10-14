//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "body_part_visitor.h"
#include "damage/group.h"
#include "items/item.h"
#include "stats/modifier.h"
#include "units/screen_space.h"
#include "utility/dynamic_property.h"
#include "utility/id.h"

namespace ql
{
	class being;

	//! A being's body part.
	class body_part : public element<body_part_subtype_list>
	{
	public:
		id<body_part> const id;

		dynamic_property<double> health;

		//! The ID of the item equipped to this body or nullopt if none.
		std::optional<ql::id<item>> equipped_item_id;

		virtual ~body_part() = default;

		//! Whether the body part is vital to its being. If true, the being dies when this body part is disabled.
		virtual bool vital() const = 0;

		//! Stat modifiers to apply to the part's owner.
		virtual std::vector<uptr<modifier>> modifiers() const = 0;

		//! A list of actions that this body part enables its owner to perform.
		virtual std::vector<uptr<action>> abilities() const = 0; //! @todo Is this useful?

		//! Advances the body part one time unit.
		void update();

		//! Adds the given body part to the list of child parts.
		void attach(uptr<body_part> child) { _children.push_back(std::move(child)); }

		//! The player-visisble name of the body part.
		std::string const& name() const { return _name; }

		//! The body parts attached to this body part.
		std::vector<uptr<body_part>> const& children() const { return _children; }

		//! The set of regions that this body part occupies.
		std::vector<units::screen_space::box> const& regions() const { return _regions; }

		//! The body part's vitality, which determines maximum health.
		double vitality() const { return _vitality; }

		//! The body part's contribution to its owner's weight.
		double weight() const { return _weight; }

		//! The body part's protection stat.
		dmg::protect const& protection() const { return _protection; }

		//! The body part's resistance stat.
		dmg::resist const& resistance() const { return _resistance; }

		//! The body part's vulnerability stat.
		dmg::vuln const& vulnerability() const { return _vulnerability; }

		//! Causes the body part to take damage from the specified source being.
		//! @param damage Damage group to be applied to this being.
		//! @param source_id The ID of the being which caused the damage, if any.
		void take_damage(dmg::group& damage, std::optional<ql::id<being>> source_id);
	protected:
		//! @param owner The being that owns this body.
		//! @param name The name of the body part.
		//! @param vitality The body part's vitality, which determines its maximum health.
		//! @param weight The body part's contribution to its owner's weight.
		//! @param protection The body part's protection stat.
		//! @param resistance The body part's resistance stat.
		//! @param vulnerability The body part's vulnerability stat.
		//! @param regions The set of rectangular regions that this body part occupies. Used for display and hit detection.
		//! @param id The body part's unique ID.
		body_part
			( being& owner
			, std::string name
			, ql::vitality vitality
			, ql::weight weight
			, dmg::protect protection
			, dmg::resist resistance
			, dmg::vuln vulnerability
			, std::vector<units::screen_space::box> regions
			, ql::id<body_part> id = ql::id<body_part>::make()
			);
	private:
		being& _owner;

		std::string _name;
		std::vector<uptr<body_part>> _children;
		std::vector<units::screen_space::box> _regions;

		bool _enabled;
		double _vitality;
		double _weight;
		dmg::protect _protection;
		dmg::resist _resistance;
		dmg::vuln _vulnerability;

		std::function<void(double&, double const&)> health_mutator();
	};

	DEFINE_ELEMENT_BASE(body_part, body_part)

	// body_part Subtypes

	class head : public body_part_base<head>
	{
	public:
		head
			( being& owner
			, std::string name
			, ql::vitality vitality
			, ql::weight weight
			, intellect intellect
			, spirit spirit
			, dmg::protect protection
			, dmg::resist resistance
			, dmg::vuln vulnerability
			, std::vector<units::screen_space::box> regions
			)
			: body_part_base<head>
				{ owner
				, std::move(name)
				, vitality
				, weight
				, protection
				, resistance
				, vulnerability
				, std::move(regions)
				}
			, _intellect{intellect}
			, _spirit{spirit}
		{}

		bool vital() const override { return true; }

		std::vector<uptr<modifier>> modifiers() const override
		{
			return modifier::make_vector
				( std::make_unique<intellect_modifier>(_intellect)
				, std::make_unique<spirit_modifier>(_spirit)
				, std::make_unique<mute_modifier>(false)
				, std::make_unique<weight_modifier>(weight())
				);
		}

		virtual std::vector<uptr<action>> abilities() const { return {}; }
	private:
		double _intellect;
		double _spirit;
	};

	class torso : public body_part_base<torso>
	{
	public:
		torso
			( being& owner
			, std::string name
			, ql::vitality vitality
			, ql::weight weight
			, strength strength
			, dmg::protect protection
			, dmg::resist resistance
			, dmg::vuln vulnerability
			, std::vector<units::screen_space::box> regions
			)
			: body_part_base<torso>
				{ owner
				, std::move(name)
				, vitality
				, weight
				, protection
				, resistance
				, vulnerability
				, std::move(regions)
				}
			, _strength{strength}
		{}

		bool vital() const override { return true; }

		std::vector<uptr<modifier>> modifiers() const override
		{
			return modifier::make_vector
				( std::make_unique<strength_modifier>(_strength)
				, std::make_unique<weight_modifier>(weight())
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
			( being& owner
			, std::string name
			, ql::vitality vitality
			, ql::weight weight
			, strength strength
			, dmg::protect protection
			, dmg::resist resistance
			, dmg::vuln vulnerability
			, std::vector<units::screen_space::box> regions
			)
			: body_part_base<arm>
				{ owner
				, std::move(name)
				, vitality
				, weight
				, protection
				, resistance
				, vulnerability
				, std::move(regions)
				}
			, _strength{strength}
		{}

		bool vital() const override { return false; }

		std::vector<uptr<modifier>> modifiers() const override
		{
			return modifier::make_vector
				( std::make_unique<strength_modifier>(_strength)
				, std::make_unique<weight_modifier>(weight())
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
			( being& owner
			, std::string name
			, ql::vitality vitality
			, ql::weight weight
			, dexterity dexterity
			, dmg::protect protection
			, dmg::resist resistance
			, dmg::vuln vulnerability
			, std::vector<units::screen_space::box> regions
			)
			: body_part_base<hand>
				{ owner
				, std::move(name)
				, vitality
				, weight
				, protection
				, resistance
				, vulnerability
				, std::move(regions)
				}
			, _dexterity{dexterity}
		{}

		bool vital() const override { return false; }

		std::vector<uptr<modifier>> modifiers() const override
		{
			return modifier::make_vector
				( std::make_unique<dexterity_modifier>(_dexterity)
				, std::make_unique<weight_modifier>(weight())
			);
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
			( being& owner
			, std::string name
			, ql::vitality vitality
			, ql::weight weight
			, agility agility
			, strength strength
			, dmg::protect protection
			, dmg::resist resistance
			, dmg::vuln vulnerability
			, std::vector<units::screen_space::box> regions
			)
			: body_part_base<leg>
				{ owner
				, std::move(name)
				, vitality
				, weight
				, protection
				, resistance
				, vulnerability
				, std::move(regions)
				}
			, _agility{agility}
			, _strength{strength}
		{}

		bool vital() const override { return false; }

		std::vector<uptr<modifier>> modifiers() const override
		{
			return modifier::make_vector
				( std::make_unique<agility_modifier>(_agility)
				, std::make_unique<strength_modifier>(_strength)
				, std::make_unique<weight_modifier>(weight())
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
			( being& owner
			, std::string name
			, ql::vitality vitality
			, ql::weight weight
			, agility agility
			, dmg::protect protection
			, dmg::resist resistance
			, dmg::vuln vulnerability
			, std::vector<units::screen_space::box> regions
			)
			: body_part_base<foot>
				{ owner
				, std::move(name)
				, vitality
				, weight
				, protection
				, resistance
				, vulnerability
				, std::move(regions)
				}
			, _agility{agility}
		{}

		bool vital() const override { return false; }

		std::vector<uptr<modifier>> modifiers() const override
		{
			return modifier::make_vector
				( std::make_unique<agility_modifier>(_agility)
				, std::make_unique<weight_modifier>(weight())
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
			( being& owner
			, std::string name
			, ql::vitality vitality
			, ql::weight weight
			, dmg::protect protection
			, dmg::resist resistance
			, dmg::vuln vulnerability
			, std::vector<units::screen_space::box> regions
			)
			: body_part_base<wing>
				{ owner
				, std::move(name)
				, vitality
				, weight
				, protection
				, resistance
				, vulnerability
				, std::move(regions)
				}
			, _weight{weight}
		{}

		bool vital() const override { return false; }

		std::vector<uptr<modifier>> modifiers() const override
		{
			return modifier::make_vector(std::make_unique<weight_modifier>(weight()));
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

		bool vital() const override { return false; }

		std::vector<uptr<modifier>> modifiers() const override
		{
			return modifier::make_vector(std::make_unique<weight_modifier>(weight()));
		}

		virtual std::vector<uptr<action>> abilities() const { return {}; }
	};
}
