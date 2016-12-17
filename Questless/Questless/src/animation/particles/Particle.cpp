/**
* @file    Particle.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Particle class.
*/

#include "animation/particles/Particle.h"

namespace questless
{
	void Particle::update()
	{
		subupdate();

		_time_left -= frame_duration;
		if (dead()) {
			return;
		}
		_position += _velocity * frame_duration;
		_velocity += _acceleration * frame_duration;
		if (face_towards_heading()) {
			_angle = VectorF{_velocity.step().x, -_velocity.step().y}.angle();
		} else {
			_angle += _angular_velocity * frame_duration;
		}
		_scale += _scale_velocity * frame_duration;
	}

	void Particle::draw(const Camera& camera)
	{
		camera.draw
			( texture()
			, _position.to_point()
			, Origin{boost::none}
			, sdl::Color::white(percentage_to_byte(fade_out() ? _time_left.count() / _lifetime.count() : 1.0))
			, HScale{_scale}
			, VScale{_scale}
			, AngleRadians{_angle}
			);
	}
}
