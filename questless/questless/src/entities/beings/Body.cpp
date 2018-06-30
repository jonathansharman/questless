//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/body.hpp"

#include <algorithm>
#include <memory>
#include <queue>

#include "entities/beings/being.hpp"
#include "entities/beings/body_part.hpp"
#include "game.hpp"

using namespace units;

namespace ql
{
	body::body(being& owner, uptr<body_part> root)
		: blood{0.0, [] { return 0.0; }, [&owner = owner] { return owner.body.total_vitality(); }}
		, _owner{owner}
		, _root{std::move(root)}
		, _total_vitality{0.0}
	{
		class part_attacher : public body_part_mutable_visitor
		{
		public:
			part_attacher(body& body) : _body{body} {}

			void visit(head& head) final { _body._heads.push_back(head); }
			void visit(torso& torso) final { _body._torsos.push_back(torso); }
			void visit(arm& arm) final { _body._arms.push_back(arm); }
			void visit(hand& hand) final { _body._hands.push_back(hand); }
			void visit(leg& leg) final { _body._legs.push_back(leg); }
			void visit(foot& foot) final { _body._feet.push_back(foot); }
			void visit(wing& wing) final { _body._wings.push_back(wing); }
			void visit(tail& tail) final { _body._tails.push_back(tail); }
		private:
			body& _body;
		};

		// Walk the parts tree to build the parts lists and compute cumulative values.
		std::queue<ref<body_part>> work_list;
		work_list.push(*_root);
		while (!work_list.empty()) {
			body_part& part = work_list.front();
			_parts.push_back(part);
			part_attacher attacher{*this};
			part.accept(attacher);

			// Add part values to totals.
			_total_vitality += part.vitality;

			// Remove current part from work list and add its children.
			work_list.pop();
			for (auto const& attachment : part.attachments()) {
				if (attachment->part) {
					work_list.push(*attachment->part);
				}
			}
		}

		// Start with maximum blood.
		blood = _total_vitality;
	}

	body_part* body::find_part(id<body_part> id)
	{
		for (body_part& part : _parts) {
			if (part.id == id) {
				return &part;
			}
		}
		return nullptr;
	}
	body_part const* body::find_part(id<body_part> id) const
	{
		for (body_part& part : _parts) {
			if (part.id == id) {
				return &part;
			}
		}
		return nullptr;
	}

	head* body::find_head(id<body_part> id)
	{
		for (head& head : _heads) {
			if (head.id == id) {
				return &head;
			}
		}
		return nullptr;
	}

	torso* body::find_torso(id<body_part> id)
	{
		for (torso& torso : _torsos) {
			if (torso.id == id) {
				return &torso;
			}
		}
		return nullptr;
	}

	arm* body::find_arm(id<body_part> id)
	{
		for (arm& arm : _arms) {
			if (arm.id == id) {
				return &arm;
			}
		}
		return nullptr;
	}

	hand* body::find_hand(id<body_part> id)
	{
		for (hand& hand : _hands) {
			if (hand.id == id) {
				return &hand;
			}
		}
		return nullptr;
	}

	leg* body::find_leg(id<body_part> id)
	{
		for (leg& leg : _legs) {
			if (leg.id == id) {
				return &leg;
			}
		}
		return nullptr;
	}

	foot* body::find_foot(id<body_part> id)
	{
		for (foot& foot : _feet) {
			if (foot.id == id) {
				return &foot;
			}
		}
		return nullptr;
	}

	wing* body::find_wing(id<body_part> id)
	{
		for (wing& wing : _wings) {
			if (wing.id == id) {
				return &wing;
			}
		}
		return nullptr;
	}

	tail* body::find_tail(id<body_part> id)
	{
		for (tail& tail : _tails) {
			if (tail.id == id) {
				return &tail;
			}
		}
		return nullptr;
	}

	void body::update()
	{
		// Update cumulative values.
		_total_vitality = 0.0;
		for (body_part const& part : _parts) {
			_total_vitality += part.vitality;
		}

		// Bleed and regenerate blood.
		for (body_part const& part : _parts) {
			blood += _owner.stats.health_regen * part.vitality - part.bleeding;
		}

		// Update parts.
		for (body_part& part : _parts) {
			part.update();
		}
	}
}
