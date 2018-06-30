//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "magic/spell.hpp"

#include "agents/agent.hpp"
#include "entities/beings/being.hpp"
#include "game.hpp"

namespace ql::magic
{
	uptr<action> spell::cast(uptr<spell> spell)
	{
		class cast : public action
		{
		public:
			cast(uptr<magic::spell> spell) : _spell{std::move(spell)} {}
			std::string name() const final { return "cast " + _spell->name(); }
			complete perform(being& actor, cont cont) final
			{
				if (auto gatestone = the_game().items.ptr_as<ql::gatestone>(_spell->_gatestone_id)) {
					return _spell->perform_cast(actor, *gatestone, cont);
				}
			}
		private:
			uptr<magic::spell> _spell;
		};

		return umake<cast>(std::move(spell));
	}
	
	double spell::incant_time(being& caster) const
	{
		return base_incant_time() / (1.0 + being::intellect_factor * caster.stats.intellect);
	}
}
