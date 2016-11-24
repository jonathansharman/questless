/**
* @file    MessageDialog.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the MagnitudeDialog class.
*/

#include "ui/qte/LightningBolt.h"

#include "utility/utility.h"

using namespace sdl;

namespace
{
	constexpr int charges_count = 30;
}

namespace questless::qte
{
	sdl::Handle<sdl::Texture> LightningBolt::_point_charge_texture_handle;

	Initializer<LightningBolt> LightningBolt::_initializer;
	void LightningBolt::initialize()
	{
		_point_charge_texture_handle = sdl::texture_manager().add([] { return Texture::make("resources/textures/glow.png", renderer(), SDL_BLENDMODE_BLEND); });
	}

	LightningBolt::LightningBolt(std::function<void(double)> cont) : _cont{ std::move(cont) }
	{
		load_textures();
		for (int i = 0; i < charges_count; ++i) {
			_point_charges.push_back(PointCharge{{uniform(0.0, 1000.0), uniform(0.0, 1000.0)}, 0.0});
		}
	}

	bool LightningBolt::update(const Input& input)
	{
		if (input.presses(SDLK_BACKSPACE) || input.presses(SDLK_ESCAPE)) {
			return _cont(1.0);
		}
		return false;
	}

	void LightningBolt::draw(const Window& window)
	{
		int x_center = window.center().x;

		// Draw point charges.
		for (auto&& point_charge : _point_charges) {
			texture_manager()[_point_charge_texture_handle].draw_transformed
				( point_charge.pos.to_point()
				, boost::none
				, 1.0 + point_charge.charge
				, 1.0 + point_charge.charge
				, false
				, false
				, Color::yellow());
		}

		// Draw the prompt.
		renderer().draw_rect(Rect{x_center - _txt_prompt->width() / 2, _prompt_top, _txt_prompt->width(), _txt_prompt->height()}, Color{0, 0, 0, 128}, true);
		_txt_prompt->draw(Point{x_center, _prompt_top}, HAlign::center);
	}

	void LightningBolt::load_textures()
	{
		_txt_prompt = Texture::make(font_manager()[prompt_font_handle()].render("Build up a charge!", renderer(), Color::white()));
	}
}
