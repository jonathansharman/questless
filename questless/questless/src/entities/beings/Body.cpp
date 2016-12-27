/**
* @file    Body.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Body class.
*/

#include <deque>
#include <algorithm>
#include <memory>

#include "entities/beings/Body.h"
#include "entities/beings/BodyPart.h"

using namespace sdl;

namespace questless
{
	Body::Body(Being& owner, std::unique_ptr<BodyPart> root) : _owner{owner}, _root{std::move(root)}
	{
		int x_min = 0;
		int y_min = 0;
		int x_max = 0;
		int y_max = 0;

		// Walk the parts tree to build the parts lists and compute bounds.
		std::deque<BodyPart::ref> work_list;
		work_list.push_back(*_root);
		while (!work_list.empty()) {
			BodyPart& part = work_list.front();
			_parts.push_back(part);
			PartAttacher attacher{*this};
			part.accept(attacher);

			for (const ScreenRect& region : part.regions()) {
				x_min = std::min(x_min, region.x);
				x_max = std::max(x_max, region.right());
				y_min = std::min(y_min, region.y);
				y_max = std::max(y_max, region.bottom());
			}

			// Remove current part from work list and add its children.
			work_list.pop_front();
			for (const BodyPart::ptr& child : part.children()) {
				work_list.push_back(*child);
			}
		}

		_bounds = ScreenRect{x_min, y_min, x_max - x_min + 1, y_max - y_min + 1};
		_offset_to_center = ScreenVector{-x_min, -y_min};
	}

	void Body::PartAttacher::visit(Torso& torso)
	{
		_body._torsos.push_back(torso);
	}
	void Body::PartAttacher::visit(Head& head)
	{
		_body._heads.push_back(head);
	}
	void Body::PartAttacher::visit(Arm& arm)
	{
		_body._arms.push_back(arm);
	}
	void Body::PartAttacher::visit(Hand& hand)
	{
		_body._hands.push_back(hand);
	}
	void Body::PartAttacher::visit(Leg& leg)
	{
		_body._legs.push_back(leg);
	}
	void Body::PartAttacher::visit(Foot& foot)
	{
		_body._feet.push_back(foot);
	}
	void Body::PartAttacher::visit(Wing& wing)
	{
		_body._wings.push_back(wing);
	}
	void Body::PartAttacher::visit(Tail& tail)
	{
		_body._tails.push_back(tail);
	}
}
