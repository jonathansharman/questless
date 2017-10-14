//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/body.h"

#include <deque>
#include <algorithm>
#include <memory>

#include "entities/beings/body_part.h"

using namespace units;

namespace ql
{
	body::body(being& owner, uptr<body_part> root) : _owner{owner}, _root{std::move(root)}
	{
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

	void body::part_attacher::visit(head& head)
	{
		_body._heads.push_back(head);
	}
	void body::part_attacher::visit(torso& torso)
	{
		_body._torsos.push_back(torso);
	}
	void body::part_attacher::visit(arm& arm)
	{
		_body._arms.push_back(arm);
	}
	void body::part_attacher::visit(hand& hand)
	{
		_body._hands.push_back(hand);
	}
	void body::part_attacher::visit(leg& leg)
	{
		_body._legs.push_back(leg);
	}
	void body::part_attacher::visit(foot& foot)
	{
		_body._feet.push_back(foot);
	}
	void body::part_attacher::visit(wing& wing)
	{
		_body._wings.push_back(wing);
	}
	void body::part_attacher::visit(tail& tail)
	{
		_body._tails.push_back(tail);
	}
}
