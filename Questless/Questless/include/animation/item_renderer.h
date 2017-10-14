//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/item_visitor.h"
#include "sdl/resources.h"
#include "utility/reference.h"
#include "units/screen_space.h"

namespace ql
{
	//! Draws items to the screen.
	class item_renderer : public item_const_visitor
	{
	public:
		//! @param position The coordinates at which the item should be drawn to the screen.
		item_renderer(units::screen_space::point position) : _position{position} {}

		void visit(arrow const&) final;
		void visit(bow const&) final;
		void visit(quarterstaff const&) final;
		void visit(quiver const&) final;
		void visit(scroll const&) final;
		void visit(gatestone const&) final;
	private:
		units::screen_space::point _position;
	};
}
