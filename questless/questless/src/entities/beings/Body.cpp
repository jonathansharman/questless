/**
* @file    Body.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Body class.
*/

#include <deque>
using std::deque;
#include <functional>
using std::function;
#include <type_traits>
using std::reference_wrapper;
#include <algorithm>
using std::min;
using std::max;
#include <memory>
using std::make_shared;

#include "entities/beings/Body.h"
#include "entities/beings/BodyPart.h"

using namespace sdl;

using std::shared_ptr;
using std::make_shared;

namespace questless
{
	Body::Body(std::unique_ptr<BodyPart> root) : _root{std::move(root)}, _parts_count{0}
	{
		int x_min = 0;
		int y_min = 0;
		int x_max = 0;
		int y_max = 0;
		for (auto part : parts()) {
			++_parts_count;
			for (Rect region : part->regions()) {
				x_min = min(x_min, region.x);
				x_max = max(x_max, region.right());
				y_min = min(y_min, region.y);
				y_max = max(y_max, region.bottom());
			}
		}
		_bounds = {x_min, y_min, x_max - x_min + 1, y_max - y_min + 1};
		_offset_to_center = {-x_min, -y_min};
	}
	Body::~Body() = default;

	Stream<BodyPart*> Body::parts()
	{
		/// @todo Look into the efficiency of this implementation. (Seems bad?)

		auto work_list = make_shared<deque<reference_wrapper<BodyPart>>>();
		work_list->push_back(*_root);

		shared_ptr<function<Cell<BodyPart*>()>> lambda = make_shared<function<Cell<BodyPart*>()>>();
		*lambda = [lambda, work_list] {
			BodyPart& part = work_list->front();
			work_list->pop_front();
			for (const BodyPart::ptr& child : part.children()) {
				work_list->push_back(*child);
			}
			return work_list->empty()
				? Cell<BodyPart*>{&part}
				: Cell<BodyPart*>{&part, Stream<BodyPart*>{[lambda] { return (*lambda)(); }}};
		};

		return Stream<BodyPart*>{*lambda};
	}

	Stream<const BodyPart*> Body::parts() const
	{
		/// @todo Make this a template along with the non-const version to avoid code duplication.

		auto work_list = make_shared<deque<reference_wrapper<const BodyPart>>>();
		work_list->push_back(*_root);

		shared_ptr<function<Cell<const BodyPart*>()>> generator = make_shared<function<Cell<const BodyPart*>()>>();
		*generator = [generator, work_list] {
			const BodyPart& part = work_list->front();
			work_list->pop_front();
			for (const BodyPart::ptr& child : part.children()) {
				work_list->push_back(*child);
			}
			return work_list->empty()
				? Cell<const BodyPart*>{&part}
				: Cell<const BodyPart*>{&part, Stream<const BodyPart*>{[generator] { return (*generator)(); }}};
		};

		return Stream<const BodyPart*>{*generator};
	}
}
