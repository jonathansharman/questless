/**
* @file    MessageDialog.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the MagnitudeDialog class.
*/

#include "ui/qte/LightningBolt.h"
#include "Game.h"
#include "utility/utility.h"

using namespace sdl;
using namespace units;

namespace questless::qte
{
	sdl::Handle<sdl::Texture> LightningBolt::_point_charge_texture_handle;

	Initializer<LightningBolt> LightningBolt::_initializer;
	void LightningBolt::initialize()
	{
		_point_charge_texture_handle = sdl::texture_manager().add([] {
			return std::make_unique<Texture>("resources/textures/glow.png", renderer(), SDL_BLENDMODE_BLEND);
		});
	}

	LightningBolt::LightningBolt(Camera const& camera, RegionTileCoords target_coords, std::function<void(double)> cont)
		: _camera{camera}
		, _target_point{units::Layout::dflt().to_world(target_coords)}
		, _cont{std::move(cont)}
	{
		load_textures();
	}

	bool LightningBolt::update(Input&)
	{
		_elapsed_time += Game::frame_duration;
		if (_elapsed_time > _time_limit) {
			return _cont(_charges.size() / _expected_charges);
		}

		// Acclerate only when the mouse moves to the next quadrant over.
		bool accelerate = false;
		{
			GameVector v = _camera.point_hovered() - _target_point;
			switch (_quadrant) {
				case 0:
					if (v.x < v.y && v.x > -v.y) {
						_quadrant = 1;
						accelerate = true;
					}
					break;
				case 1:
					if (v.x > v.y && v.x > -v.y) {
						_quadrant = 2;
						accelerate = true;
					}
					break;
				case 2:
					if (v.x > v.y && v.x < -v.y) {
						_quadrant = 3;
						accelerate = true;
					}
					break;
				case 3:
					if (v.x < v.y && v.x < -v.y) {
						_quadrant = 0;
						accelerate = true;
					}
					break;
			}
		}
		if (accelerate) {
			for (int i = 0; i < _charges_per_quadrant; ++i) {
				_charges.push_back(Charge
					{ _target_point + random_displacement(100.0)
					, GameVelocity{GameVector{random_angle(), 100.0}}
					});
			}
		}

		for (auto& point_charge : _charges) {
			GameVector r = _target_point - point_charge.position; // displacement to target
			double d = r.length(); // distance to target

			if (accelerate) {
				// Accelerate counter-clockwise from the target.
				point_charge.velocity.step() += 7'000.0 * GameVector{-r.y, r.x} / square(d);
			}

			// Apply drag.
			point_charge.velocity *= 0.99;
			// Apply random acceleration.
			point_charge.velocity.step() += random_displacement(50.0);
			// Apply attractive and repulsive forces.
			point_charge.velocity.step() += 20'000.0 * r / std::max(1.0, square(d));
			point_charge.velocity.step() -= 800'000.0 * r / std::max(1.0, cube(d));
			// Update position.
			point_charge.position += point_charge.velocity * Game::frame_duration;
		}

		return false;
	}

	void LightningBolt::draw(Window const& window)
	{
		int x_center = window.center().x;

		// Draw point charges.
		for (auto const& point_charge : _charges) {
			double intensity = uniform(0.0, 1.0);
			_camera.draw
				( texture_manager()[_point_charge_texture_handle]
				, point_charge.position
				, Origin{boost::none}
				, Color{255, 255, percentage_to_byte(intensity)}
				, HScale{(1.0 + intensity) / 2.0}
				, VScale{(1.0 + intensity) / 2.0}
				, GameRadians{0.0}
				);
		}

		// Draw the title and prompt.
		renderer().draw_rect(ScreenRect{x_center - _txt_title->width() / 2, 0, _txt_title->width(), _txt_title->height()}, Color::black(128), true);
		_txt_title->draw(ScreenPoint{x_center, 0}, HAlign::center);
		renderer().draw_rect(ScreenRect{x_center - _txt_prompt->width() / 2, _prompt_top, _txt_prompt->width(), _txt_prompt->height()}, Color::black(128), true);
		_txt_prompt->draw(ScreenPoint{x_center, _prompt_top}, HAlign::center);
	}

	void LightningBolt::load_textures()
	{
		_txt_title = std::make_unique<Texture>(font_manager()[title_font_handle()].render("Build up a charge!", renderer(), Color::white()));

		_txt_prompt = std::make_unique<Texture>(font_manager()[prompt_font_handle()].render("Circle the target counter-clockwise as fast as you can!", renderer(), Color::white()));
	}
}
