#include "../darkside.hpp"
#include "../directx/directx.hpp"
#include "../features/anti_hit/anti_hit.hpp"
#include "../features/movement/movement.hpp"
#include "../features/eng_pred/eng_pred.hpp"
#include "../entity_system/entity.hpp"
#include "../features/skins/skins.hpp"
#include "../features/visuals/world.hpp"

#include "../valve/classes/c_envy_sky.hpp"
#include "../features/visuals/visuals.hpp"
#include "../features/rage_bot/rage_bot.hpp"
#include "../features/anim_sync/anim_sync.hpp"

using namespace hooks;

bool c_hooks::initialize( ) {
	MH_Initialize( );

	g_skins->dump_items( );

	create_move::m_create_move.hook( vmt::get_v_method( g_interfaces->m_csgo_input, 5 ), create_move::hk_create_move );
	enable_cursor::m_enable_cursor.hook( vmt::get_v_method( g_interfaces->m_input_system, 78 ), enable_cursor::hk_enable_cursor );
	validate_view_angles::m_validate_view_angles.hook( vmt::get_v_method( g_interfaces->m_csgo_input, 7 ), validate_view_angles::hk_validate_view_angles );

	mouse_input_enabled::m_mouse_input_enabled.hook( vmt::get_v_method( g_interfaces->m_csgo_input, 16 ), mouse_input_enabled::hk_mouse_input_enabled );
	present::m_present.hook( g_directx->m_present_address, present::hk_present );

	update_global_vars::m_update_global_vars.hook( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 15 ? ? ? ? 48 8D 05 ? ? ? ? 48 85 D2" ), update_global_vars::hk_update_global_vars );
	frame_stage_notify::m_frame_stage_notify.hook( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 5C 24 ? 56 48 83 EC ? 8B 05 ? ? ? ? 8D 5A" ), frame_stage_notify::hk_frame_stage_notify );
	override_view::m_override_view.hook( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 5C 24 10 55 48 8D 6C 24 A9 48 81 EC B0 00 00 00 48 8B 01" ), override_view::hk_override_view );

	on_add_entity::m_on_add_entity.hook( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 74 24 10 57 48 83 EC 20 48 8B F9 41 8B C0 B9" ), on_add_entity::hk_on_add_entity );
	on_remove_entity::m_on_remove_entity.hook( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 74 24 10 57 48 83 EC 20 48 8B F9 41 8B C0 25" ), on_remove_entity::hk_on_remove_entity );

	on_level_init::m_on_level_init.hook( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 5C 24 18 56 48 83 EC 30 48" ), on_level_init::hk_on_level_init );
	on_level_shutdown::m_on_level_shutdown.hook( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 83 EC ? 48 8B 0D ? ? ? ? 48 8D 15 ? ? ? ? 45 33 C9 45 33 C0 48 8B 01 FF 50 ? 48 85 C0 74 ? 48 8B 0D ? ? ? ? 48 8B D0 4C 8B 01 41 FF 50 ? 48 83 C4" ), on_level_shutdown::hk_on_level_shutdown );

	update_sky_box::m_update_sky_box.hook( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 8B C4 48 89 58 18 48 89 70 20 55 57 41 54 41 55" ), update_sky_box::hk_update_sky_box );

	draw_light_scene::m_draw_light_scene.hook( g_opcodes->scan( g_modules->m_modules.scenesystem_dll.get_name( ), "48 89 54 24 10 53 55 41" ), draw_light_scene::hk_draw_light_scene );

	update_aggregate_scene_object::m_update_aggregate_scene_object.hook( g_opcodes->scan( g_modules->m_modules.scenesystem_dll.get_name( ), "48 89 5C 24 18 48 89 6C 24 20 56 57 41 55" ), update_aggregate_scene_object::hk_update_aggregate_scene_object );
	draw_aggregate_scene_object::m_draw_aggregate_scene_object.hook( g_opcodes->scan( g_modules->m_modules.scenesystem_dll.get_name( ), "4C 89 44 24 ? 48 89 54 24 ? 55 57 48 8D AC 24" ), draw_aggregate_scene_object::hk_draw_aggregate_scene_object );

	update_post_processing::m_update_post_processing.hook( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 5C 24 08 57 48 83 EC 60 80" ), update_post_processing::hk_update_post_processing );

	should_update_sequences::m_should_update_sequences.hook( g_opcodes->scan( g_modules->m_modules.animation_system.get_name( ), "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 49 8B 40 48" ), should_update_sequences::hk_should_update_sequences );

	//xref: FirstpersonLegsPrepass
	should_draw_legs::m_should_draw_legs.hook( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 01" ), should_draw_legs::hk_should_draw_legs );

	mark_interp_latch_flags_dirty::m_mark_interp_latch_flags_dirty.hook( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "85 D2 0F 84 ? ? ? ? 56 57 48 83 EC ? 8B 05" ), mark_interp_latch_flags_dirty::hk_mark_interp_latch_flags_dirty );

	draw_scope_overlay::m_draw_scope_overlay.hook( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "40 56 57 48 83 EC 68" ), draw_scope_overlay::hk_draw_scope_overlay );

	get_field_of_view::m_get_field_of_view.hook( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "40 53 48 81 EC 80 00 00 00 48 8B D9 E8 ?? ?? ?? ?? 48 85" ), get_field_of_view::hk_get_field_of_view );
	
	LOG_INFO( xorstr_( "[+] Hooks initialization completed!" ) );
	return true;
}

void c_hooks::destroy( ) {
	hooks::enable_cursor::unhook( );
	g_directx->unitialize( );

	create_move::m_create_move.unhook( );
	enable_cursor::m_enable_cursor.unhook( );
	validate_view_angles::m_validate_view_angles.unhook( );
	mouse_input_enabled::m_mouse_input_enabled.unhook( );
	present::m_present.unhook( );
	update_global_vars::m_update_global_vars.unhook( );
	frame_stage_notify::m_frame_stage_notify.unhook( );
	on_add_entity::m_on_add_entity.unhook( );
	on_remove_entity::m_on_remove_entity.unhook( );
	on_level_init::m_on_level_init.unhook( );
	on_level_shutdown::m_on_level_shutdown.unhook( );
	update_sky_box::m_update_sky_box.unhook( );
	draw_light_scene::m_draw_light_scene.unhook( );
	update_aggregate_scene_object::m_update_aggregate_scene_object.unhook( );
	draw_aggregate_scene_object::m_draw_aggregate_scene_object.unhook( );
	update_post_processing::m_update_post_processing.unhook( );
	should_update_sequences::m_should_update_sequences.unhook( );
	should_draw_legs::m_should_draw_legs.unhook( );
	mark_interp_latch_flags_dirty::m_mark_interp_latch_flags_dirty.unhook( );
	draw_scope_overlay::m_draw_scope_overlay.unhook( );
	get_field_of_view::m_get_field_of_view.unhook( );

	MH_Uninitialize( );
}

void __fastcall hooks::create_move::hk_create_move( i_csgo_input* rcx, int slot, bool active ) {
	static auto original = m_create_move.get_original< decltype( &hk_create_move ) >( );
	original( rcx, slot, active );

	g_ctx->m_user_cmd = rcx->get_user_cmd( );

	if ( !g_ctx->m_local_pawn || !g_ctx->m_local_controller || !g_ctx->m_user_cmd )
		return original( rcx, slot, active );

	g_ctx->m_user_cmd->pb.mutable_base( )->clear_subtick_moves( );

	const vec3_t old_view_angles = {
		g_ctx->m_user_cmd->pb.mutable_base( )->viewangles( ).x( ),
		g_ctx->m_user_cmd->pb.mutable_base( )->viewangles( ).y( ),
		g_ctx->m_user_cmd->pb.mutable_base( )->viewangles( ).z( )
	};

	g_menu->on_create_move( );

	g_movement->on_create_move( g_ctx->m_user_cmd, old_view_angles.y );

	g_anti_hit->on_create_move( g_ctx->m_user_cmd );

	g_prediction->run( );

	g_rage_bot->on_create_move( );

	g_prediction->end( );

	g_movement->movement_fix( g_ctx->m_user_cmd, old_view_angles );
}

bool hooks::mouse_input_enabled::hk_mouse_input_enabled( void* ptr ) {
	static auto original = m_mouse_input_enabled.get_original< decltype( &hk_mouse_input_enabled ) >( );
	return g_menu->m_opened ? false : original( ptr );
}

void* hooks::enable_cursor::hk_enable_cursor( void* rcx, bool active ) {
	static auto original = m_enable_cursor.get_original< decltype( &hk_enable_cursor ) >( );

	m_enable_cursor_input = active;
	if ( g_menu->m_opened )
		active = false;

	return original( rcx, active );
}

void hooks::enable_cursor::unhook( ) {
	static auto original = m_enable_cursor.get_original< decltype( &hk_enable_cursor ) >( );

	original( g_interfaces->m_input_system, m_enable_cursor_input );
}

HRESULT hooks::present::hk_present( IDXGISwapChain* swap_chain, unsigned int sync_interval, unsigned int flags ) {
	static auto original = m_present.get_original< decltype( &hk_present ) >( );

	g_directx->start_frame( swap_chain );

	g_directx->new_frame( );
	{
		g_world->skybox( );
		g_world->exposure( g_ctx->m_local_pawn );
		g_render->update_background_drawlist( ImGui::GetBackgroundDrawList( ) );
		g_visuals->on_present( );
		g_world->draw_scope_overlay( );
		g_menu->draw( );
	}
	g_directx->end_frame( );

	return original( swap_chain, sync_interval, flags );
}

void hooks::validate_view_angles::hk_validate_view_angles( i_csgo_input* input, void* a2 ) {
	static auto original = m_validate_view_angles.get_original< decltype( &hk_validate_view_angles ) >( );

	vec3_t view_angles = input->get_view_angles( );

	original( input, a2 );

	input->set_view_angles( view_angles );
}

void hooks::update_global_vars::hk_update_global_vars( void* source_to_client, void* new_global_vars ) {
	static auto original = m_update_global_vars.get_original< decltype( &hk_update_global_vars ) >( );

	original( source_to_client, new_global_vars );

	g_interfaces->m_global_vars = *reinterpret_cast<i_global_vars**>( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), xorstr_( "48 89 0D ? ? ? ? 48 89 41" ), 0x3 ) );
}

void* hooks::frame_stage_notify::hk_frame_stage_notify( void* source_to_client, int stage ) {
	static auto original = m_frame_stage_notify.get_original< decltype( &hk_frame_stage_notify ) >( );

	g_ctx->m_local_pawn = g_interfaces->m_entity_system->get_local_pawn( );
	g_ctx->m_local_controller = g_interfaces->m_entity_system->get_base_entity<c_cs_player_controller>( g_interfaces->m_engine->get_local_player_index( ) );

	g_skins->agent_changer( stage );

	original( source_to_client, stage );

	switch ( stage ) {
	case FRAME_RENDER_START:
		g_skins->knife_changer( stage );
		break;
	case FRAME_RENDER_END:
		g_skins->knife_changer( stage );
		break;
	case FRAME_NET_UPDATE_END:
		g_rage_bot->store_records( );
		g_anim_sync->on_frame_stage( );
		break;
	case FRAME_SIMULATE_END:
		g_visuals->store_players( );
		break;
	default:
		break;
	}

	return original( source_to_client, stage );
}

vec3_t calculate_camera_pos( vec3_t anchor_pos, float distance, vec3_t view_angles ) {
	float yaw = DirectX::XMConvertToRadians( view_angles.y );
	float pitch = DirectX::XMConvertToRadians( view_angles.x );

	float x = anchor_pos.x + distance * cosf( yaw ) * cosf( pitch );
	float y = anchor_pos.y + distance * sinf( yaw ) * cosf( pitch );
	float z = anchor_pos.z + distance * sinf( pitch );

	return vec3_t{ x, y, z };
}

inline vec3_t calculate_angles( vec3_t view_pos, vec3_t aim_pos ) {
	vec3_t angle = { 0, 0, 0 };

	vec3_t delta = aim_pos - view_pos;

	angle.x = -asin( delta.z / delta.length( ) ) * _rad_pi;
	angle.y = atan2( delta.y, delta.x ) * _rad_pi;

	return angle;
}

void* hooks::override_view::hk_override_view( void* source_to_client, c_view_setup* view_setup ) {
	static auto original = m_override_view.get_original< decltype( &hk_override_view ) >( );

	original( source_to_client, view_setup );

	c_cs_player_pawn* local_player = g_ctx->m_local_pawn;

	if ( !local_player )
		return original( source_to_client, view_setup );

	bool in_third_person = false;
	if ( g_key_handler->is_pressed( g_cfg->world_esp.m_thirdperson_key_bind, g_cfg->world_esp.m_thirdperson_key_bind_style ) )
		in_third_person = true;

	if ( in_third_person && local_player->is_alive( ) )
	{
		vec3_t adjusted_cam_view_angle = g_interfaces->m_csgo_input->get_view_angles( );
		adjusted_cam_view_angle.x = -adjusted_cam_view_angle.x;
		view_setup->m_origin = calculate_camera_pos( local_player->get_eye_pos( ), -g_cfg->world_esp.m_distance, adjusted_cam_view_angle );

		ray_t ray{};
		game_trace_t trace{};
		trace_filter_t filter{ 0x1C3003, local_player, NULL, 4 };

		vec3_t direction = ( local_player->get_eye_pos( ) - view_setup->m_origin ).normalize( );

		vec3_t temp = view_setup->m_origin + direction * -10.f;

		if ( g_interfaces->m_trace->trace_shape( &ray, local_player->get_eye_pos( ), view_setup->m_origin, &filter, &trace ) ) {
			if ( trace.m_hit_entity != nullptr )
				view_setup->m_origin = trace.m_pos + ( direction * 10.f );
		}

		vec3_t pos = calculate_angles( view_setup->m_origin, local_player->get_eye_pos( ) ).normalize_angle( );
		view_setup->m_angles = vec3_t{ pos.x, pos.y };
	}

	return original( source_to_client, view_setup );
}

void* hooks::on_add_entity::hk_on_add_entity( void* a1, c_entity_instance* entity_instance, int handle ) {
	static auto original = m_on_add_entity.get_original< decltype( &hk_on_add_entity ) >( );

	g_entity_system->add_entity( entity_instance, handle );

	return original( a1, entity_instance, handle );
}

void* hooks::on_remove_entity::hk_on_remove_entity( void* a1, c_entity_instance* entity_instance, int handle ) {
	static auto original = m_on_remove_entity.get_original< decltype( &hk_on_remove_entity ) >( );

	g_entity_system->remove_entity( entity_instance, handle );

	return original( a1, entity_instance, handle );
}

void* hooks::on_level_init::hk_on_level_init( void* a1, const char* map_name ) {
	static auto original = m_on_level_init.get_original< decltype( &hk_on_level_init ) >( );

	g_entity_system->level_init( );

	return original( a1, map_name );
}

void* hooks::on_level_shutdown::hk_on_level_shutdown( void* a1, const char* map_name ) {
	static auto original = m_on_level_shutdown.get_original< decltype( &hk_on_level_shutdown ) >( );

	g_entity_system->level_shutdown( );

	return original( a1, map_name );
}

void* hooks::update_sky_box::hk_update_sky_box( c_env_sky* sky ) {
	static auto original = m_update_sky_box.get_original< decltype( &hk_update_sky_box ) >( );

	g_world->skybox( sky );

	return original( sky );
}

void* hooks::draw_light_scene::hk_draw_light_scene( void* a1, c_scene_light_object* a2, __int64 a3 ) {
	static auto original = m_draw_light_scene.get_original< decltype( &hk_draw_light_scene ) >( );

	g_world->lighting( a2 );

	return original( a1, a2, a3 );
}

void* hooks::update_aggregate_scene_object::hk_update_aggregate_scene_object( c_aggregate_scene_object* a1, void* a2 ) {
	static auto original = m_update_aggregate_scene_object.get_original< decltype( &hk_update_aggregate_scene_object ) >( );

	void* result = original( a1, a2 );

	c_byte_color color = ( g_cfg->world.m_wall * 255 ).to_byte( );

	int count = a1->m_count;

	for ( int i = 0; i < count; i++ ) {
		c_aggregate_scene_object_data* scene_object = &a1->m_array[ i ];
		if ( !scene_object )
			continue;

		scene_object->m_color = color;
	}

	return result;
}

enum e_model_type : int
{
	MODEL_SUN,
	MODEL_EFFECTS,
	MODEL_OTHER,
};

int get_model_type( const std::string_view& name ) {
	if ( name.find( "sun" ) != std::string::npos
		|| name.find( "clouds" ) != std::string::npos )
		return MODEL_SUN;

	if ( name.find( "effects" ) != std::string::npos )
		return MODEL_EFFECTS;

	return MODEL_OTHER;
}

void* hooks::draw_aggregate_scene_object::hk_draw_aggregate_scene_object( void* a1, void* a2, c_base_scene_data* a3, int a4, int a5, void* a6, void* a7, void* a8 ) {
	static auto original = m_draw_aggregate_scene_object.get_original< decltype( &hk_draw_aggregate_scene_object ) >( );

	int type = get_model_type( a3->m_material->get_name( ) );
	c_byte_color color{};

	switch ( type ) {
	case MODEL_SUN:
		color = ( g_cfg->world.m_sky_clouds * 255 ).to_byte( );
		break;
	case MODEL_EFFECTS:
		color = ( g_cfg->world.m_sky_clouds * 255 ).to_byte( );
		break;
	case MODEL_OTHER:
		color = ( g_cfg->world.m_wall * 255 ).to_byte( );
		break;
	}

	for ( int i = 0; i < a4; ++i )
	{
		auto scene = &a3[ i ];
		if ( scene ) {
			scene->r = color.r;
			scene->g = color.g;
			scene->b = color.b;
			scene->a = 255;
		}
	}

	original( a1, a2, a3, a4, a5, a6, a7, a8 );

	return original( a1, a2, a3, a4, a5, a6, a7, a8 );
}

void* hooks::update_post_processing::hk_update_post_processing( c_post_processing_volume* a1, int a2 ) {
	static auto original = m_update_post_processing.get_original< decltype( &hk_update_post_processing ) >( );

	original( a1, a2 );

	g_world->exposure( a1 );

	return original( a1, a2 );
}

void* hooks::should_update_sequences::hk_should_update_sequences( void* a1, void* a2, void* a3 ) {
	static auto original = m_should_update_sequences.get_original< decltype( &hk_should_update_sequences ) >( );

	const char* model_name = *reinterpret_cast<const char**>( reinterpret_cast<std::uintptr_t>( a1 ) + 0x8 );
	if ( model_name == nullptr )
		return original( a1, a2, a3 );

	std::string str_model_name = std::string( model_name );
	if ( str_model_name.starts_with( "weapons/models/knife/" ) )
		*reinterpret_cast<__int64*>( reinterpret_cast<std::uintptr_t>( a3 ) + 0x30 ) = 0;

	return original( a1, a2, a3 );
}

void* hooks::should_draw_legs::hk_should_draw_legs( void* a1, void* a2, void* a3, void* a4, void* a5 ) {
	static auto original = m_should_draw_legs.get_original< decltype( &hk_should_draw_legs ) >( );

	return nullptr;
}

void hooks::mark_interp_latch_flags_dirty::hk_mark_interp_latch_flags_dirty( void* a1, unsigned int a2 ) {
	return;
}

void hooks::draw_scope_overlay::hk_draw_scope_overlay( void* a1, void* a2 ) {
	
}

float hooks::get_field_of_view::hk_get_field_of_view( void* a1 ) {
	return 100.f;
}