/**
* @file    Body.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Body class, which represents a being's body. Holds a BodyPart tree.
*/

#pragma once

#include <memory>
#include <vector>

#include <boost/iterator/transform_iterator.hpp>

#include "entities/beings/BodyPart.h"
#include "units/ScreenVector.h"

namespace questless
{
	class Being;

	class Body
	{
	public:
		using uptr = std::unique_ptr<Body>;

		/// @param owner The being that owns this body.
		/// @param root The root of the body parts tree.
		Body(Being& owner, std::unique_ptr<BodyPart> root);

		Body(Body&&) = default;

		/// @return The root body part, to which all other body parts are attached.
		BodyPart& root() { return *_root; }

		/// @return Iterator to beginning of body parts.
		auto begin() { return _parts.begin(); }
		/// @return Iterator to end of body parts.
		auto end() { return _parts.end(); }

		/// @return Const iterator to beginning of body parts.
		auto begin() const
		{
			return boost::make_transform_iterator(_parts.begin(), body_part_ref_to_cref);
		}
		/// @return Const iterator to end of body parts.
		auto end() const
		{
			return boost::make_transform_iterator(_parts.end(), body_part_ref_to_cref);
		}
		
		std::vector<Head::ref> const& heads() { return _heads; }
		std::vector<Torso::ref> const& torsos() { return _torsos; }
		std::vector<Arm::ref> const& arms() { return _arms; }
		std::vector<Hand::ref> const& hands() { return _hands; }
		std::vector<Leg::ref> const& legs() { return _legs; }
		std::vector<Foot::ref> const& feet() { return _feet; }
		std::vector<Wing::ref> const& wings() { return _wings; }
		std::vector<Tail::ref> const& tails() { return _tails; }

		/// @return The body part on this body with the given ID or nullptr if none exists.
		////
		BodyPart* find_part(Id<BodyPart> id);

		/// @return The head on this body with the given ID or nullptr if none exists.
		////
		Head* find_head(Id<BodyPart> id);

		/// @return The torso on this body with the given ID or nullptr if none exists.
		////
		Torso* find_torso(Id<BodyPart> id);

		/// @return The arm on this body with the given ID or nullptr if none exists.
		////
		Arm* find_arm(Id<BodyPart> id);

		/// @return The hand on this body with the given ID or nullptr if none exists.
		////
		Hand* find_hand(Id<BodyPart> id);

		/// @return The leg on this body with the given ID or nullptr if none exists.
		////
		Leg* find_leg(Id<BodyPart> id);

		/// @return The foot on this body with the given ID or nullptr if none exists.
		////
		Foot* find_foot(Id<BodyPart> id);

		/// @return The wing on this body with the given ID or nullptr if none exists.
		////
		Wing* find_wing(Id<BodyPart> id);

		/// @return The tail on this body with the given ID or nullptr if none exists.
		////
		Tail* find_tail(Id<BodyPart> id);

		/// @return The bounding box around the body's parts.
		units::ScreenRect bounds() const { return _bounds; }

		/// @return The offset from the upper left corner of the bounds to the body's center.
		units::ScreenVector offset_to_center() const { return _offset_to_center; }
	private:
		class PartAttacher : public BodyPartVisitor
		{
		public:
			PartAttacher(Body& body) : _body{body} {}

			void visit(Head&) override;
			void visit(Torso&) override;
			void visit(Arm&) override;
			void visit(Hand&) override;
			void visit(Leg&) override;
			void visit(Foot&) override;
			void visit(Wing&) override;
			void visit(Tail&) override;
		private:
			Body& _body;
		};
		friend PartAttacher;

		Being& _owner;

		std::unique_ptr<BodyPart> _root;

		units::ScreenRect _bounds;
		units::ScreenVector _offset_to_center;

		std::vector<BodyPart::ref> _parts;

		std::vector<Head::ref> _heads;
		std::vector<Torso::ref> _torsos;
		std::vector<Arm::ref> _arms;
		std::vector<Hand::ref> _hands;
		std::vector<Leg::ref> _legs;
		std::vector<Foot::ref> _feet;
		std::vector<Wing::ref> _wings;
		std::vector<Tail::ref> _tails;

		static BodyPart::cref body_part_ref_to_cref(BodyPart::ref body_part)
		{
			return static_cast<BodyPart::cref>(body_part);
		}
	};
}
