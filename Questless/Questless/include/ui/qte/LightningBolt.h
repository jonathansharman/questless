/**
* @file    QteLightningBolt.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Quick time event to determine lightning bolt strength.
*/

#ifndef QTE_LIGHTNING_BOLT_H
#define QTE_LIGHTNING_BOLT_H

#include <string>
#include <vector>
#include <functional>

#include "../Dialog.h"
#include "utility/Initializer.h"
#include "animation/Camera.h"
#include "units/Point.h"
#include "units/GameVelocity.h"

namespace questless::qte
{
	class LightningBolt : public Dialog
	{
	public:
		/// @param camera The game camera.
		/// @param target The target strike game point.
		/// @param cont The dialog continuation function.
		LightningBolt(const Camera& camera, units::GamePoint target, std::function<void(double)> cont);

		bool update(sdl::Input& input) override;

		void draw(const sdl::Window& window) override;
	private:
		static constexpr units::GameSeconds _time_limit = 5.0s;
		static constexpr int _charges_per_quadrant = 4;
		static constexpr double _expected_charges = _charges_per_quadrant * 70.0;

		struct Charge
		{
			units::GamePoint position;
			units::GameVelocity velocity;
		};

		friend class Initializer<LightningBolt>;
		static Initializer<LightningBolt> _initializer;
		static void initialize();

		static sdl::Handle<sdl::Texture> LightningBolt::_point_charge_texture_handle;

		const Camera& _camera;
		units::GamePoint _target;
		Continuation<double> _cont;

		units::GameSeconds _elapsed_time = 0.0s;
		int _quadrant = 0;

		std::vector<Charge> _charges;

		sdl::Texture::ptr _txt_prompt;

		void refresh() override { load_textures(); }

		void load_textures();
	};
}

#endif