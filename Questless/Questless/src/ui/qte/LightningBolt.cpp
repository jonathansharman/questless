/**
* @file    MessageDialog.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the MagnitudeDialog class.
*/

#include "ui/qte/LightningBolt.h"

#include "utility/utility.h"
#include "utility/Velocity.h"

using namespace sdl;

namespace questless::qte
{
	sdl::Handle<sdl::Texture> LightningBolt::_point_charge_texture_handle;

	Initializer<LightningBolt> LightningBolt::_initializer;
	void LightningBolt::initialize()
	{
		_point_charge_texture_handle = sdl::texture_manager().add([] {
			return Texture::make("resources/textures/glow.png", renderer(), SDL_BLENDMODE_BLEND);
		});
	}

	LightningBolt::LightningBolt(const Camera& camera, PointF target, std::function<void(double)> cont)
		: _camera{camera}
		, _target{target}
		, _cont { std::move(cont) }
	{
		load_textures();
	}

	bool LightningBolt::update(Input&)
	{
		_elapsed_time += frame_duration;
		if (_elapsed_time > _time_limit) {
			return _cont(_charges.size() / _expected_charges);
		}

		// Acclerate only when the mouse moves to the next quadrant over.
		bool accelerate = false;
		{
			double x = _camera.pt_hovered().x - _target.x;
			double y = _camera.pt_hovered().y - _target.y;
			switch (_quadrant) {
			case 0:
				if (x < y && x > -y) {
					_quadrant = 1;
					accelerate = true;
				}
				break;
			case 1:
				if (x > y && x > -y) {
					_quadrant = 2;
					accelerate = true;
				}
				break;
			case 2:
				if (x > y && x < -y) {
					_quadrant = 3;
					accelerate = true;
				}
				break;
			case 3:
				if (x < y && x < -y) {
					_quadrant = 0;
					accelerate = true;
				}
				break;
			}
		}
		if (accelerate) {
			for (int i = 0; i < _charges_per_quadrant; ++i) {
				_charges.push_back(Charge
					{ _target + random_displacement(100.0)
					, Velocity::from_angle_speed(uniform(0.0, tau), 100.0)
					});
			}
		}

		for (auto& point_charge : _charges) {
			VectorF r = _target - point_charge.position; // displacement to target
			double d = r.length(); // distance to target

			if (accelerate) {
				// Accelerate counter-clockwise from the target.
				point_charge.velocity += 7'000.0 * Velocity{-r.y, r.x} / square(d);
			}

			// Apply drag.
			point_charge.velocity *= 0.99;
			// Apply random acceleration.
			point_charge.velocity += Velocity{random_displacement(50.0)};
			// Apply attractive and repulsive forces.
			point_charge.velocity += 20'000.0 * Velocity{r} / std::max(1.0, square(d));
			point_charge.velocity -= 800'000.0 * Velocity{r} / std::max(1.0, cube(d));
			// Update position.
			point_charge.position += point_charge.velocity.displacement(frame_duration);
		}

		return false;
	}

	void LightningBolt::draw(const Window& window)
	{
		int x_center = window.center().x;

		// Draw point charges.
		for (auto&& point_charge : _charges) {
			double intensity = uniform(0.0, 1.0);
			_camera.draw
				( texture_manager()[_point_charge_texture_handle]
				, point_charge.position.to_point()
				, boost::none // origin
				, Color{255, 255, percentage_to_byte(intensity)}
				, (1.0 + intensity) / 2 // h-scale
				, (1.0 + intensity) / 2 // v-scale
				, 0.0 // angle
				);
		}

		// Draw line.
		_camera.draw_lines({_target.to_point(), _camera.pt_hovered().to_point()}, Color::yellow());

		// Draw the prompt.
		renderer().draw_rect(Rect{x_center - _txt_prompt->width() / 2, _prompt_top, _txt_prompt->width(), _txt_prompt->height()}, Color::black(128), true);
		_txt_prompt->draw(Point{x_center, _prompt_top}, HAlign::center);
	}

	void LightningBolt::load_textures()
	{
		_txt_prompt = Texture::make(font_manager()[prompt_font_handle()].render("Build up a charge!", renderer(), Color::white()));
	}
}
