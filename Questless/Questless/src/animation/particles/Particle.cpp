//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particles/Particle.h"
#include "Game.h"

using namespace sdl;
using namespace units;

namespace questless
{
	void Particle::animation_subupdate()
	{
		auto elapsed_time = Animation::elapsed_time();

		_time_left -= elapsed_time;
		if (_time_left < units::GameSeconds::zero()) {
			_over = true;
			return;
		}

		_displacement += _velocity * elapsed_time;
		_velocity += _acceleration * elapsed_time;
		if (face_towards_heading()) {
			_angle = _velocity.step().angle();
		} else {
			_angle += _angular_velocity * elapsed_time;
		}
		_scale += _scale_velocity * elapsed_time;

		particle_subupdate();
	}

	void Particle::draw(units::ScreenSpace::Point position) const
	{
		texture().draw(position, HAlign::center, VAlign::middle);
	}

	void Particle::draw(units::GameSpace::Point position, Camera const& camera, colors::ColorFactor color_factor) const
	{
		// Determine color factor to use.
		color_factor = ignore_color_mod() ? _color_factor : color_factor * _color_factor;
		if (fade_out()) {
			color_factor[3] *= static_cast<float>(_time_left.count() / _lifetime.count());
		}

		camera.draw
			( texture()
			, position + _displacement
			, Origin{std::nullopt}
			, color_factor
			, HScale{_scale}
			, VScale{_scale}
			, _angle
			);
	}
}
