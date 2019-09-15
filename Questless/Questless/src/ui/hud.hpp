//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

#include "hotbar.hpp"
#include "inventory_widget.hpp"
#include "panel.hpp"
#include "view_space.hpp"
#include "world_widget.hpp"

#include "reg.hpp"
#include "rsrc/hud.hpp"

#include <atomic>
#include <future>
#include <thread>

namespace ql {
	namespace effects {
		struct effect;
	}
	struct list_dialog;

	//! The primary interface with the player during gameplay.
	struct hud : widget {
		//! @param player_id The ID of the player-controlled being.
		hud(uptr<widget>& root, rsrc::fonts const& fonts, id region_id, id player_id);

		~hud();

		//! Renders @p effect to be perceived by the player.
		auto render_effect(effects::effect const& effect) -> void;

		//! Gets a future which is set after the player passes the current turn.
		auto pass_future() -> std::future<void>;

		//! Resets this HUD's stored player ID to @p player_id.
		auto set_player_id(id player_id) -> void;

		auto get_size() const -> view::vector final;

		auto update(sec elapsed_time) -> void final;

		auto set_position(view::point position) -> void final;

		auto get_position() const -> view::point final;

		auto on_parent_resize(view::vector parent_size) -> void final;

		auto on_key_press(sf::Event::KeyEvent const& event) -> event_handled final;

		auto on_mouse_press(sf::Event::MouseButtonEvent const& event) -> event_handled final;

		auto on_mouse_release(sf::Event::MouseButtonEvent const& event) -> event_handled final;

		auto on_mouse_move(view::point mouse_position) -> void final;

		auto on_request_quit() -> event_handled final;

	private:
		uptr<widget>& _root;

		id _region_id;
		id _player_id{};
		std::promise<void> _pass_promise;

		rsrc::hud _rsrc;
		view::point _position;
		view::vector _size;
		world_widget _world_widget;
		hotbar _hotbar;
		inventory_widget _inv;
		uptr<list_dialog> _item_dialog;
		bool _show_inv = false;

		enum class state { player_input, game_loop, ending };
		std::atomic<state> _state;
		std::thread _game_logic_thread;

		auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;

		auto get_item_options(id item_id) -> std::vector<std::tuple<sf::String, std::function<void()>>>;

		auto pass() -> void;

		auto make_game_logic_thread() -> std::thread;
	};
}
