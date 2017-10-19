//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/body.hpp"

#include <deque>
#include <algorithm>
#include <memory>

#include "entities/beings/body_part.hpp"

using namespace units;

namespace ql
{
	body::body(being& owner, uptr<body_part> root) : _owner{owner}, _root{std::move(root)}
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

		int x_min = 0;
		int y_min = 0;
		int x_max = 0;
		int y_max = 0;

		// Walk the parts tree to build the parts lists and compute bounds.
		std::deque<ref<body_part>> work_list;
		work_list.push_back(*_root);
		while (!work_list.empty()) {
			body_part& part = work_list.front();
			_parts.push_back(part);
			part_attacher attacher{*this};
			part.accept(attacher);

			for (screen_space::box const& region : part.regions()) {
				x_min = std::min(x_min, left(region));
				x_max = std::max(x_max, right(region));
				y_min = std::min(y_min, top(region));
				y_max = std::max(y_max, bottom(region));
			}

			// Remove current part from work list and add its children.
			work_list.pop_front();
			for (uptr<body_part> const& child : part.children()) {
				work_list.push_back(*child);
			}
		}

		_bounds = screen_space::box{screen_space::point{x_min, y_min}, screen_space::vector{x_max - x_min + 1, y_max - y_min + 1}};
		_offset_to_center = screen_space::vector{-x_min, -y_min};
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
}
