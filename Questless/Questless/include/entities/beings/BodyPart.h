//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "BodyPartVisitor.h"
#include "Damage.h"
#include "items/Item.h"
#include "stats/Modifier.h"
#include "units/ScreenSpace.h"
#include "utility/DynamicProperty.h"
#include "utility/Id.h"

namespace questless
{
	class Being;

	//! A being's body part.
	class BodyPart : public Element<BodyPartSubtypeList>
	{
	public:
		Id<BodyPart> const id;

		DynamicProperty<double> health;

		//! The ID of the item equipped to this body or nullopt if none.
		std::optional<Id<Item>> equipped_item_id;

		virtual ~BodyPart() = default;

		//! Whether the body part is vital to its being. If true, the being dies when this body part is disabled.
		virtual bool vital() const = 0;

		//! Stat modifiers to apply to the part's owner.
		virtual std::vector<uptr<Modifier>> modifiers() const = 0;

		//! A list of actions that this body part enables its owner to perform.
		virtual std::vector<uptr<Action>> abilities() const = 0; //! @todo Is this useful?

		//! Advances the body part one time unit.
		void update();

		//! Adds the given body part to the list of child parts.
		void attach(uptr<BodyPart> child) { _children.push_back(std::move(child)); }

		//! The player-visisble name of the body part.
		std::string const& name() const { return _name; }

		//! The body parts attached to this body part.
		std::vector<uptr<BodyPart>> const& children() const { return _children; }

		//! The set of regions that this body part occupies.
		std::vector<units::ScreenSpace::Box> const& regions() const { return _regions; }

		//! The body part's vitality, which determines maximum health.
		double vitality() const { return _vitality; }

		//! The body part's contribution to its owner's weight.
		double weight() const { return _weight; }

		//! The body part's protection stat.
		Protection const& protection() const { return _protection; }

		//! The body part's resistance stat.
		Resistance const& resistance() const { return _resistance; }

		//! The body part's vulnerability stat.
		Vulnerability const& vulnerability() const { return _vulnerability; }

		//! Causes the body part to take damage from the specified source being.
		//! @param damage Damage to be applied to this being.
		//! @param source_id The ID of the being which caused the damage, if any.
		void take_damage(Damage& damage, std::optional<Id<Being>> source_id);
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
		BodyPart
			( Being& owner
			, std::string name
			, Vitality vitality
			, Weight weight
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, std::vector<units::ScreenSpace::Box> regions
			, Id<BodyPart> id = Id<BodyPart>::make()
			);
	private:
		Being& _owner;

		std::string _name;
		std::vector<uptr<BodyPart>> _children;
		std::vector<units::ScreenSpace::Box> _regions;

		bool _enabled;
		double _vitality;
		double _weight;
		Protection _protection;
		Resistance _resistance;
		Vulnerability _vulnerability;

		std::function<void(double&, double const&)> health_mutator();
	};

	DEFINE_ELEMENT_BASE(BodyPart, BodyPart)

	// BodyPart Subtypes

	class Head : public BodyPartBase<Head>
	{
	public:
		Head
			( Being& owner
			, std::string name
			, Vitality vitality
			, Weight weight
			, Intellect intellect
			, Spirit spirit
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, std::vector<units::ScreenSpace::Box> regions
			)
			: BodyPartBase<Head>
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

		std::vector<uptr<Modifier>> modifiers() const override
		{
			return Modifier::make_vector
				( std::make_unique<IntellectModifier>(_intellect)
				, std::make_unique<SpiritModifier>(_spirit)
				, std::make_unique<MuteModifier>(false)
				, std::make_unique<WeightModifier>(weight())
				);
		}

		virtual std::vector<uptr<Action>> abilities() const { return {}; }
	private:
		double _intellect;
		double _spirit;
	};

	class Torso : public BodyPartBase<Torso>
	{
	public:
		Torso
			( Being& owner
			, std::string name
			, Vitality vitality
			, Weight weight
			, Strength strength
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, std::vector<units::ScreenSpace::Box> regions
			)
			: BodyPartBase<Torso>
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

		std::vector<uptr<Modifier>> modifiers() const override
		{
			return Modifier::make_vector
				( std::make_unique<StrengthModifier>(_strength)
				, std::make_unique<WeightModifier>(weight())
				);
		}

		virtual std::vector<uptr<Action>> abilities() const { return {}; }

		double strength() const { return _strength; }
	private:
		double _strength;
	};

	class Arm : public BodyPartBase<Arm>
	{
	public:
		Arm
			( Being& owner
			, std::string name
			, Vitality vitality
			, Weight weight
			, Strength strength
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, std::vector<units::ScreenSpace::Box> regions
			)
			: BodyPartBase<Arm>
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

		std::vector<uptr<Modifier>> modifiers() const override
		{
			return Modifier::make_vector
				( std::make_unique<StrengthModifier>(_strength)
				, std::make_unique<WeightModifier>(weight())
				);
		}

		virtual std::vector<uptr<Action>> abilities() const { return {}; }

		double strength() const { return _strength; }
	private:
		double _strength;
	};

	class Hand : public BodyPartBase<Hand>
	{
	public:
		Hand
			( Being& owner
			, std::string name
			, Vitality vitality
			, Weight weight
			, Dexterity dexterity
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, std::vector<units::ScreenSpace::Box> regions
			)
			: BodyPartBase<Hand>
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

		std::vector<uptr<Modifier>> modifiers() const override
		{
			return Modifier::make_vector
				( std::make_unique<DexterityModifier>(_dexterity)
				, std::make_unique<WeightModifier>(weight())
			);
		}

		virtual std::vector<uptr<Action>> abilities() const { return {}; }

		double dexterity() const { return _dexterity; }
	private:
		double _dexterity;
	};

	class Leg : public BodyPartBase<Leg>
	{
	public:
		Leg
			( Being& owner
			, std::string name
			, Vitality vitality
			, Weight weight
			, Agility agility
			, Strength strength
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, std::vector<units::ScreenSpace::Box> regions
			)
			: BodyPartBase<Leg>
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

		std::vector<uptr<Modifier>> modifiers() const override
		{
			return Modifier::make_vector
				( std::make_unique<AgilityModifier>(_agility)
				, std::make_unique<StrengthModifier>(_strength)
				, std::make_unique<WeightModifier>(weight())
				);
		}

		virtual std::vector<uptr<Action>> abilities() const { return {}; }

		double agility() const { return _agility; }
		double strength() const { return _strength; }
	private:
		double _agility;
		double _strength;
	};

	class Foot : public BodyPartBase<Foot>
	{
	public:
		Foot
			( Being& owner
			, std::string name
			, Vitality vitality
			, Weight weight
			, Agility agility
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, std::vector<units::ScreenSpace::Box> regions
			)
			: BodyPartBase<Foot>
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

		std::vector<uptr<Modifier>> modifiers() const override
		{
			return Modifier::make_vector
				( std::make_unique<AgilityModifier>(_agility)
				, std::make_unique<WeightModifier>(weight())
				);
		}

		virtual std::vector<uptr<Action>> abilities() const { return {}; }

		double agility() const { return _agility; }
	private:
		double _agility;
	};

	class Wing : public BodyPartBase<Wing>
	{
	public:
		Wing
			( Being& owner
			, std::string name
			, Vitality vitality
			, Weight weight
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, std::vector<units::ScreenSpace::Box> regions
			)
			: BodyPartBase<Wing>
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

		std::vector<uptr<Modifier>> modifiers() const override
		{
			return Modifier::make_vector(std::make_unique<WeightModifier>(weight()));
		}

		virtual std::vector<uptr<Action>> abilities() const { return {}; }

		double weight() const { return _weight; }
	private:
		double _weight;
	};

	class Tail : public BodyPartBase<Tail>
	{
	public:
		using BodyPartBase<Tail>::BodyPartBase;

		bool vital() const override { return false; }

		std::vector<uptr<Modifier>> modifiers() const override
		{
			return Modifier::make_vector(std::make_unique<WeightModifier>(weight()));
		}

		virtual std::vector<uptr<Action>> abilities() const { return {}; }
	};
}
