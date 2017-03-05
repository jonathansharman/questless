/**
* @file    QteLightningBolt.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Quick time event to determine lightning bolt strength.
*/

#pragma once

#include <string>
#include <vector>
#include <functional>

#include "../Dialog.h"
#include "animation/Camera.h"
#include "units/Point.h"
#include "units/GameVelocity.h"

namespace questless::qte
{
	class LightningBolt : public Dialog
	{
	public:
		/// @param target_coords The target strike coordinates.
		/// @param cont The dialog continuation function.
		LightningBolt(Camera const& camera, RegionTileCoords target_coords, std::function<void(double)> cont);

		State update() override;

		void draw() const override;
	private:
		static constexpr units::GameSeconds _time_limit = 5.0s;
		static constexpr int _charges_per_quadrant = 4;
		static constexpr double _expected_charges = _charges_per_quadrant * 70.0;

		struct Charge
		{
			units::GamePoint position;
			units::GameVelocity velocity;
		};

		static sdl::Handle<sdl::Texture> LightningBolt::_point_charge_texture_handle;

		Camera const& _camera;
		units::GamePoint _target_point;
		Continuation<double> _cont;

		units::GameSeconds _elapsed_time = 0.0s;
		int _quadrant = 0;

		std::vector<Charge> _charges;

		sdl::Texture::ptr _txt_title;
		sdl::Texture::ptr _txt_prompt;

		void refresh() override { load_textures(); }

		void load_textures();
	};
}
