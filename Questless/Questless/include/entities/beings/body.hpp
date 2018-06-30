//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>
#include <vector>

#include <range/v3/all.hpp>

#include "entities/beings/body_part.hpp"
#include "units/window_space.hpp"
#include "utility/utility.hpp"

namespace ql
{
	class being;

	//! A being's body, which is composed of a tree of body parts.
	class body
	{
	public:
		//! The amount of blood in this body.
		lazy_bounded<double> blood;

		//! @param owner The being that owns this body.
		//! @param root The root of the body parts tree.
		body(being& owner, uptr<body_part> root);

		body(body&&) = default;

		//! The root body part, to which all other body parts are attached.
		body_part const& root() const { return *_root; }
		//! The root body part, to which all other body parts are attached.
		body_part& root() { return *_root; }

		//! The cumulative vitality of all parts of this body.
		double total_vitality() const { return _total_vitality; }

		auto parts() const { return _parts | cast_transform<cref<body_part>>(); }
		auto heads() const { return _heads | cast_transform<cref<head>>(); }
		auto torsos() const { return _torsos | cast_transform<cref<torso>>(); }
		auto arms() const { return _arms | cast_transform<cref<arm>>(); }
		auto hands() const { return _hands | cast_transform<cref<hand>>(); }
		auto legs() const { return _legs | cast_transform<cref<leg>>(); }
		auto feet() const { return _feet | cast_transform<cref<foot>>(); }
		auto wings() const { return _wings | cast_transform<cref<wing>>(); }
		auto tails() const { return _tails | cast_transform<cref<tail>>(); }

		std::vector<ref<body_part>> const& parts() { return _parts; }
		std::vector<ref<head>> const& heads() { return _heads; }
		std::vector<ref<torso>> const& torsos() { return _torsos; }
		std::vector<ref<arm>> const& arms() { return _arms; }
		std::vector<ref<hand>> const& hands() { return _hands; }
		std::vector<ref<leg>> const& legs() { return _legs; }
		std::vector<ref<foot>> const& feet() { return _feet; }
		std::vector<ref<wing>> const& wings() { return _wings; }
		std::vector<ref<tail>> const& tails() { return _tails; }

		//! The body part on this body with the given ID or nullptr if none exists.
		body_part* find_part(id<body_part> id);
		//! The body part on this body with the given ID or nullptr if none exists.
		body_part const* find_part(id<body_part> id) const;

		//! The head on this body with the given ID or nullptr if none exists.
		head* find_head(id<body_part> id);

		//! The torso on this body with the given ID or nullptr if none exists.
		torso* find_torso(id<body_part> id);

		//! The arm on this body with the given ID or nullptr if none exists.
		arm* find_arm(id<body_part> id);

		//! The hand on this body with the given ID or nullptr if none exists.
		hand* find_hand(id<body_part> id);

		//! The leg on this body with the given ID or nullptr if none exists.
		leg* find_leg(id<body_part> id);

		//! The foot on this body with the given ID or nullptr if none exists.
		foot* find_foot(id<body_part> id);

		//! The wing on this body with the given ID or nullptr if none exists.
		wing* find_wing(id<body_part> id);

		//! The tail on this body with the given ID or nullptr if none exists.
		tail* find_tail(id<body_part> id);

		//! Advances the body and all its parts one time unit.
		void update();
	private:
		being& _owner; // Okay to store reference here. A body's lifetime is a subset of its owning being's lifetime.

		uptr<body_part> _root;

		std::vector<ref<body_part>> _parts;

		std::vector<ref<head>> _heads;
		std::vector<ref<torso>> _torsos;
		std::vector<ref<arm>> _arms;
		std::vector<ref<hand>> _hands;
		std::vector<ref<leg>> _legs;
		std::vector<ref<foot>> _feet;
		std::vector<ref<wing>> _wings;
		std::vector<ref<tail>> _tails;

		// Cumulative attributes
		double _total_vitality;
	};
}
