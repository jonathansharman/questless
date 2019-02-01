//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/qte/aim_missile.hpp"

#include "animation/body_texturer.hpp"
#include "entities/beings/being.hpp"
#include "entities/beings/body_part.hpp"
#include "game.hpp"
#include "units/math.hpp"
#include "utility/random.hpp"

using namespace media;
using namespace units;
using namespace units::math;

namespace ql::qte {
	aim_missile::aim_missile(region_tile::point source_coords, being const& target_being, std::function<void(body_part*)> cont)
	    : _source_tile_coords{source_coords}
	    , _target_being{target_being}
	    , _cont{std::move(cont)}
	    , _aiming_circle{spaces::view::point{500.0f, 500.0f}, 10.0f}
	    , _title{make_title("Aim your shot!")}
	    , _prompt{make_prompt("Pull back the aiming circle.")} //
	{
		auto const tile_source_to_target = _target_being.coords - _source_tile_coords;
		auto const game_source_to_target = to_world(tile_source_to_target);
		spaces::view::vector const view_source_to_target{static_cast<spaces::view::scalar>(game_source_to_target.x()),
		    static_cast<spaces::view::scalar>(-game_source_to_target.y())};
		spaces::view::scalar const view_distance = view_source_to_target.length();
		auto const unit_source_to_target = view_distance == 0.0f ? view_source_to_target : view_source_to_target / view_distance;

		// Reduce scale with distance.
		constexpr float base_scale_divisor = 50.0f;
		_target_view_scale = base_scale_divisor / (view_distance + 1.0f);

		constexpr float aiming_circle_distance = 100.0f;
		_aiming_circle.center = _window.view_center() - aiming_circle_distance * unit_source_to_target;

		constexpr float target_distance = 200.0f;
		_target_view_coords = _window.view_center() + target_distance * unit_source_to_target;
	}

	dialog::state aim_missile::update(input_manager& im) {
		static constexpr auto time_limit = 5.0_s;

		switch (_aiming_state) {
			case aiming_state::beginning:
				if (im.down(sf::Mouse::Left) && _aiming_circle.contains(to_view_space(sf::Mouse::getPosition()))) {
					_aiming_state = aiming_state::aiming;
				}
				break;
			case aiming_state::aiming:
				// Can't hold at full draw for too long, or it's an automatic miss.
				//! @todo This seems bow-specific.
				_elapsed_time += game::target_frame_duration;
				if (_elapsed_time > time_limit * 10000) { // @todo Remove the factor after debugging.
					return _cont(nullptr);
				}

				_aiming_circle.center = to_view_space(sf::Mouse::getPosition());

				// Loose.
				if (im.up(sf::Mouse::Left)) {
					//! @todo Calculate heading.
					return _cont(nullptr);
				}
				break;
			case aiming_state::in_flight:
				//! @todo Implement.
				break;
			case aiming_state::ending:
				//! @todo Implement.
				break;
		}

		return state::open;
	}

	void aim_missile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		{ // Draw the target.
			body_texturer texturer;
			texturer.visit(_target_being.body);
			uptr<texture> texture = texturer.texture();
			texture->draw_transformed(_target_view_coords,
			    spaces::window::vector{0, texture->height() / 2},
			    colors::white_vector(),
			    spaces::view::vector{_target_view_scale, _target_view_scale});
		}

		if (_aiming_state == aiming_state::beginning || _aiming_state == aiming_state::aiming) {
			// Draw the aiming circle.
			the_renderer().draw_disc(_aiming_circle, 3.0f, colors::white(), colors::red(0.75f));
			// Draw the hint circle.
			spaces::view::sphere const hint_circle
			    //{ the_window().view_center() + (the_window().view_center() - _aiming_circle.center)
			    {_target_view_coords, _aiming_circle.radius};
			the_renderer().draw_disc(hint_circle, 3.0f, colors::white(0.5f), colors::red(0.375f));
		}

		draw_title(*_title);
		draw_prompt(*_prompt);
	}
}
