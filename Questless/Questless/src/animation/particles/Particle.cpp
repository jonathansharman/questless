//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particles/Particle.h"
#include "Game.h"

using namespace sdl;

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

	void Particle::draw(units::ScreenPoint position) const
	{
		texture().draw(position, HAlign::center, VAlign::middle);
	}

	void Particle::draw(units::GamePoint position, Camera const& camera, Color color) const
	{
		// Determine color mod to use.
		color = ignore_color_mod() ? _color : color.mod(_color);
		if (fade_out()) {
			uint8_t a = percentage_to_byte(_time_left.count() / _lifetime.count());
			color.a = static_cast<uint8_t>(static_cast<uint32_t>(color.a) * a / 255);
		}

		camera.draw
			( texture()
			, position + _displacement
			, Origin{std::nullopt}
			, color
			, HScale{_scale}
			, VScale{_scale}
			, _angle
			);
	}
}
