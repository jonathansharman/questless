//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/item_visitor.hpp"
#include "sdl/resources.hpp"
#include "utility/reference.hpp"
#include "units/window_space.hpp"

namespace ql
{
	//! Draws items to the screen.
	class item_renderer : public item_const_visitor
	{
	public:
		//! @param position The coordinates at which the item should be drawn to the screen.
		item_renderer(units::window_space::point position) : _position{position} {}

		void visit(arrow const&) final;
		void visit(bow const&) final;
		void visit(quarterstaff const&) final;
		void visit(quiver const&) final;
		void visit(scroll const&) final;
		void visit(gatestone const&) final;
	private:
		units::window_space::point _position;
	};
}
