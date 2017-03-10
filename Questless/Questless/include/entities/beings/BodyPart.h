/**
* @file    BodyPart.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the BodyPart class, which represents a tree of body parts.
*/

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "stats/Modifier.h"
#include "BodyPartVisitor.h"
#include "Damage.h"
#include "items/weapons/Weapon.h"
#include "items/armor/Armor.h"
#include "units/ScreenRect.h"
#include "utility/Property.h"
#include "utility/DynamicProperty.h"
#include "utility/Id.h"

namespace questless
{
	class Being;

	class BodyPart
	{
	public:
		using ptr = std::unique_ptr<BodyPart>;
		using ref = std::reference_wrapper<BodyPart>;
		using cref = std::reference_wrapper<BodyPart const>;

		DynamicProperty<double> health;

		/// @param owner The being that owns this body.
		/// @param name The name of the body part.
		/// @param vitality The body part's vitality, which determines its maximum health.
		/// @param weight The body part's contribution to its owner's weight.
		/// @param protection The body part's protection stat.
		/// @param resistance The body part's resistance stat.
		/// @param vulnerability The body part's vulnerability stat.
		/// @param regions The set of rectangular regions that this body part occupies. Used for display and hit detection.
		BodyPart
			( Being& owner
			, std::string name
			, Vitality vitality
			, Weight weight
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, std::vector<units::ScreenRect> regions
			);

		virtual ~BodyPart() = default;

		virtual void accept(BodyPartVisitor& visitor) = 0;

		/// @return Whether the body part is vital to its being. If true, the being dies when this body part is disabled.
		virtual bool vital() const = 0;

		/// @return Stat modifiers to apply to the part's owner.
		virtual std::vector<Modifier::ptr> modifiers() const = 0;

		/// @return A list of actions that this body part enables its owner to perform.
		virtual std::vector<Action::ptr> abilities() const = 0;

		/// Advances the body part one time unit.
		void update();

		/// Adds the given body part to the list of child parts.
		void attach(ptr child) { _children.push_back(std::move(child)); }

		/// @return The player-visisble name of the body part.
		std::string const& name() const { return _name; }

		/// @return The body part to which this body part is attached.
		std::vector<BodyPart::ptr> const& children() const { return _children; }

		/// @return The set of regions that this body part occupies.
		std::vector<units::ScreenRect> const& regions() const { return _regions; }

		/// @return The body part's vitality, which determines maximum health.
		double vitality() const { return _vitality; }

		/// @return The body part's contribution to its owner's weight.
		double weight() const { return _weight; }

		/// @return The body part's protection stat.
		Protection const& protection() const { return _protection; }

		/// @return The body part's resistance stat.
		Resistance const& resistance() const { return _resistance; }

		/// @return The body part's vulnerability stat.
		Vulnerability const& vulnerability() const { return _vulnerability; }

		/// @return The part's equipped weapons.
		std::vector<Weapon::ref> const& weapons() { return _weapons; }

		/// @return The part's equipped armor.
		std::vector<Armor::ref> const& armor() { return _armor; }

		/// Causes the body part to take damage from the specified source being.
		/// @param damage Damage to be applied to this being.
		/// @param source_id The ID of the being which caused the damage, if any.
		void take_damage(Damage& damage, boost::optional<Id<Being>> source_id);
	private:
		Being& _owner;

		std::string _name;
		std::vector<BodyPart::ptr> _children;
		std::vector<units::ScreenRect> _regions;

		bool _enabled;
		double _vitality;
		double _weight;
		Protection _protection;
		Resistance _resistance;
		Vulnerability _vulnerability;

		std::vector<Weapon::ref> _weapons;
		std::vector<Armor::ref> _armor;

		std::function<void(double&, double const&)> health_mutator();
	};

	// Body part subtypes

	class Head : public virtual BodyPart
	{
	public:
		using ptr = std::unique_ptr<Head>;
		using ref = std::reference_wrapper<Head>;

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
			, std::vector<units::ScreenRect> regions
			)
			: BodyPart
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

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }

		bool vital() const override { return true; }

		std::vector<Modifier::ptr> modifiers() const override
		{
			return Modifier::make_vector
				( std::make_unique<IntellectModifier>(_intellect)
				, std::make_unique<SpiritModifier>(_spirit)
				, std::make_unique<MuteModifier>(false)
				, std::make_unique<WeightModifier>(weight())
				);
		}

		virtual std::vector<Action::ptr> abilities() const { return {}; }
	private:
		double _intellect;
		double _spirit;
	};

	class Torso : public virtual BodyPart
	{
	public:
		using ptr = std::unique_ptr<Torso>;
		using ref = std::reference_wrapper<Torso>;
		
		Torso
			( Being& owner
			, std::string name
			, Vitality vitality
			, Weight weight
			, Strength strength
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, std::vector<units::ScreenRect> regions
			)
			: BodyPart
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

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }

		bool vital() const override { return true; }

		std::vector<Modifier::ptr> modifiers() const override
		{
			return Modifier::make_vector
				( std::make_unique<StrengthModifier>(_strength)
				, std::make_unique<WeightModifier>(weight())
				);
		}

		virtual std::vector<Action::ptr> abilities() const { return{}; }

		double strength() const { return _strength; }
	private:
		double _strength;
	};

	class Arm : public virtual BodyPart
	{
	public:
		using ptr = std::unique_ptr<Arm>;
		using ref = std::reference_wrapper<Arm>;
		
		Arm
			( Being& owner
			, std::string name
			, Vitality vitality
			, Weight weight
			, Strength strength
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, std::vector<units::ScreenRect> regions
			)
			: BodyPart
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

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }

		bool vital() const override { return false; }

		std::vector<Modifier::ptr> modifiers() const override
		{
			return Modifier::make_vector
				( std::make_unique<StrengthModifier>(_strength)
				, std::make_unique<WeightModifier>(weight())
				);
		}

		virtual std::vector<Action::ptr> abilities() const { return{}; }

		double strength() const { return _strength; }
	private:
		double _strength;
	};

	class Hand : public virtual BodyPart
	{
	public:
		using ptr = std::unique_ptr<Hand>;
		using ref = std::reference_wrapper<Hand>;

		Hand
			( Being& owner
			, std::string name
			, Vitality vitality
			, Weight weight
			, Dexterity dexterity
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, std::vector<units::ScreenRect> regions
			)
			: BodyPart
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

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }

		bool vital() const override { return false; }

		std::vector<Modifier::ptr> modifiers() const override
		{
			return Modifier::make_vector
				( std::make_unique<DexterityModifier>(_dexterity)
				, std::make_unique<WeightModifier>(weight())
			);
		}

		virtual std::vector<Action::ptr> abilities() const { return{}; }

		double dexterity() const { return _dexterity; }
	private:
		double _dexterity;
	};

	class Leg : public virtual BodyPart
	{
	public:
		using ptr = std::unique_ptr<Leg>;
		using ref = std::reference_wrapper<Leg>;

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
			, std::vector<units::ScreenRect> regions
			)
			: BodyPart
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

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }

		bool vital() const override { return false; }

		std::vector<Modifier::ptr> modifiers() const override
		{
			return Modifier::make_vector
				( std::make_unique<AgilityModifier>(_agility)
				, std::make_unique<StrengthModifier>(_strength)
				, std::make_unique<WeightModifier>(weight())
				);
		}

		virtual std::vector<Action::ptr> abilities() const { return{}; }

		double agility() const { return _agility; }
		double strength() const { return _strength; }
	private:
		double _agility;
		double _strength;
	};

	class Foot : public virtual BodyPart
	{
	public:
		using ptr = std::unique_ptr<Foot>;
		using ref = std::reference_wrapper<Foot>;
		
		Foot
			( Being& owner
			, std::string name
			, Vitality vitality
			, Weight weight
			, Agility agility
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, std::vector<units::ScreenRect> regions
			)
			: BodyPart
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

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }

		bool vital() const override { return false; }

		std::vector<Modifier::ptr> modifiers() const override
		{
			return Modifier::make_vector
				( std::make_unique<AgilityModifier>(_agility)
				, std::make_unique<WeightModifier>(weight())
				);
		}

		virtual std::vector<Action::ptr> abilities() const { return{}; }

		double agility() const { return _agility; }
	private:
		double _agility;
	};

	class Wing : public virtual BodyPart
	{
	public:
		using ptr = std::unique_ptr<Wing>;
		using ref = std::reference_wrapper<Wing>;
		
		Wing
			( Being& owner
			, std::string name
			, Vitality vitality
			, Weight weight
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, std::vector<units::ScreenRect> regions
			)
			: BodyPart
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

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }

		bool vital() const override { return false; }

		std::vector<Modifier::ptr> modifiers() const override
		{
			return Modifier::make_vector(std::make_unique<WeightModifier>(weight()));
		}

		virtual std::vector<Action::ptr> abilities() const { return{}; }

		double weight() const { return _weight; }
	private:
		double _weight;
	};

	class Tail : public virtual BodyPart
	{
	public:
		using ptr = std::unique_ptr<Tail>;
		using ref = std::reference_wrapper<Tail>;

		using BodyPart::BodyPart;

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }

		bool vital() const override { return false; }

		std::vector<Modifier::ptr> modifiers() const override
		{
			return Modifier::make_vector(std::make_unique<WeightModifier>(weight()));
		}

		virtual std::vector<Action::ptr> abilities() const { return{}; }
	};
}
