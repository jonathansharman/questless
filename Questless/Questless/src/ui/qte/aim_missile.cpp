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

using namespace sdl;
using namespace units;
using namespace units::math;

namespace ql::qte {
	aim_missile::aim_missile(region_tile::point source_coords, being const& target_being, std::function<void(body_part*)> cont)
		: _source_tile_coords{source_coords}
		, _target_being{target_being}
		, _cont{std::move(cont)}
		, _aiming_circle{view_space::point{500.0f, 500.0f}, 10.0f}
		, _txt_title{make_title("Aim your shot!")}
		, _txt_prompt{make_prompt("Pull back the aiming circle.")}
	{
		auto const tile_source_to_target = _target_being.coords - _source_tile_coords;
		auto const game_source_to_target = to_world(tile_source_to_target);
		view_space::vector const view_source_to_target
			{ static_cast<view_space::scalar>(game_source_to_target.x())
			, static_cast<view_space::scalar>(-game_source_to_target.y())
			};
		view_space::scalar const view_distance = view_source_to_target.length();
		auto const unit_source_to_target = view_distance == 0.0f ? view_source_to_target : view_source_to_target / view_distance;

		// Reduce scale with distance.
		constexpr float base_scale_divisor = 50.0f;
		_target_view_scale = base_scale_divisor / (view_distance + 1.0f);

		constexpr float aiming_circle_distance = 100.0f;
		_aiming_circle.center = the_window().view_center() - aiming_circle_distance * unit_source_to_target;

		constexpr float target_distance = 200.0f;
		_target_view_coords = the_window().view_center() + target_distance * unit_source_to_target;
	}

	dialog::state aim_missile::update() {
		static constexpr units::game_space::seconds time_limit = 5.0s;

		switch (_aiming_state) {
			case aiming_state::beginning:
				if (the_input().down(sdl::mouse_button::left) && _aiming_circle.contains(to_view_space(the_input().mouse_position()))) {
					_aiming_state = aiming_state::aiming;
				}
				break;
			case aiming_state::aiming:
				// Can't hold at full draw for too long, or it's an automatic miss.
				//! @todo This seems bow-specific.
				_elapsed_time += game::frame_duration;
				if (_elapsed_time > time_limit * 10000) { // @todo Remove the factor after debugging.
					return _cont(nullptr);
				}

				_aiming_circle.center = to_view_space(the_input().mouse_position());

				// Loose.
				if (the_input().up(sdl::mouse_button::left)) {
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

	void aim_missile::draw() const {
		{ // Draw the target.
			body_texturer texturer;
			texturer.visit(_target_being.body);
			uptr<texture> texture = texturer.texture();
			texture->draw_transformed
				( _target_view_coords
				, texture_space::vector{0, texture->height() / 2}
				, colors::white_vector()
				, view_space::vector{_target_view_scale, _target_view_scale}
				);
		}

		if (_aiming_state == aiming_state::beginning || _aiming_state == aiming_state::aiming) {
			// Draw the aiming circle.
			the_renderer().draw_disc(_aiming_circle, 3.0f, colors::white(), colors::red(0.75f));
			// Draw the hint circle.
			view_space::sphere const hint_circle
				//{ the_window().view_center() + (the_window().view_center() - _aiming_circle.center)
				{ _target_view_coords
				, _aiming_circle.radius
				};
			the_renderer().draw_disc(hint_circle, 3.0f, colors::white(0.5f), colors::red(0.375f));
		}

		draw_title(*_txt_title);
		draw_prompt(*_txt_prompt);
	}
}
