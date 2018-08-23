//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/human.hpp"
#include "entities/beings/body.hpp"
#include "entities/beings/body_part.hpp"

using namespace units;

namespace ql {
	human::human(const std::function<uptr<ql::agent>(being&)>& make_agent, ql::id<being> id)
		: corporeal_being_base<human>{make_agent, id, make_body(id), [] {
			return ql::stats
				{ spirit{0.0}
				, health_regen{0.001}
				, strength{0.0}
				, endurance{120.0}
				, stamina{120.0}
				, agility{0.0}
				, stealth{100.0}
				, vision
					{ acuity{100.0}
					, ideal_illuminance{100.0}
					, darkness_tolerance{25.0}
					, glare_tolerance{100.0}
					}
				, hearing{80.0}
				, intellect{0.0}
				, weight{0.0}
				, min_temp{-100.0}
				, max_temp{100.0}
				, mute{true}
				, dmg::protect::zero()
				, dmg::resist::zero()
				, dmg::vuln::zero()
				};
			}
		}
	{}

	human::human(std::istream& in) : corporeal_being_base<human>{in, make_body(ql::id<being>{999})} {} //! @todo using ID 999 here to allow compilation. Will need to extract ID from stream.

	void human::serialize(std::ostream& out) const {
		being::serialize(out);
		
		out << std::endl;
	}

	body human::make_body(ql::id<being> owner_id) {
		// Hands

		class human_hand : public hand {
		public:
			human_hand(ql::id<being> owner_id) : hand
				{ owner_id
				, ql::vitality{15.0}
				, ql::weight{5.0}
				, ql::dexterity{120.0}
				, {}
				}
			{}
			int layer() const final { return 3; };
		protected:
		};
		class human_left_hand : public human_hand {
		public:
			using human_hand::human_hand;
			view_space::polygon const& hitbox() const final {
				static auto result = view_space::polygon
					{ view_space::point{-10.0f, -5.0f}
					, view_space::point{-7.0f, 10.0f}
					, view_space::point{7.0f, 10.0f}
					, view_space::point{10.0f, -5.0f}
					};
				return result;
			}
		};
		class human_right_hand : public human_hand {
		public:
			using human_hand::human_hand;
			view_space::polygon const& hitbox() const final {
				static auto result = view_space::polygon
					{ view_space::point{-10.0f, -5.0f}
					, view_space::point{-7.0f, 10.0f}
					, view_space::point{7.0f, 10.0f}
					, view_space::point{10.0f, -5.0f}
					};
				return result;
			}
		};

		// Arms

		class human_arm : public arm {
		public:
			human_arm(ql::id<being> owner_id, std::vector<uptr<attachment>> attachments) : arm
				{ owner_id
				, ql::vitality{25.0}
				, ql::weight{10.0}
				, ql::strength{15.0}
				, std::move(attachments)
				}
			{}
			view_space::polygon const& hitbox() const final {
				static auto result = view_space::polygon
					{ view_space::point{-7.0f, -5.0f}
					, view_space::point{-7.0f, 55.0f}
					, view_space::point{7.0f, 55.0f}
					, view_space::point{7.0f, -5.0f}
					};
				return result;
			}
			int layer() const final { return 2; };
		};
		class left_arm_attachment : public attachment {
		public:
			using attachment::attachment;
			virtual view_space::vector offset() const final { return view_space::vector{0.0f, 55.0f}; }
			virtual view_space::radians rotation() const final { return view_space::radians::zero(); }
			uptr<body_part> default_part(ql::id<being> owner_id) const final { return umake<human_left_hand>(owner_id); }
		};
		class human_left_arm : public human_arm {
		public:
			human_left_arm(ql::id<being> owner_id) : human_arm
				{ owner_id
				, make_uptr_vector<attachment>(umake<left_arm_attachment>(nullptr))
				}
			{}
		};
		class right_arm_attachment : public attachment {
		public:
			using attachment::attachment;
			virtual view_space::vector offset() const final { return view_space::vector{0.0f, 55.0f}; }
			virtual view_space::radians rotation() const final { return view_space::radians::zero(); }
			uptr<body_part> default_part(ql::id<being> owner_id) const final { return umake<human_right_hand>(owner_id); }
		};
		class human_right_arm : public human_arm {
		public:
			human_right_arm(ql::id<being> owner_id) : human_arm
				{ owner_id
				, make_uptr_vector<attachment>(umake<right_arm_attachment>(nullptr))
				}
			{}
		};

		// Feet

		class human_foot : public foot {
		public:
			human_foot(ql::id<being> owner_id) : foot
				{ owner_id
				, ql::vitality{15.0}
				, ql::weight{5.0}
				, ql::agility{15.0}
				, {}
				}
			{}
			int layer() const final { return 1; };
		protected:
		};
		class human_left_foot : public human_foot {
		public:
			using human_foot::human_foot;
			view_space::polygon const& hitbox() const final {
				static auto result = view_space::polygon
					{ view_space::point{-10.0f, 10.0f}
					, view_space::point{-10.0f, -5.0f}
					, view_space::point{20.0f, 10.0f}
					};
				return result;
			}
		};
		class human_right_foot : public human_foot {
		public:
			using human_foot::human_foot;
			view_space::polygon const& hitbox() const final {
				static auto result = view_space::polygon
					{ view_space::point{10.0f, -5.0f}
					, view_space::point{10.0f, 10.0f}
					, view_space::point{-20.0f, 10.0f}
					};
				return result;
			}
		};

		// Legs

		class human_leg : public leg {
		public:
			human_leg(ql::id<being> owner_id, std::vector<uptr<attachment>> attachments) : leg
				{ owner_id
				, ql::vitality{25.0}
				, ql::weight{10.0}
				, ql::agility{35.0}
				, ql::strength{15.0}
				, std::move(attachments)
				}
			{}
			view_space::polygon const& hitbox() const final {
				static auto result = view_space::polygon
					{ view_space::point{-7.0f, -5.0f}
					, view_space::point{-7.0f, 65.0f}
					, view_space::point{7.0f, 65.0f}
					, view_space::point{7.0f, -5.0f}
					};
				return result;
			}
			int layer() const final { return 0; };
		};
		class left_leg_attachment : public attachment {
		public:
			using attachment::attachment;
			virtual view_space::vector offset() const final { return view_space::vector{0.0f, 60.0f}; }
			virtual view_space::radians rotation() const final { return view_space::radians::zero(); }
			uptr<body_part> default_part(ql::id<being> owner_id) const final { return umake<human_left_foot>(owner_id); }
		};
		class human_left_leg : public human_leg {
		public:
			human_left_leg(ql::id<being> owner_id) : human_leg
				{ owner_id
				, make_uptr_vector<attachment>(umake<left_leg_attachment>(nullptr))
				}
			{}
		};
		class right_leg_attachment : public attachment {
		public:
			using attachment::attachment;
			virtual view_space::vector offset() const final { return view_space::vector{0.0f, 60.0f}; }
			virtual view_space::radians rotation() const final { return view_space::radians::zero(); }
			uptr<body_part> default_part(ql::id<being> owner_id) const final { return umake<human_right_foot>(owner_id); }
		};
		class human_right_leg : public human_leg {
		public:
			human_right_leg(ql::id<being> owner_id) : human_leg
				{ owner_id
				, make_uptr_vector<attachment>(umake<right_leg_attachment>(nullptr))
				}
			{}
		};

		// Head

		class human_head : public head {
		public:
			human_head(ql::id<being> owner_id) : head
				{ owner_id
				, ql::vitality{25.0}
				, ql::weight{5.0}
				, ql::intellect{100.0}
				, ql::spirit{50.0}
				, ql::mute{false}
				, {}
				}
			{}
			view_space::polygon const& hitbox() const final {
				static auto result = view_space::polygon
					{ view_space::point{-10.0f, 0.0f}
					, view_space::point{-10.0f, -20.0f}
					, view_space::point{0.0f, -22.0f}
					, view_space::point{10.0f, -20.0f}
					, view_space::point{10.0f, 0.0f}
					, view_space::point{0.0f, 2.0f}
					};
				return result;
			}
			int layer() const final { return 4; };
		protected:
		};

		// Torso

		class human_torso_attachment_head : public attachment {
		public:
			using attachment::attachment;
			virtual view_space::vector offset() const final { return view_space::vector{0.0f, -25.0f}; }
			virtual view_space::radians rotation() const final { return view_space::radians::zero(); }
			uptr<body_part> default_part(ql::id<being> owner_id) const final { return umake<human_head>(owner_id); }
		};
		class human_torso_attachment_right_arm : public attachment {
		public:
			using attachment::attachment;
			virtual view_space::vector offset() const final { return view_space::vector{-25.0f, -25.0f}; }
			virtual view_space::radians rotation() const final { return view_space::radians::circle() / 20.0; }
			uptr<body_part> default_part(ql::id<being> owner_id) const final { return umake<human_right_arm>(owner_id); }
		};
		class human_torso_attachment_left_arm : public attachment {
		public:
			using attachment::attachment;
			virtual view_space::vector offset() const final { return view_space::vector{25.0f, -25.0f}; }
			virtual view_space::radians rotation() const final { return -view_space::radians::circle() / 20.0; }
			uptr<body_part> default_part(ql::id<being> owner_id) const final { return umake<human_left_arm>(owner_id); }
		};
		class human_torso_attachment_right_leg : public attachment {
		public:
			using attachment::attachment;
			virtual view_space::vector offset() const final { return view_space::vector{-15.0f, 25.0f}; }
			virtual view_space::radians rotation() const final { return view_space::radians::zero(); }
			uptr<body_part> default_part(ql::id<being> owner_id) const final { return umake<human_right_leg>(owner_id); }
		};
		class human_torso_attachment_left_leg : public attachment {
		public:
			using attachment::attachment;
			virtual view_space::vector offset() const final { return view_space::vector{15.0f, 25.0f}; }
			virtual view_space::radians rotation() const final { return view_space::radians::zero(); }
			uptr<body_part> default_part(ql::id<being> owner_id) const final { return umake<human_left_leg>(owner_id); }
		};
		class human_torso : public torso {
		public:
			human_torso(ql::id<being> owner_id) : torso
				{ owner_id
				, ql::vitality{50.0}
				, ql::weight{35.0}
				, ql::strength{20.0}
				, make_uptr_vector<attachment>
					( umake<human_torso_attachment_head>(nullptr)
					, umake<human_torso_attachment_right_arm>(nullptr)
					, umake<human_torso_attachment_left_arm>(nullptr)
					, umake<human_torso_attachment_right_leg>(nullptr)
					, umake<human_torso_attachment_left_leg>(nullptr)
					)
				}
			{}
			view_space::polygon const& hitbox() const final {
				static auto result = view_space::polygon
					{ view_space::point{-25.0f, -25.0f}
					, view_space::point{-15.0f, 25.0f}
					, view_space::point{15.0f, 25.0f}
					, view_space::point{25.0f, -25.0f}
					};
				return result;
			}
			int layer() const final { return 5; };
		};

		auto root = umake<human_torso>(owner_id);
		root->generate_attached_parts();
		return ql::body{*this, std::move(root)};
	}
}
