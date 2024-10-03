#include "eng_pred.hpp"

void c_eng_pred::run( ) {
	if ( !g_interfaces->m_engine->is_in_game( ) || !g_interfaces->m_engine->is_connected( ) || !g_ctx->m_local_pawn->is_alive( ) )
		return;

	c_user_cmd* user_cmd = g_ctx->m_user_cmd;
	if ( !user_cmd )
		return;

	i_network_client* network_client = g_interfaces->m_network_client_service->get_network_client( );
	if ( !network_client )
		return;

	if ( !g_interfaces->m_global_vars )
		return;

	/* store all predicted info there */
	m_pred_data.m_absolute_frame_start_time_std_dev = g_interfaces->m_global_vars->m_absolute_frame_start_time_std_dev;
	m_pred_data.m_absolute_frame_time = g_interfaces->m_global_vars->m_absolute_frame_time;
	m_pred_data.m_current_time = g_interfaces->m_global_vars->m_current_time;
	m_pred_data.m_tick_base = g_ctx->m_local_controller->m_tick_base( );

	if ( auto active_weapon = g_ctx->m_local_pawn->get_active_weapon( ) ) {
		active_weapon->update_accuracy_penality( );

		m_pred_data.m_spread = active_weapon->get_spread( );
		m_pred_data.m_inaccuracy = active_weapon->get_inaccuracy( );
	}

	this->predict( );
}

void c_eng_pred::predict( ) {
	c_networked_client_info* client_info = g_interfaces->m_engine->get_networked_client_info( );
	if ( !client_info )
		return;

	if ( !client_info->m_local_data )
		return;

	m_pred_data.m_eye_pos = client_info->m_local_data->m_eye_pos;
	m_pred_data.m_player_tick = client_info->m_player_tick_count;
	m_pred_data.m_player_tick_fraction = client_info->m_player_tick_fraction;
	m_pred_data.m_render_tick = client_info->m_render_tick;
	m_pred_data.m_render_tick_fraction = client_info->m_render_tick_fraction;

	if ( m_pred_data.m_player_tick_fraction > ( 1.f - 0.0099999998f ) )
		m_pred_data.m_tick_base++;

	g_ctx->m_local_pawn->m_movement_services( )->set_prediction_command( g_ctx->m_user_cmd );

	static auto process_movement = reinterpret_cast<void* ( __fastcall* )( void*, c_user_cmd* )>( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), "E8 ? ? ? ? 48 8B 06 48 8B CE FF 90 ? ? ? ? 44 38 63", 0x1 ) );

	/* premium fixes */
	g_ctx->m_local_controller->physics_run_think( );

	m_initialized = true;
}

void c_eng_pred::end( ) {
	if ( !g_interfaces->m_engine->is_in_game( ) || !g_interfaces->m_engine->is_connected( ) || !g_ctx->m_local_pawn->is_alive( ) )
		return;

	if ( !m_initialized )
		return;

	i_network_client* network_client = g_interfaces->m_network_client_service->get_network_client( );
	if ( !network_client )
		return;

	c_networked_client_info* client_info = g_interfaces->m_engine->get_networked_client_info( );
	if ( !client_info )
		return;

	if ( !client_info->m_local_data )
		return;

	static auto interpolate_shoot_position = reinterpret_cast<void* ( __fastcall* )( void*, vec3_t*, int, float )>( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), "E8 ? ? ? ? 41 8B 86 ? ? ? ? C1 E8 ? A8 ? 0F 85", 0x1 ) );

	g_ctx->m_local_pawn->m_movement_services( )->reset_prediction_command( );

	/* restore all predicted info there */
	g_interfaces->m_global_vars->m_absolute_frame_start_time_std_dev = m_pred_data.m_absolute_frame_start_time_std_dev;
	g_interfaces->m_global_vars->m_absolute_frame_time = m_pred_data.m_absolute_frame_time;
	g_interfaces->m_global_vars->m_current_time = m_pred_data.m_current_time;

	m_pred_data.m_shoot_tick = g_ctx->m_local_controller->m_tick_base( ) - 1;
	interpolate_shoot_position( g_ctx->m_local_pawn->m_weapon_services( ), &m_pred_data.m_eye_pos, m_pred_data.m_shoot_tick, 0.0099999998f );

	g_ctx->m_local_controller->m_tick_base( ) = m_pred_data.m_tick_base;
}