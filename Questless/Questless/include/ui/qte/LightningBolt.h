/**
* @file    QteLightningBolt.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Quick time event to determine lightning bolt strength.
*/

#ifndef QTE_LIGHTNING_BOLT_H
#define QTE_LIGHTNING_BOLT_H

#include <string>
#include <vector>
#include <functional>

#include "../Dialog.h"
#include "utility/PointF.h"
#include "utility/Initializer.h"

namespace questless::qte
{
	class LightningBolt : public Dialog
	{
	public:
		LightningBolt(std::function<void(double)> cont);

		bool update(const sdl::Input& input) override;

		void draw(const sdl::Window& window) override;
	private:
		struct PointCharge
		{
			PointF pos;
			double charge;
		};

		friend class Initializer<LightningBolt>;
		static Initializer<LightningBolt> _initializer;
		static void initialize();

		static sdl::Handle<sdl::Texture> LightningBolt::_point_charge_texture_handle;

		Continuation<double> _cont;

		std::vector<PointCharge> _point_charges;

		sdl::Texture::ptr _txt_prompt;

		void refresh() override { load_textures(); }

		void load_textures();
	};
}

#endif