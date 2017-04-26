//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>
#include <vector>

#include <boost/iterator/transform_iterator.hpp>

#include "entities/beings/BodyPart.h"
#include "units/ScreenVector.h"

namespace questless
{
	class Being;

	//! A being's body, which is composed of a tree of body parts.
	class Body
	{
	public:
		//! @param owner The being that owns this body.
		//! @param root The root of the body parts tree.
		Body(Being& owner, std::unique_ptr<BodyPart> root);

		Body(Body&&) = default;

		//! The root body part, to which all other body parts are attached.
		BodyPart& root() { return *_root; }

		//! Iterator to beginning of body parts.
		auto begin() { return _parts.begin(); }
		//! Iterator to end of body parts.
		auto end() { return _parts.end(); }

		//! Const iterator to beginning of body parts.
		auto begin() const
		{
			return boost::make_transform_iterator(_parts.begin(), body_part_ref_to_cref);
		}
		//! Const iterator to end of body parts.
		auto end() const
		{
			return boost::make_transform_iterator(_parts.end(), body_part_ref_to_cref);
		}
		
		std::vector<ref<Head>> const& heads() { return _heads; }
		std::vector<ref<Torso>> const& torsos() { return _torsos; }
		std::vector<ref<Arm>> const& arms() { return _arms; }
		std::vector<ref<Hand>> const& hands() { return _hands; }
		std::vector<ref<Leg>> const& legs() { return _legs; }
		std::vector<ref<Foot>> const& feet() { return _feet; }
		std::vector<ref<Wing>> const& wings() { return _wings; }
		std::vector<ref<Tail>> const& tails() { return _tails; }

		//! The body part on this body with the given ID or nullptr if none exists.
		BodyPart* find_part(Id<BodyPart> id);

		//! The head on this body with the given ID or nullptr if none exists.
		Head* find_head(Id<BodyPart> id);

		//! The torso on this body with the given ID or nullptr if none exists.
		Torso* find_torso(Id<BodyPart> id);

		//! The arm on this body with the given ID or nullptr if none exists.
		Arm* find_arm(Id<BodyPart> id);

		//! The hand on this body with the given ID or nullptr if none exists.
		Hand* find_hand(Id<BodyPart> id);

		//! The leg on this body with the given ID or nullptr if none exists.
		Leg* find_leg(Id<BodyPart> id);

		//! The foot on this body with the given ID or nullptr if none exists.
		Foot* find_foot(Id<BodyPart> id);

		//! The wing on this body with the given ID or nullptr if none exists.
		Wing* find_wing(Id<BodyPart> id);

		//! The tail on this body with the given ID or nullptr if none exists.
		Tail* find_tail(Id<BodyPart> id);

		//! The bounding box around the body's parts.
		units::ScreenRect bounds() const { return _bounds; }

		//! The offset from the upper left corner of the bounds to the body's center.
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

		std::vector<ref<BodyPart>> _parts;

		std::vector<ref<Head>> _heads;
		std::vector<ref<Torso>> _torsos;
		std::vector<ref<Arm>> _arms;
		std::vector<ref<Hand>> _hands;
		std::vector<ref<Leg>> _legs;
		std::vector<ref<Foot>> _feet;
		std::vector<ref<Wing>> _wings;
		std::vector<ref<Tail>> _tails;

		static cref<BodyPart> body_part_ref_to_cref(ref<BodyPart> body_part)
		{
			return static_cast<cref<BodyPart>>(body_part);
		}
	};
}
