//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include <deque>
#include <type_traits>
#include <algorithm>
#include <utility>

#include "animation/BodyTexturer.h"
#include "entities/beings/BodyPart.h"
#include "sdl/resources.h"
#include "utility/utility.h"

using namespace sdl;
using namespace units;

namespace questless
{
	void BodyTexturer::visit(Body const& body)
	{
		ScreenRect bounds = body.bounds();

		_texture = std::make_unique<Texture>(bounds.w, bounds.h);
		_texture->as_target([this, &body] {
			renderer().clear(Color::clear());
			renderer().draw_rect(ScreenRect{0, 0, _texture->width(), _texture->height()}, Color::red(), false);

			for (BodyPart const& part : body) {
				for (ScreenRect const& region : part.regions()) {
					constexpr double threshold = 1.0 / 2.0;

					double health_percentage = part.health / part.vitality();
					uint8_t red;
					uint8_t green;
					if (health_percentage < threshold) {
						red = 255;
						green = percentage_to_byte(health_percentage / threshold);
					} else {
						red = percentage_to_byte(1.0 - (health_percentage - threshold) / (1.0 - threshold));
						green = 255;
					}
					Color color{red, green, 0};
					renderer().draw_rect(region.translated(body.offset_to_center()), Color::black(), color);
				}
			}
		});
	}
}
