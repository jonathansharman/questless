/**
* @file    TileDialog.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the TileDialog class, which retrives tile coordinates from the player.
*/

#ifndef TILE_DIALOG_H
#define TILE_DIALOG_H

#include <string>
#include <functional>

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
			, const Camera& camera
			, boost::optional<RegionTileCoords> origin
			, std::function<bool(RegionTileCoords)> predicate
			, std::function<void(boost::optional<RegionTileCoords>)> cont
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

		bool update(sdl::Input& input) override;

		void draw(const sdl::Window& window) override;
	private:
		std::string _title;
		std::string _prompt;
		const Camera& _camera;
		boost::optional<RegionTileCoords> _origin;
		std::function<bool(RegionTileCoords)> _predicate;
		Continuation<boost::optional<RegionTileCoords>> _cont;

		sdl::Texture::ptr _txt_title;
		sdl::Texture::ptr _txt_prompt;

		void refresh() override { load_textures(); }

		void load_textures();

	};
}

#endif
