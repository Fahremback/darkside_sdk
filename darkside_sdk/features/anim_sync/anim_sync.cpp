#include "anim_sync.hpp"
#include "../../entity_system/entity.hpp"

void c_anim_sync::set_playback_rate( c_base_anim_graph* base_anim_graph ) {

}

void c_anim_sync::on_frame_stage( ) {
	for ( auto entity : g_entity_system->get_players( ENTITY_ENEMIES_ONLY ) ) {
		if ( !entity || !entity->is_alive( ) )
			continue;

		c_body_component* body_component = entity->m_body_component( );
		if ( !body_component )
			continue;

		c_base_anim_graph* base_anim_graph = body_component->get_base_anim_graph_controller( );
		if ( !base_anim_graph )
			continue;

		if ( !( entity->m_flags( ) & FL_ONGROUND ) )
			this->on_sequence_update( base_anim_graph );
	}
}

void c_anim_sync::on_sequence_update( c_base_anim_graph* base_anim_graph ) {
	int new_sequence = 0;
	int sequence = base_anim_graph->m_sequence( );

	float finished_cycle_rate = get_finished_cycle_rate( base_anim_graph );

	base_anim_graph->write_new_sequence( new_sequence );
	bool diff_btw_sequences = sequence != new_sequence;

	if ( base_anim_graph->m_sequence( ) != sequence )
		base_anim_graph->m_sequence( ) = sequence;

	base_anim_graph->m_cached_sequence_cycle_rate( ) = finished_cycle_rate;
	base_anim_graph->m_sequence_finished( ) = false;

	if ( base_anim_graph->m_playback_rate( ) == 0.0f ) {
		if ( base_anim_graph->m_seq_fixed_cycle( ) != g_interfaces->m_global_vars->m_tick_count )
			base_anim_graph->m_seq_fixed_cycle( ) = g_interfaces->m_global_vars->m_tick_count;
	}
	else {
		if ( base_anim_graph->m_seq_fixed_cycle( ) != 0.0 )
			base_anim_graph->m_seq_fixed_cycle( ) = 0.0;

		float seq_cycle_rate = base_anim_graph->m_cached_sequence_cycle_rate( );
		float playback_rate = base_anim_graph->m_playback_rate( );
		if ( seq_cycle_rate != 0.0 )
		{
			if ( playback_rate < 0.0 )
				g_interfaces->m_global_vars->m_tick_count--;
			sequence = sequence - ( g_interfaces->m_global_vars->m_tick_count / playback_rate );
		}
		if ( playback_rate != base_anim_graph->m_seq_start_time( ) )
			base_anim_graph->m_seq_start_time( ) = playback_rate;
	}
}

float c_anim_sync::get_finished_cycle_rate( c_base_anim_graph* a1 ) {
	static auto some_poop_func = reinterpret_cast<bool( __fastcall* )( void* )>( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), "E8 ? ? ? ? 84 C0 75 ? 0F 2F F7 72", 0x1 ) );

	auto& v3 = a1->m_seq_fixed_cycle( );
	auto& v5 = a1->m_playback_rate( );
	auto v6 = 0;

	if ( v5 ) {
		v3 = ( ( v3 - a1->m_seq_start_time( ) ) * a1->m_cached_sequence_cycle_rate( ) ) * v5;
		if ( v5 < 0.0 )
			v3 = v3 + 1.0;
	}

	if ( some_poop_func( a1 ) ) {
		if ( v5 < 0.0f )
			return v3;
		else
			return v3 - v3;
	}
	else {
		if ( v3 >= 0.0 )
			return fminf( v3, 1.0 );
		return v6;
	}
}