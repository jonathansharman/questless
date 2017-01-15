/**
* @file    Particle.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Particle implementation.
*/

#include "animation/particles/Particle.h"
#include "Game.h"

namespace questless
{
	void Particle::update()
	{
		subupdate();

		_time_left -= Game::frame_duration;
		if (dead()) {
			return;
		}
		_position += _velocity * Game::frame_duration;
		_velocity += _acceleration * Game::frame_duration;
		if (face_towards_heading()) {
			_angle = _velocity.step().angle();
		} else {
			_angle += _angular_velocity * Game::frame_duration;
		}
		_scale += _scale_velocity * Game::frame_duration;
	}

	void Particle::draw(const Camera& camera)
	{
		camera.draw
			( texture()
			, _position
			, Origin{boost::none}
			, sdl::Color::white(fade_out() ? percentage_to_byte(_time_left.count() / _lifetime.count()) : 255)
			, HScale{_scale}
			, VScale{_scale}
			, _angle
			);
	}
}
