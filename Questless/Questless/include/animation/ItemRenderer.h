//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/ItemVisitor.h"
#include "sdl/resources.h"
#include "utility/reference.h"
#include "units/ScreenSpace.h"

namespace questless
{
	//! Draws items to the screen.
	class ItemRenderer : public ItemConstVisitor
	{
	public:
		//! @param position The coordinates at which the item should be drawn to the screen.
		ItemRenderer(units::ScreenSpace::Point position) : _position{position} {}

		void visit(Arrow const&) final;
		void visit(Bow const&) final;
		void visit(Quarterstaff const&) final;
		void visit(Quiver const&) final;
		void visit(Scroll const&) final;
		void visit(Gatestone const&) final;
	private:
		units::ScreenSpace::Point _position;
	};
}
