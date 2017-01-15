/**
* @file    BodyPart.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the BodyPart class, which represents a tree of body parts.
*/

#ifndef BODY_PART_H
#define BODY_PART_H

#include <string>
#include <vector>
#include <memory>

#include "attributes/Modifier.h"
#include "BodyPartVisitor.h"
#include "Damage.h"
#include "items/weapons/Weapon.h"
#include "items/armor/Armor.h"
#include "BeingId.h"
#include "units/ScreenRect.h"

namespace questless
{
	class Being;

	class BodyPart
	{
	public:
		using ptr = std::unique_ptr<BodyPart>;
		using ref = std::reference_wrapper<BodyPart>;

		/// @param owner The being that owns this body.
		/// @param name The name of the body part.
		/// @param modifiers Attribute modifiers to apply to the part's owner.
		/// @param vitality The body part's vitality, which determines its maximum health.
		/// @param protection The body part's protection attribute.
		/// @param resistance The body part's resistance attribute.
		/// @param vulnerability The body part's vulnerability attribute.
		/// @param vital Whether the body part is vital to its being. If true, the being dies when this body part is disabled.
		/// @param regions The set of rectangular regions that this body part occupies. Used for display and hit detection.
		BodyPart
			( Being& owner
			, std::string name
			, std::vector<Modifier::ptr> modifiers
			, double vitality
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, bool vital
			, std::vector<units::ScreenRect> regions
			);

		virtual ~BodyPart() = default;

		virtual void accept(BodyPartVisitor& visitor) = 0;

		/// Advances the body part one time unit.
		void update();

		/// Adds the given body part to the list of child parts.
		void attach(ptr child) { _children.push_back(std::move(child)); }

		/// @return The player-visisble name of the body part.
		const std::string& name() const { return _name; }

		/// @return The body part to which this body part is attached.
		const std::vector<BodyPart::ptr>& children() const { return _children; }

		/// @return The set of regions that this body part occupies.
		const std::vector<units::ScreenRect>& regions() const { return _regions; }

		/// @return Attribute modifiers to apply to the part's owner.
		const std::vector<Modifier::ptr>& modifiers() const { return _modifiers; }

		/// @return The body part's current health.
		double health() const { return _health; }

		void gain_health(double amount);
		void lose_health(double amount);

		/// @return The body part's vitality, which determines maximum health.
		double vitality() const { return _vitality; }

		/// @return The body part's protection attribute.
		const Protection& protection() const { return _protection; }

		/// @return The body part's resistance attribute.
		const Resistance& resistance() const { return _resistance; }

		/// @return The body part's vulnerability attribute.
		const Vulnerability& vulnerability() const { return _vulnerability; }

		/// @return Whether the body part is vital to its being. If true, the being dies when this body part is disabled.
		bool vital() const { return _vital; }

		/// @return The part's equipped weapons.
		const std::vector<Weapon::ref>& weapons() { return _weapons; }

		/// @return The part's equipped armor.
		const std::vector<Armor::ref>& armor() { return _armor; }

		/// Causes the body part to take damage from the specified source being.
		/// @param damage Damage to be applied to this being.
		/// @param source_id The ID of the being which caused the damage, if any.
		void take_damage(Damage& damage, boost::optional<BeingId> source_id);
	private:
		Being& _owner;

		std::string _name;
		std::vector<BodyPart::ptr> _children;
		std::vector<units::ScreenRect> _regions;

		std::vector<Modifier::ptr> _modifiers;

		/// @todo Group these into a BodyPartAttributes struct or something.

		double _health;
		double _vitality;
		Protection _protection;
		Resistance _resistance;
		Vulnerability _vulnerability;
		const bool _vital;

		std::vector<Weapon::ref> _weapons;
		std::vector<Armor::ref> _armor;
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
			, double intellect
			, double vitality
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, bool vital
			, std::vector<units::ScreenRect> regions
			)
			: BodyPart
				{ owner
				, std::move(name)
				, Modifier::make_vector(std::make_unique<IntellectModifier>(intellect))
				, vitality
				, protection
				, resistance
				, vulnerability
				, vital
				, std::move(regions)
				}
			, _intellect{intellect}
		{}

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }
	private:
		double _intellect;
	};

	class Torso : public virtual BodyPart
	{
	public:
		using ptr = std::unique_ptr<Torso>;
		using ref = std::reference_wrapper<Torso>;
		
		Torso
			( Being& owner
			, std::string name
			, double strength
			, double vitality
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, bool vital
			, std::vector<units::ScreenRect> regions
			)
			: BodyPart
				{ owner
				, std::move(name)
				, Modifier::make_vector(std::make_unique<StrengthModifier>(strength))
				, vitality
				, protection
				, resistance
				, vulnerability
				, vital
				, std::move(regions)
				}
			, _strength{strength}
		{}

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }

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
			, double strength
			, double vitality
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, bool vital
			, std::vector<units::ScreenRect> regions
			)
			: BodyPart
				{ owner
				, std::move(name)
				, Modifier::make_vector(std::make_unique<StrengthModifier>(strength))
				, vitality
				, protection
				, resistance
				, vulnerability
				, vital
				, std::move(regions)
				}
			, _strength{strength}
		{}

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }

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
			, double dexterity
			, double vitality
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, bool vital
			, std::vector<units::ScreenRect> regions
			)
			: BodyPart
				{ owner
				, std::move(name)
				, Modifier::make_vector(std::make_unique<DexterityModifier>(dexterity))
				, vitality
				, protection
				, resistance
				, vulnerability
				, vital
				, std::move(regions)
				}
			, _dexterity{dexterity}
		{}

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }
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
			, double agility
			, double strength
			, double vitality
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, bool vital
			, std::vector<units::ScreenRect> regions
			)
			: BodyPart
				{ owner
				, std::move(name)
				, Modifier::make_vector(std::make_unique<AgilityModifier>(agility), std::make_unique<StrengthModifier>(strength))
				, vitality
				, protection
				, resistance
				, vulnerability
				, vital
				, std::move(regions)
				}
			, _agility{agility}
			, _strength{strength}
		{}

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }

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
			, double agility
			, double vitality
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, bool vital
			, std::vector<units::ScreenRect> regions
			)
			: BodyPart
				{ owner
				, std::move(name)
				, Modifier::make_vector(std::make_unique<AgilityModifier>(agility))
				, vitality
				, protection
				, resistance
				, vulnerability
				, vital
				, std::move(regions)
				}
			, _agility{agility}
		{}

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }

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
			, double lift
			, double vitality
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, bool vital
			, std::vector<units::ScreenRect> regions
			)
			: BodyPart
				{ owner
				, std::move(name)
				, Modifier::make_vector(std::make_unique<LiftModifier>(lift))
				, vitality
				, protection
				, resistance
				, vulnerability
				, vital
				, std::move(regions)
				}
			, _lift{lift}
		{}

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }

		double lift() const { return _lift; }
	private:
		double _lift;
	};

	class Tail : public virtual BodyPart
	{
	public:
		using ptr = std::unique_ptr<Tail>;
		using ref = std::reference_wrapper<Tail>;

		using BodyPart::BodyPart;

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }
	};
}

#endif
