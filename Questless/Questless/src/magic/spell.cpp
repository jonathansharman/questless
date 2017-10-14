//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "magic/spell.h"
#include "entities/beings/being.h"
#include "agents/agent.h"

namespace ql::magic
{
	uptr<action> spell::cast(uptr<spell> spell)
	{
		class cast : public action
		{
		public:
			cast(uptr<magic::spell> spell) : _spell{std::move(spell)} {}
			std::string name() const final { return "cast " + _spell->name(); }
			complete perform(being& actor, cont cont) final { return _spell->perform_cast(actor, cont); }
		private:
			uptr<magic::spell> _spell;
		};

		return std::make_unique<cast>(std::move(spell));
	}
	
	double spell::incant_time(being& caster) const
	{
		return base_incant_time() / (1.0 + being::intellect_factor * caster.stats.intellect);
	}
}
