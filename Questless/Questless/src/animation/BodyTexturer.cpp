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
		ScreenSpace::Box bounds = body.bounds();

		_texture = std::make_unique<Texture>(bounds.size, colors::Color{0.0f, 1.0f, 1.0f, 0.5f});
		_texture->as_target([this, &body] {
			{ // Draw outline.
				ScreenSpace::Box outline
					{ ScreenSpace::Point{0, 0}
					, ScreenSpace::Vector{_texture->width(), _texture->height()}
					};
				renderer().draw_box(outline, colors::red(), sdl::Fill::outline);
			}

			for (BodyPart const& part : body) {
				for (ScreenSpace::Box const& region : part.regions()) {
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
					colors::Color color{red, green, 0.0f, 1.0f};
					renderer().draw_box(region.translated(body.offset_to_center()), colors::black(), color);
				}
			}
		}, solid_program());
	}
}
