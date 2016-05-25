/**
* @file    Scroll.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Scroll class.
*/

#ifndef SCROLL_H
#define SCROLL_H

#include <memory>
using std::unique_ptr;

#include "Item.h"
#include "spells/Spell.h"

namespace questless
{
	namespace ScrollK
	{
		const double weight = 0.1;
	}

	class Scroll : public Item
	{
	public:
		Scroll(Spell::ptr spell) : _spell{std::move(spell)} {}

		void accept(ItemVisitor& visitor) const override { return visitor.visit(*this); }

		std::string name() const override { return blank() ? "Blank Scroll" : "Scroll"; }

		double weight() const override { return ScrollK::weight; }

		bool blank() const { return _spell == nullptr; }

		Spell& spell() { return *_spell; }
		const Spell& spell() const { return *_spell; }

		std::vector<Action::ptr> actions() override
		{
			std::vector<Action::ptr> actions;
			if (!blank()) {
				if (_spell->charges() > 0) {
					actions.push_back(Cast::make(*this));
				}
				if (auto max_charges = _spell->max_charges()) {
					if (_spell->charges() < *max_charges) {
						actions.push_back(Incant::make(*this));
					}
				}
				if (_spell->charges() > 0) {
					actions.push_back(Discharge::make(*this));
				}
			}
			actions.push_back(Drop::make(*this));
			actions.push_back(Throw::make(*this));
			return actions;
		}

		void update() override { _spell->update(); }

		/// @todo COOLDOWNS. Spell will need to be modified as well.
	private:
		class Cast : public Action
		{
		public:
			Cast(Scroll& scroll) : _scroll{scroll} {}

			static ptr make(Scroll& item) { return std::make_unique<Cast>(item); }

			std::string name() const override { return "Cast"; }

			void perform(Game& game, Being& actor, cont_t cont) override
			{
				_scroll.spell().perform(game, actor, std::move(cont));
			}
		private:
			Scroll& _scroll;
		};

		class Incant : public Action
		{
		public:
			Incant(Scroll& scroll) : _scroll{scroll} {}

			static ptr make(Scroll& item) { return std::make_unique<Incant>(item); }

			std::string name() const override { return "Incant"; }

			void perform(Game& game, Being& actor, cont_t cont) override
			{
				/// @todo Costs, times, etc.
				_scroll.spell().charge();
				cont(Result::success);
			}
		private:
			Scroll& _scroll;
		};

		class Discharge : public Action
		{
		public:
			Discharge(Scroll& scroll) : _scroll{scroll} {}

			static ptr make(Scroll& item) { return std::make_unique<Discharge>(item); }

			std::string name() const override { return "Discharge"; }

			void perform(Game& game, Being& actor, cont_t cont) override
			{
				/// @todo Costs, times, etc.
				_scroll.spell().discharge();
				cont(Result::success);
			}
		private:
			Scroll& _scroll;
		};

		Spell::ptr _spell;
	};
}

#endif
