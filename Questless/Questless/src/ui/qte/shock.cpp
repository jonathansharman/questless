//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/qte/shock.hpp"

#include "game.hpp"
#include "utility/random.hpp"

using namespace media;

namespace ql::qte {
	shock::shock(sf::Window const& window, fonts const& fonts, region_tile::point target_coords, std::function<void(double)> cont)
		: dialog{window, fonts}
		, _target_point{to_world(target_coords)}
		, _cont{std::move(cont)} //
	{
		_title = make_title("Build up a charge!");
		_prompt = make_prompt("Circle the target counter-clockwise as fast as you can!");
	}

	dialog::state shock::update(input_manager& im) {
		constexpr auto time_limit = 5.0_s;
		constexpr int charges_per_quadrant = 4;
		constexpr double expected_charges = charges_per_quadrant * 70.0;

		_elapsed_time += target_frame_duration;
		if (_elapsed_time > time_limit) { return _cont(_charges.size() / expected_charges); }

		bool accelerate = false;
		{ // Acclerate only when the mouse moves to the next quadrant over.
			world::vector const v = the_game().camera().point_hovered() - _target_point;
			switch (_quadrant) {
				case quadrant::ur:
					if (v.x() > 0.0 && v.y() > 0.0) {
						_quadrant = quadrant::ul;
						accelerate = true;
					}
					break;
				case quadrant::ul:
					if (v.x() < 0.0 && v.y() > 0.0) {
						_quadrant = quadrant::ll;
						accelerate = true;
					}
					break;
				case quadrant::ll:
					if (v.x() < 0.0 && v.y() < 0.0) {
						_quadrant = quadrant::lr;
						accelerate = true;
					}
					break;
				case quadrant::lr:
					if (v.x() > 0.0 && v.y() < 0.0) {
						_quadrant = quadrant::ur;
						accelerate = true;
					}
					break;
			}
		}
		if (accelerate) {
			for (int i = 0; i < charges_per_quadrant; ++i) {
				_charges.push_back(charge{_target_point + random_displacement(100.0),
					world_space::velocity{world_space::vector{random_angle(), 100.0}}});
			}
		}

		for (auto& point_charge : _charges) {
			world_space::vector r = _target_point - point_charge.position; // displacement to target
			double d = r.length(); // distance to target

			if (accelerate) {
				// Accelerate counter-clockwise from the target.
				point_charge.velocity.step() += 7'000.0 * world_space::vector{-r.y(), r.x()} / square(d);
			}

			// Apply drag.
			point_charge.velocity *= 0.99;
			// Apply random acceleration.
			point_charge.velocity.step() += random_displacement(50.0);
			// Apply attractive and repulsive forces.
			point_charge.velocity.step() += 20'000.0 * r / std::max(1.0, square(d));
			point_charge.velocity.step() -= 800'000.0 * r / std::max(1.0, cube(d));
			// Update position.
			point_charge.position += point_charge.velocity * target_frame_duration;
		}

		return state::open;
	}

	void shock::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		//! @todo Refactor point charge to use a particle animation?

		static auto point_charge_texture_handle = the_texture_manager().add(
			"resources/textures/particles/glow-large.png");

		// Draw point charges.
		for (auto const& point_charge : _charges) {
			sf::Uint8 const intensity = uniform(0, 255);
			spaces::view::scalar scale = (1.0f + intensity) / 2.0f;
			the_game().camera().draw(the_texture_manager()[point_charge_texture_handle],
				point_charge.position,
				spaces::window::vector::zero(),
				sf::Color{255, intensity, intensity},
				spaces::view::vector{scale, scale},
				world_space::radians{0.0});
		}

		draw_title(*_title);
		draw_prompt(*_prompt);
	}
}
