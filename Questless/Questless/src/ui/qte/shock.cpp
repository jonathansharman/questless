//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "shock.hpp"

#include "rsrc/particle.hpp"
#include "utility/random.hpp"

#include <range/v3/action/push_back.hpp>
#include <range/v3/view/generate_n.hpp>

namespace ql::qte {
	using namespace view::literals;

	shock::shock(rsrc::particle const& particle_resources, tile_hex_point target_coords)
		: _particle_resources{particle_resources}
		, _target_point{tile_layout.to_world(target_coords)} //
	{}

	auto shock::update(sec elapsed_time) -> void {
		constexpr auto time_limit = 5.0_s;
		constexpr int charges_per_quadrant = 4;
		constexpr double expected_charges = charges_per_quadrant * 70.0;

		_elapsed_time += target_frame_duration;
		if (_elapsed_time > time_limit) {
			//! @todo Compute result based on expected average particle speed.
			_promise.set_value(1.0f);
			return;
		}

		ranges::actions::push_back(_charges,
			ranges::views::generate_n(
				[&] {
					auto const position = _target_point + random_displacement(100.0_px);
					auto const velocity = vecx::make_polar_vector(100.0_px / 1.0_s, random_radians());
					return charge{position, velocity};
				},
				charges_per_quadrant));

		for (auto& point_charge : _charges) {
			// Get displacement to target.
			view::vector r = _target_point - point_charge.position;

			// Apply drag.
			point_charge.velocity *= 0.99f;
			// Apply random acceleration.
			point_charge.velocity += random_displacement(50.0_px / 1.0_s);
			// Attract towards the target.
			point_charge.velocity += 20'000.0f * 1.0_px * 1.0_px / 1.0_s * r / std::max(1.0_px * 1.0_px, r.length_squared());
			// Update position.
			point_charge.position += point_charge.velocity * elapsed_time;
		}
	}

	auto shock::draw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		// Draw point charges.
		for (auto const& charge : _charges) {
			sf::Sprite sprite{_particle_resources.glow_small};

			sprite.setPosition(view::to_sfml(charge.position));

			// Choose a random intensity, and use it to set the color and scale.
			sf::Uint8 const intensity = static_cast<sf::Uint8>(uniform(0, 255));
			sprite.setColor(sf::Color{intensity, intensity, intensity});
			float const scale = (1.0f + intensity) / 2.0f;
			sprite.setScale(scale, scale);

			target.draw(sprite, states);
		}
	}

	auto shock::on_mouse_move(view::point /*mouse_position*/) -> void {
		//! @todo Apply a force to all particles in the direction of the mouse, inversely proportional to d^2.
	}
}
