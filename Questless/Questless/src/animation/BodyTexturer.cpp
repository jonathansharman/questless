/**
* @file    BodyTexturer.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the BodyTexturer class.
*/

#include <deque>
using std::deque;
#include <type_traits>
using std::reference_wrapper;
#include <algorithm>
using std::min;
using std::max;
#include <utility>
using std::pair;
using std::make_pair;

#include "animation/BodyTexturer.h"
#include "entities/beings/BodyPart.h"
#include "sdl-wrappers/resources.h"
#include "utility/utility.h"

using namespace sdl;

namespace questless
{
	void BodyTexturer::visit(const Body& body)
	{
		Rect bounds = body.bounds();

		_texture = Texture::make(renderer(), SDL_BLENDMODE_BLEND, bounds.w, bounds.h, true);
		_texture->as_target([this, &body] {
			renderer().clear(Color::clear());
			renderer().draw_rect(Rect{0, 0, _texture->width(), _texture->height()}, Color::red(), false);

			for (auto part : body.parts()) {
				for (Rect region : part->regions()) {
					constexpr double threshold = 1.0 / 2.0;

					double health_percentage = part->health() / part->vitality();
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
