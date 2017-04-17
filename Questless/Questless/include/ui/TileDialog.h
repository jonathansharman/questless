//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <functional>
#include <optional>

#include "Dialog.h"
#include "animation/Camera.h"
#include "world/coordinates.h"

namespace questless
{
	//! Retrieves region tile coordinates from the player.
	class TileDialog : public Dialog
	{
	public:
		TileDialog
			( std::string title
			, std::string prompt
			, std::optional<RegionTileCoords> origin
			, std::function<bool(RegionTileCoords)> predicate
			, std::function<void(std::optional<RegionTileCoords>)> cont
			)
			: _title{std::move(title)}
			, _prompt{std::move(prompt)}
			, _origin{std::move(origin)}
			, _predicate{std::move(predicate)}
			, _cont{std::move(cont)}
		{
			load_textures();
		}

		State update() override;

		void draw() const override;
	private:
		std::string _title;
		std::string _prompt;
		std::optional<RegionTileCoords> _origin;
		std::function<bool(RegionTileCoords)> _predicate;
		Continuation<std::optional<RegionTileCoords>> _cont;

		sdl::Texture::uptr _txt_title;
		sdl::Texture::uptr _txt_prompt;

		void refresh() override { load_textures(); }

		void load_textures();
	};
}
