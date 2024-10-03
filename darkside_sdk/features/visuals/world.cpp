#include "world.hpp"
#include "../../entity_system/entity.hpp"
#include "../../render/render.hpp"

void c_world::skybox( ) {
    static auto update_sky_box = reinterpret_cast<void( __fastcall* )( void* )>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 5C 24 08 57 48 83 EC 30 48 8B F9 E8 ?? ?? ?? ?? 48 8B 47" ) );

    bool dsiable_fog_rendering = g_cfg->world.m_render_fog;

    c_color skybox_clr = g_cfg->world.m_sky;

    static bool prev_disable_fog_rendering = !dsiable_fog_rendering;

    static c_color prev_skybox_clr( -1, -1, -1 );

    if ( prev_skybox_clr == skybox_clr && prev_disable_fog_rendering == dsiable_fog_rendering )
        return;

    prev_disable_fog_rendering = dsiable_fog_rendering;

    prev_skybox_clr = skybox_clr;

    const auto& sky = g_entity_system->get( "C_EnvSky" );
    for ( c_entity_instance* entity : sky ) {
        c_env_sky* env_sky = reinterpret_cast<c_env_sky*>( entity );

        skybox( env_sky );

        update_sky_box( env_sky );
    }
}

void c_world::skybox( c_env_sky* env_sky ) {
    bool disable_fog_rendering = g_cfg->world.m_render_fog;

    c_color skybox_clr = g_cfg->world.m_sky;

    c_byte_color color = ( skybox_clr * 255 ).to_byte( );

    env_sky->m_tint_color( ) = color;

    env_sky->m_brightness_scale( ) = disable_fog_rendering ? 0.f : 1.f;
}

void c_world::lighting( c_scene_light_object* light_object ) {
    c_color lighintg_color = g_cfg->world.m_lighting;

    light_object->m_color = lighintg_color * 3;
}

void c_world::exposure( c_cs_player_pawn* pawn ) {
    if ( !g_interfaces->m_engine->is_in_game( ) )
        return;

    static auto update_exposure = reinterpret_cast<void( __fastcall* )( c_player_camera_service*, int )>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 5C 24 08 57 48 83 EC 20 8B FA 48 8B D9 E8 ?? ?? ?? ?? 84 C0 0F 84" ) );

    static float prev_exposure = -1;

    float exposure = static_cast<float>( g_cfg->world.m_exposure );

    if ( prev_exposure == exposure )
        return;

    if ( !pawn || !pawn->m_camera_services( ) )
        return;

    c_player_camera_service* camera_service = pawn->m_camera_services( );
    if ( !camera_service )
        return;

    c_post_processing_volume* post_processing_volume = reinterpret_cast<c_post_processing_volume*>( g_interfaces->m_entity_system->get_base_entity( camera_service->m_active_post_processing( ).get_entry_index( ) ) );
    if ( !post_processing_volume )
        return;

    prev_exposure = exposure;

    this->exposure( post_processing_volume );
    update_exposure( camera_service, 0 );
}

void c_world::exposure( c_post_processing_volume* post_processing ) {
    float exposure = g_cfg->world.m_exposure * 0.01f;

    post_processing->m_exposure_control( ) = true;

    post_processing->m_fade_speed_down( ) = post_processing->m_fade_speed_up( ) = 0;

    post_processing->m_min( ) = post_processing->m_max( ) = exposure;
}

void c_world::draw_scope_overlay( ) {
    if ( !g_interfaces->m_engine->is_in_game( ) )
        return;

    c_cs_player_pawn* local_player = g_ctx->m_local_pawn;

    if ( !local_player || !local_player->is_alive( ) || !local_player->m_scoped( ) )
        return;

    vec3_t m_screen_size = g_render->m_screen_size;

    g_render->line( vec3_t( 0, m_screen_size.y / 2 ), vec3_t( m_screen_size.x, m_screen_size.y / 2 ), c_color( 0, 0, 0, 1.f ) );
    g_render->line( vec3_t( m_screen_size.x / 2, 0 ), vec3_t( m_screen_size.x / 2, m_screen_size.y ), c_color( 0, 0, 0, 1.f ) );
}