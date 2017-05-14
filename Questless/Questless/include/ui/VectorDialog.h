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
	//! Retrieves a region tile vector from the player.
	class VectorDialog : public Dialog
	{
	public:
		VectorDialog
			( std::string title
			, std::string prompt
			, std::optional<RegionTile::Point> origin
			, std::function<bool(RegionTile::Vector)> predicate
			, std::function<void(std::optional<RegionTile::Vector>)> cont
			)
			: _title{std::move(title)}
			, _prompt{std::move(prompt)}
			, _origin{std::move(origin)}
			, _predicate{std::move(predicate)}
			, _cont{std::move(cont)}
		{
			load_textures();
		}

		State update() final;

		void draw() const final;
	private:
		std::string _title;
		std::string _prompt;
		std::optional<RegionTile::Point> _origin;
		std::function<bool(RegionTile::Vector)> _predicate;
		Continuation<std::optional<RegionTile::Vector>> _cont;

		uptr<sdl::Texture> _txt_title;
		uptr<sdl::Texture> _txt_prompt;

		void refresh() final { load_textures(); }

		void load_textures();
	};
}
