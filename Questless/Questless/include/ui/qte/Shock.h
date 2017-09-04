//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <vector>
#include <functional>

#include "../Dialog.h"
#include "animation/Camera.h"
#include "units/GameSpace.h"
#include "units/GameSeconds.h"
#include "units/GameVelocity.h"

namespace questless::qte
{
	//! Quick time event for determining shock strength.
	class Shock : public Dialog
	{
	public:
		//! @param target_coords The target strike coordinates.
		//! @param cont The dialog continuation function.
		Shock(RegionTile::Point target_coords, std::function<void(double)> cont);

		State update() final;

		void draw() const final;
	private:
		static constexpr units::GameSeconds _time_limit = 5.0s;
		static constexpr int _charges_per_quadrant = 4;
		static constexpr double _expected_charges = _charges_per_quadrant * 70.0;

		struct Charge
		{
			units::GameSpace::Point position;
			units::GameVelocity velocity;
		};

		static sdl::TextureHandle _point_charge_texture_handle;

		units::GameSpace::Point _target_point;
		Continuation<double> _cont;

		units::GameSeconds _elapsed_time = 0.0s;
		int _quadrant = 0;

		std::vector<Charge> _charges;

		uptr<sdl::Texture> _txt_title;
		uptr<sdl::Texture> _txt_prompt;

		void refresh() final { load_textures(); }

		void load_textures();
	};
}
