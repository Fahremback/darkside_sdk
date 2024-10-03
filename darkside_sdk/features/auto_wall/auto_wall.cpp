#include "auto_wall.hpp"

void c_auto_wall::scale_damage( int hitgroup, c_cs_player_pawn* entity, c_cs_weapon_base_v_data* weapon_data, float& damage ) {
	static convar_t* mp_damage_scale_ct_head = g_interfaces->m_var->get_by_name( "mp_damage_scale_ct_head" );
	static convar_t* mp_damage_scale_t_head = g_interfaces->m_var->get_by_name( "mp_damage_scale_t_head" );
	static convar_t* mp_damage_scale_ct_body = g_interfaces->m_var->get_by_name( "mp_damage_scale_ct_body" );
	static convar_t* mp_damage_scale_t_body = g_interfaces->m_var->get_by_name( "mp_damage_scale_t_body" );

	float damage_scale_ct_head = mp_damage_scale_ct_head->get_float( );
	float damage_scale_t_head = mp_damage_scale_t_head->get_float( );
	float damage_scale_ct_body = mp_damage_scale_ct_body->get_float( );
	float damage_scale_t_body = mp_damage_scale_t_body->get_float( );

	const bool is_ct = entity->m_team_num( ) == 3, is_t = entity->m_team_num( ) == 2;

	float head_damage_scale = is_ct ? damage_scale_ct_head : is_t ? damage_scale_t_head : 1.0f;
	const float body_damage_scale = is_ct ? damage_scale_ct_body : is_t ? damage_scale_t_body : 1.0f;

	switch ( hitgroup )
	{
	case hitgroup_head:
		damage *= weapon_data->m_headshot_multiplier( ) * head_damage_scale;
		break;
	case hitgroup_chest:
	case hitgroup_left_hand:
	case hitgroup_right_hand:
	case hitgroup_neck:
		damage *= body_damage_scale;
		break;
	case hitgroup_stomach:
		damage *= 1.25f * body_damage_scale;
		break;
	case hitgroup_left_leg:
	case hitgroup_right_leg:
		damage *= 0.75f * body_damage_scale;
		break;
	default:
		break;
	}

	if ( !entity->has_armor( hitgroup ) )
		return;

	float heavy_armor_bonus = 1.0f, armor_bonus = 0.5f, armor_ratio = weapon_data->m_armor_ratio( ) * 0.5f;

	float damage_to_health = damage * armor_ratio;
	const float damage_to_armor = ( damage - damage_to_health ) * ( heavy_armor_bonus * armor_bonus );

	if ( damage_to_armor > static_cast<float>( entity->m_armor_value( ) ) )
	{
		damage_to_health = damage - static_cast<float>( entity->m_armor_value( ) ) / armor_bonus;
	}

	damage = damage_to_health;
}

bool c_auto_wall::fire_bullet( vec3_t start, vec3_t end, c_cs_player_pawn* target, c_cs_weapon_base_v_data* weapon_data, penetration_data_t& pen_data, bool is_taser ) {
	c_cs_player_pawn* local_player = g_ctx->m_local_pawn;

	if ( !local_player )
		return false;

	trace_data_t trace_data = trace_data_t( );
	trace_data.arr_pointer = &trace_data.arr;

	vec3_t direction = end - start;
	vec3_t end_pos = direction * weapon_data->m_range( );

	const trace_filter_t filter( 0x1C300B, local_player, nullptr, 3 );
	g_interfaces->m_trace->create_trace( &trace_data, start, end_pos, filter, 4 );

	handle_bulllet_penetration_data_t handle_bullet_data = handle_bulllet_penetration_data_t(
		static_cast<float>( weapon_data->m_damage( ) ),
		weapon_data->m_penetration( ),
		weapon_data->m_range( ),
		weapon_data->m_range_modifier( ),
		4,
		false
	);

	float max_range = weapon_data->m_range( );
	float trace_length = 0.0f;
	pen_data.m_damage = static_cast<float>( weapon_data->m_damage( ) );

	for ( int i = 0; i < trace_data.num_update; i++ ) {
		update_value_t* modulate_values = reinterpret_cast<update_value_t* const>( reinterpret_cast<std::uintptr_t>( trace_data.pointer_update_value ) + i * sizeof( update_value_t ) );

		game_trace_t game_trace;
		g_interfaces->m_trace->init_trace_info( &game_trace );
		g_interfaces->m_trace->get_trace_info( &trace_data, &game_trace, 0.0f, reinterpret_cast<void*>( reinterpret_cast<std::uintptr_t>( trace_data.arr.data( ) ) + sizeof( trace_array_element_t ) * ( modulate_values->m_handle_index & ENT_ENTRY_MASK ) ) );

		max_range -= trace_length;

		if ( game_trace.m_fraction == 1.0f )
			break;

		trace_length += game_trace.m_fraction * max_range;
		pen_data.m_damage *= std::powf( weapon_data->m_range_modifier( ), trace_length / 500.f );

		if ( trace_length > 3000.f )
			break;

		if ( game_trace.m_hit_entity && game_trace.m_hit_entity->is_player_pawn( ) ) {

			if ( pen_data.m_damage < 1.0f )
				continue;

			scale_damage( game_trace.get_hit_group( ), game_trace.m_hit_entity, weapon_data, pen_data.m_damage );

			pen_data.m_hitbox = game_trace.get_hitbox_id( );
			pen_data.m_penetrated = i == 0;

			if ( is_taser && pen_data.m_penetrated )
				break;

			return true;
		}

		if ( handle_bullet_penetration( &trace_data, &handle_bullet_data, modulate_values, false ) )
			return false;

		pen_data.m_damage = handle_bullet_data.m_damage;
	}

	return false;
}
