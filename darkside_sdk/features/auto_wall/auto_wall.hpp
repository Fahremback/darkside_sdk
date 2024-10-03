#pragma once

#include "../../darkside.hpp"
#include "../../valve/interfaces/vtables/i_trace.hpp"

struct penetration_data_t {
	float m_damage{ };
	uint8_t m_hitgroup{ };

	int m_hitbox{ };

	bool m_penetrated{ true };
};

struct handle_bulllet_penetration_data_t {
	handle_bulllet_penetration_data_t( const float damage, const float pen, const float range_mod, const float range, const int pen_count, const bool failed ) :
		m_damage( damage ),
		m_penetration( pen ),
		m_range_modifier( range_mod ),
		m_range( range ),
		m_pen_count( pen_count ),
		m_failed( failed ) 
	{}

	float m_damage{ };
	float m_penetration{ };
	float m_range_modifier{ };
	float m_range{ };
	int m_pen_count{ };
	bool m_failed{ };
};

enum e_hitgroups
{
	hitgroup_gear,
	hitgroup_head,
	hitgroup_chest,
	hitgroup_stomach,
	hitgroup_left_hand,
	hitgroup_right_hand,
	hitgroup_left_leg,
	hitgroup_right_leg,
	hitgroup_neck
};

class c_auto_wall {
	using fn_handle_bullet_penetration = bool( __fastcall* ) ( trace_data_t*, handle_bulllet_penetration_data_t*, update_value_t*, void*, void*, void*, void*, void*, bool );

	inline bool handle_bullet_penetration( trace_data_t* const trace, handle_bulllet_penetration_data_t* stats, update_value_t* const mod_value, const bool draw_showimpacts ) {
		static fn_handle_bullet_penetration fn = reinterpret_cast<fn_handle_bullet_penetration>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name(), xorstr_( "48 8B C4 44 89 48 20 55 57 41 55" ) ) );

		return fn( trace, stats, mod_value, nullptr, nullptr, nullptr, nullptr, nullptr, draw_showimpacts );
	}
public:
	void scale_damage( int hitgroup, c_cs_player_pawn* entity, c_cs_weapon_base_v_data* weapon_data, float& damage );
	bool fire_bullet( vec3_t start, vec3_t end, c_cs_player_pawn* target, c_cs_weapon_base_v_data* weapon_data, penetration_data_t& pen_data, bool is_taser = false );
};

const auto g_auto_wall = std::make_unique<c_auto_wall>( );