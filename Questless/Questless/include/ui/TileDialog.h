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
#include "utility/hex-utilities.h"

namespace questless
{
	class TileDialog : public Dialog
	{
	public:
		TileDialog(std::string title, std::string prompt, const Camera& camera, std::function<bool(HexCoords)> predicate, std::function<void(optional<HexCoords>)> cont)
			: _title{std::move(title)}, _prompt{std::move(prompt)}, _camera{camera}, _predicate{std::move(predicate)}, _cont{std::move(cont)}
		{
			load_textures();
		}

		/// Updates the dialog state based on input.
		/// @param input User input used to update the dialog.
		void update(const sdl::Input& input) override;

		/// Draws the dialog to the screen.
		/// @param window The window.
		void draw(const sdl::Window& window) override;
	private:
		std::string _title;
		std::string _prompt;
		const Camera& _camera;
		std::function<bool(HexCoords)> _predicate;
		std::function<void(optional<HexCoords>)> _cont;

		sdl::Texture::ptr _txt_title;
		sdl::Texture::ptr _txt_prompt;

		void refresh() override { load_textures(); }

		void load_textures();

	};
}

#endif
