//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include <deque>
#include <type_traits>
#include <algorithm>
#include <utility>

#include "animation/body_texturer.hpp"
#include "entities/beings/body_part.hpp"
#include "sdl/resources.hpp"
#include "utility/utility.hpp"

using namespace sdl;
using namespace units;

namespace ql
{
	void body_texturer::visit(body const& body)
	{
		screen_space::box bounds = body.bounds();

		_texture = std::make_unique<sdl::texture>(bounds.size, colors::color{0.0f, 1.0f, 1.0f, 0.5f});
		_texture->as_target([this, &body] {
			{ // Draw outline.
				screen_space::box outline
					{ screen_space::point{0, 0}
					, screen_space::vector{_texture->width(), _texture->height()}
					};
				the_renderer().draw_box(outline, 1, colors::red(), colors::clear());
			}

			for (body_part const& part : body) {
				for (screen_space::box const& region : part.regions()) {
					constexpr double threshold = 0.5f;

					double health_percentage = part.health / part.vitality();
					float red;
					float green;
					if (health_percentage < threshold) {
						red = 1.0f;
						green = static_cast<float>(health_percentage / threshold);
					} else {
						red = static_cast<float>(1.0 - (health_percentage - threshold) / (1.0 - threshold));
						green = 1.0f;
					}
					colors::color color{red, green, 0.0f, 1.0f};
					the_renderer().draw_box(region.translated(body.offset_to_center()), 1, colors::black(), color);
				}
			}
		}, solid_program());
	}
}
