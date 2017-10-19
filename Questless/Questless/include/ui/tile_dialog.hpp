//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <functional>
#include <optional>

#include "dialog.hpp"
#include "animation/camera.hpp"
#include "world/coordinates.hpp"

namespace ql
{
	//! Retrieves region tile coordinates from the player.
	class tile_dialog : public dialog
	{
	public:
		tile_dialog
			( std::string title
			, std::string prompt
			, std::optional<region_tile::point> origin
			, std::function<bool(region_tile::point)> predicate
			, std::function<void(std::optional<region_tile::point>)> cont
			)
			: _title{std::move(title)}
			, _prompt{std::move(prompt)}
			, _origin{std::move(origin)}
			, _predicate{std::move(predicate)}
			, _cont{std::move(cont)}
		{
			load_textures();
		}

		state update() final;

		void draw() const final;
	private:
		std::string _title;
		std::string _prompt;
		std::optional<region_tile::point> _origin;
		std::function<bool(region_tile::point)> _predicate;
		continuation<std::optional<region_tile::point>> _cont;

		uptr<sdl::texture> _txt_title;
		uptr<sdl::texture> _txt_prompt;

		void refresh() final { load_textures(); }

		void load_textures();
	};
}
