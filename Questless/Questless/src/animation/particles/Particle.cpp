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
		_time_left -= frame_duration;
		if (_time_left < double_seconds::zero()) {
			_dead = true;
			return;
		}
		_position += _velocity.displacement(frame_duration);
		_angle += _angular_velocity * frame_duration;
		_scale += _scale_velocity * frame_duration;
	}

	void Particle::draw(const Camera& camera)
	{
		texture()->alpha(percentage_to_byte(_fade_out ? _time_left.count() / _lifetime.count() : 1.0));
		camera.draw(*texture(), _position.to_point(), nullopt, _scale, _scale, false, false, _angle);
	}
}