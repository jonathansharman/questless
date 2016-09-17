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

#include "sdl-wrappers/basic-sdl-wrappers.h"
#include "attributes/Attributes.h" /// @todo Probably remove this eventually. (BodyParts shouldn't need the full attributes struct.)
#include "BodyPartVisitor.h"

namespace questless
{
	class BodyPart
	{
	public:
		using ptr = std::unique_ptr<BodyPart>;
		using ref = std::reference_wrapper<BodyPart>;

		/// @param name The name of the body part.
		/// @param vitality The body part's vitality, which determines its maximum health.
		/// @param attributes The body part's attributes, which are added to its owner's attributes.
		/// @param vital Whether the body part is vital to its being. If true, the being dies when this body part is disabled.
		/// @param regions The set of rectangular regions that this body part occupies. Used for display and hit detection.
		BodyPart(std::string name, double vitality, Attributes attributes, bool vital, std::vector<sdl::Rect> regions);

		virtual ~BodyPart() = 0 {}; /// @todo Change to default if actual pure virtuals are added to BodyPart later.

		virtual void accept(BodyPartVisitor& visitor) = 0;

		/// @param name The name of the body part.
		/// @param vitality The body part's vitality, which determines its maximum health.
		/// @param attributes The body part's attributes, which are added to its owner's attributes.
		/// @param vital Whether the body part is vital to its being. If true, the being dies when this body part is disabled.
		/// @param regions The set of rectangular regions that this body part occupies. Used for display and hit detection.
		/// @return A BodyPart pointer from the given data.
		template <typename Type>
		static ptr make(std::string name, double vitality, Attributes attributes, bool vital, std::vector<sdl::Rect> regions)
		{
			return std::make_unique<Type>(std::move(name), vitality, attributes, vital, std::move(regions));
		}

		/// Adds the given body part to the list of child parts.
		void attach(ptr child) { _children.push_back(std::move(child)); }

		/// @return The player-visisble name of the body part.
		const std::string& name() const { return _name; }

		/// @return The body part to which this body part is attached.
		const std::vector<BodyPart::ptr>& children() const { return _children; }

		/// @return The body part's attributes, which are added to its owner's attributes.
		const Attributes& attributes() const { return _attributes; }

		/// @return Whether the body part is vital to its being. If true, the being dies when this body part is disabled.
		bool vital() const { return _vital; }

		/// @return The set of regions that this body part occupies.
		const std::vector<sdl::Rect>& regions() const { return _regions; }

		/// @return The body part's current health.
		double health() const { return _health; }

		/// @return The body part's vitality.
		double vitality() const { return _vitality; }

		/// @todo The following method is temporary and for testing purposes only.
		/// Deals the given damage to the part.
		void take_damage(double amount) { _health -= amount; }
	private:
		std::string _name;
		std::vector<BodyPart::ptr> _children;
		Attributes _attributes;
		std::vector<sdl::Rect> _regions;
		double _health;
		double _vitality;
		const bool _vital;
	};

	// Body part subtypes

	class Torso : public virtual BodyPart
	{
	public:
		using ptr = std::unique_ptr<Torso>;
		using ref = std::reference_wrapper<Torso>;

		using BodyPart::BodyPart;

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }
	};

	class Head : public virtual BodyPart
	{
	public:
		using ptr = std::unique_ptr<Head>;
		using ref = std::reference_wrapper<Head>;

		using BodyPart::BodyPart;

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }
	};

	class Arm : public virtual BodyPart
	{
	public:
		using ptr = std::unique_ptr<Arm>;
		using ref = std::reference_wrapper<Arm>;

		using BodyPart::BodyPart;

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }
	};

	class Hand : public virtual BodyPart
	{
	public:
		using ptr = std::unique_ptr<Hand>;
		using ref = std::reference_wrapper<Hand>;

		using BodyPart::BodyPart;

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }
	};

	class Leg : public virtual BodyPart
	{
	public:
		using ptr = std::unique_ptr<Leg>;
		using ref = std::reference_wrapper<Leg>;

		using BodyPart::BodyPart;

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }
	};

	class Foot : public virtual BodyPart
	{
	public:
		using ptr = std::unique_ptr<Foot>;
		using ref = std::reference_wrapper<Foot>;

		using BodyPart::BodyPart;

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }
	};

	class Wing : public virtual BodyPart
	{
	public:
		using ptr = std::unique_ptr<Wing>;
		using ref = std::reference_wrapper<Wing>;

		using BodyPart::BodyPart;

		void accept(BodyPartVisitor& visitor) override { visitor.visit(*this); }
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
