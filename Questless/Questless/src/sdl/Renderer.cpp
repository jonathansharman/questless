/**
* @file    Renderer.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for Renderer, a simple wrapper class around SDL_Renderer.
*/

#include "sdl/Renderer.h"

using std::vector;
using std::runtime_error;
using namespace std::string_literals;

using namespace units;

namespace sdl
{
	Renderer::Renderer(Window& window, int width, int height) : _w{width}, _h{height}, _target{nullptr}
	{
		_renderer = SDL_CreateRenderer(window.sdl_ptr(), -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
		if (_renderer == nullptr) {
			throw runtime_error{"Failed to create renderer."};
		}

		if (SDL_RenderSetLogicalSize(_renderer, width, height)) {
			throw runtime_error{"Failed to set renderer's logical size."};
		}

		if (SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND)) {
			throw runtime_error{"Failed to set renderer blend mode."};
		}

		if (SDL_GetRendererInfo(_renderer, &_info)) {
			throw runtime_error{"Failed to get renderer info."};
		}
	}

	Renderer::~Renderer()
	{
		SDL_DestroyRenderer(_renderer);
	}

	SDL_Texture* Renderer::target() const
	{
		return _target;
	}

	void Renderer::target(SDL_Texture* target)
	{
		if (SDL_SetRenderTarget(_renderer, target)) {
			throw runtime_error("Failed to set render target.");
		} else {
			_target = target;
		}
	}

	void Renderer::clear(Color color)
	{
		set_draw_color(color);
		SDL_RenderClear(_renderer);
	}

	void Renderer::draw_lines(vector<ScreenPoint> points, Color color)
	{
		set_draw_color(color);
		// This reinterpret_cast is safe because SDL_Point and ScreenPoint have the same data structure.
		SDL_RenderDrawLines(_renderer, reinterpret_cast<SDL_Point*>(&points[0]), points.size());
	}

	void Renderer::draw_rect(ScreenRect const& rect, Color color, bool filled)
	{
		set_draw_color(color);

		// This reinterpret_cast is safe because SDL_Rect and ScreenRect have the same data structure.
		SDL_Rect const* sdl_rect = reinterpret_cast<const SDL_Rect*>(&rect);

		if (filled) {
			SDL_RenderFillRect(_renderer, sdl_rect);
		} else {
			SDL_RenderDrawRect(_renderer, sdl_rect);
		}
	}

	void Renderer::draw_rect(ScreenRect const& rect, Color border_color, Color fill_color)
	{
		SDL_Rect sdl_rect{rect.x, rect.y, rect.w, rect.h};

		set_draw_color(border_color);
		SDL_RenderDrawRect(_renderer, &sdl_rect);
		set_draw_color(fill_color);
		sdl_rect.w -= 2;
		sdl_rect.h -= 2;
		++sdl_rect.x;
		++sdl_rect.y;
		SDL_RenderFillRect(_renderer, &sdl_rect);
	}

	void Renderer::set_draw_color(Color color)
	{
		if (SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a)) {
			std::string message = "Failed to set renderer draw color. SDL error: "s + SDL_GetError();
			throw runtime_error{message};
		}
	}
}
