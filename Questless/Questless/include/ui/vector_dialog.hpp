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
	//! Retrieves a region tile vector from the player.
	class vector_dialog : public dialog
	{
	public:
		vector_dialog
			( std::string title
			, std::string prompt
			, std::optional<region_tile::point> origin
			, std::function<bool(region_tile::vector)> predicate
			, std::function<void(std::optional<region_tile::vector>)> cont
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
		std::function<bool(region_tile::vector)> _predicate;
		continuation<std::optional<region_tile::vector>> _cont;

		uptr<sdl::texture> _txt_title;
		uptr<sdl::texture> _txt_prompt;

		void load_textures();
	};
}
