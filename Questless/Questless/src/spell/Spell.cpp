//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "spell/Spell.h"
#include "entities/beings/Being.h"
#include "agents/Agent.h"

namespace questless::spell
{
	uptr<Action> Spell::cast(uptr<Spell> spell)
	{
		class Cast : public Action
		{
		public:
			Cast(uptr<Spell> spell) : _spell{std::move(spell)} {}
			std::string name() const final { return "Cast " + _spell->name(); }
			Complete perform(Being& actor, cont_t cont) final { return _spell->perform_cast(actor, cont); }
		private:
			uptr<Spell> _spell;
		};

		return std::make_unique<Cast>(std::move(spell));
	}
	
	double Spell::incant_time(Being& caster) const
	{
		return base_incant_time() / (1.0 + Being::intellect_factor * caster.stats.intellect);
	}
}
