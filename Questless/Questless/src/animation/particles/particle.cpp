//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particles/particle.h"

#include "game.h"

using namespace sdl;
using namespace units;

namespace ql
{
	void particle::animation_subupdate()
	{
		auto elapsed_time = animation::elapsed_time();

		_time_left -= elapsed_time;
		if (_time_left < units::game_space::seconds::zero()) {
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

	void particle::draw(units::screen_space::point position) const
	{
		texture().draw(position, texture_space::align_center, texture_space::align_middle);
	}

	void particle::draw(units::game_space::point position, camera const& camera, colors::color_factor color_factor) const
	{
		// Determine color factor to use.
		color_factor = ignore_color_mod() ? _color_factor : color_factor * _color_factor;
		if (fade_out()) {
			color_factor[3] *= static_cast<float>(_time_left.count() / _lifetime.count());
		}

		camera.draw
			( texture()
			, position + _displacement
			, origin{texture_space::vector::zero()}
			, color_factor
			, h_scale{_scale}
			, v_scale{_scale}
			, _angle
			);
	}
}
