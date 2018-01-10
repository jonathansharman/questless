//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <optional>
#include <variant>

#include "dialog.hpp"
#include "hud.hpp"
#include "utility/utility.hpp"

namespace ql
{
	//! Dialog for the player to choose an action for his or her controlled being.
	class player_action_dialog : public dialog
	{
	public:
		struct idle { bool prolonged; };
		struct move { bool strafe; region_tile::direction direction; };
		struct use { std::size_t index; };
		using choice = std::variant<idle, move, use>;

		player_action_dialog(hud& hud, std::function<void(choice)> cont) : _hud{hud}, _cont{std::move(cont)} {}

		state update() final;

		void draw() const final
		{}
	private:
		hud& _hud;
		continuation<choice> _cont;
		static std::size_t _last_used;
	};
}
