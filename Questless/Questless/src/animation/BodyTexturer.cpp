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
#include "sdl-wrappers/resources.h"

using namespace sdl;

namespace questless
{
	pair<Rect, Vector> bounds_and_offset(const BodyPart& part)
	{
		int x_min = 0;
		int y_min = 0;
		int x_max = 0;
		int y_max = 0;

		deque<reference_wrapper<const BodyPart>> work_queue;
		work_queue.push_back(part);

		while (!work_queue.empty()) {
			// Pop next part from the work queue.
			const BodyPart& p = work_queue.front();
			work_queue.pop_front();

			// Add its children to the work queue.
			for (const auto& child : p.children()) {
				work_queue.push_back(*child);
			}

			// Adjust bounds.
			for (Rect region : p.regions()) {
				x_min = min(x_min, region.x);
				x_max = max(x_max, region.right());
				y_min = min(y_min, region.y);
				y_max = max(y_max, region.bottom());
			}
		}
		Rect bounds{x_min, y_min, x_max - x_min + 1, y_max - y_min + 1};
		Vector offset{-x_min, -y_min};
		return make_pair(bounds, offset);
	}

	void BodyTexturer::visit(const BodyPart& part)
	{
		auto b_o = bounds_and_offset(part);
		Rect bounds = b_o.first;
		Vector offset = b_o.second;

		_texture = Texture::make(renderer(), SDL_BLENDMODE_BLEND, bounds.w, bounds.h, true);
		_texture->as_target([this, &part, offset] {
			renderer().clear(Color::clear());
			renderer().draw_rect(Rect{0, 0, _texture->width(), _texture->height()}, Color::red(), false);

			std::vector<Color> color_sequence{Color::red(), Color::orange(), Color::yellow(), Color::green(), Color::blue(), Color::purple()};
			size_t i = 0;

			deque<reference_wrapper<const BodyPart>> work_queue;
			work_queue.push_back(part);

			while (!work_queue.empty()) {
				// Pop next part from the work queue.
				const BodyPart& p = work_queue.front();
				work_queue.pop_front();

				// Add its children to the work queue.
				for (const auto& child : p.children()) {
					work_queue.push_back(*child);
				}

				// Draw the part.
				for (Rect region : p.regions()) {
					//renderer().draw_rect(region, Color::black, Color::green);
					renderer().draw_rect(region.translated(offset), Color::black(), color_sequence[i % color_sequence.size()]);
				}
				++i;
			}
		});
	}
}
