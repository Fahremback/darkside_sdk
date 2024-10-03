#include "i_trace.hpp"

#include "../../classes/c_cs_player_pawn.hpp"

surface_data_t* game_trace_t::get_surface_data_t( ) {
    using fn_get_surface_data_t = std::uint64_t( __fastcall* )( void* );
	static fn_get_surface_data_t o_get_surface_data = reinterpret_cast<fn_get_surface_data_t>( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), "E8 ? ? ? ? 48 85 C0 74 ? 44 38 60", 0x1 ) );

    return reinterpret_cast<surface_data_t*>( o_get_surface_data( m_surface ) );
}

int game_trace_t::get_hitbox_id( ) {
    if ( m_hitbox_data )
        return m_hitbox_data->m_hitbox_id;

    return 0;
}

int game_trace_t::get_hit_group( ) {
    if ( m_hitbox_data )
        return m_hitbox_data->m_hit_group;

    return 0;
}

trace_filter_t::trace_filter_t( std::uint64_t mask, c_cs_player_pawn* entity, c_cs_player_pawn* player, int layer ) {
	m_mask = mask;
	m_ptr[ 0 ] = m_ptr[ 1 ] = 0;
	m_ptr2 = 7;
	m_ptr3 = layer;
	m_ptr4 = 0x49;
	m_ptr5 = 0;

	if ( entity != nullptr )
	{
		m_skip_handles[ 0 ] = entity->get_handle( ).get_entry_index( );
		m_skip_handles[ 2 ] = entity->get_owner_handle_index( );
		m_arr_collisions[ 0 ] = entity->m_collision( )->get_collision_mask( );
	}

	if ( player != nullptr )
	{
		m_skip_handles[ 0 ] = player->get_handle( ).get_entry_index( );
		m_skip_handles[ 2 ] = player->get_owner_handle_index( );
		m_arr_collisions[ 0 ] = player->m_collision( )->get_collision_mask( );
	}
}