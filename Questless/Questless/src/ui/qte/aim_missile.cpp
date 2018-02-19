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

namespace ql::qte
{
	aim_missile::aim_missile(region_tile::point source_coords, being const& target_being, std::function<void(body_part*)> cont)
		: _source_coords{source_coords}
		, _target_being{target_being}
		, _cont{std::move(cont)}
		, _aiming_circle{view_space::point{500.0f, 500.0f}, 10.0f}
	{
		_txt_title = make_title("Aim your shot!");
		_txt_prompt = make_prompt("Pull back the aiming circle.");
	}

	dialog::state aim_missile::update()
	{
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
				if (_elapsed_time > time_limit) {
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

	void aim_missile::draw() const
	{
		static auto point_charge_texture_handle = the_texture_manager().add("resources/textures/particles/glow-large.png");

		{ // Draw the target.
			body_texturer texturer;
			texturer.visit(_target_being.body);
			uptr<texture> texture = texturer.texture();
			texture->draw(the_window().window_center(), texture_space::align_left, texture_space::align_bottom);
		}

		// Draw the aiming circle.
		the_renderer().draw_disc(_aiming_circle, 3.0f, colors::white(), colors::red(0.75f));

		draw_title(*_txt_title);
		draw_prompt(*_txt_prompt);
	}
}
