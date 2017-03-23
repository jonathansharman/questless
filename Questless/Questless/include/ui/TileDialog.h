/**
* @file    TileDialog.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the TileDialog class, which retrives tile coordinates from the player.
*/

#pragma once

#include <string>
#include <functional>
#include <optional>

#include "Dialog.h"
#include "animation/Camera.h"
#include "world/coordinates.h"

namespace questless
{
	class TileDialog : public Dialog
	{
	public:
		TileDialog
			( std::string title
			, std::string prompt
			, Camera const& camera
			, std::optional<RegionTileCoords> origin
			, std::function<bool(RegionTileCoords)> predicate
			, std::function<void(std::optional<RegionTileCoords>)> cont
			)
			: _title{std::move(title)}
			, _prompt{std::move(prompt)}
			, _camera{camera}
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
		Camera const& _camera;
		std::optional<RegionTileCoords> _origin;
		std::function<bool(RegionTileCoords)> _predicate;
		Continuation<std::optional<RegionTileCoords>> _cont;

		sdl::Texture::ptr _txt_title;
		sdl::Texture::ptr _txt_prompt;

		void refresh() override { load_textures(); }

		void load_textures();

	};
}
