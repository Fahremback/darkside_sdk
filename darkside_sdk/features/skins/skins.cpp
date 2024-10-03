#include "skins.hpp"

void c_skins::dump_items( ) {
    auto item_schema = g_interfaces->m_client->get_econ_item_system( )->get_econ_item_schema( );
    if ( !item_schema )
        return;

    if ( !m_dumped_items.empty( ) )
        return;

    const auto& vec_items = item_schema->m_sorted_item_definition_map;
    const auto& paint_kits = item_schema->m_paint_kits;
    const auto& alternate_icons = item_schema->m_alternate_icons_map;

    for ( const auto& it : vec_items )
    {
        c_econ_item_definition* item = it.m_value;
        if ( !item )
            continue;

        const bool is_weapon = item->is_weapon( );
        const bool is_agent = item->is_agent( );
        const bool is_knife = item->is_knife( true );
        const bool is_glove = item->is_glove( true );

        const char* item_base_name = item->m_item_base_name;
        if ( !item_base_name || item_base_name[ 0 ] == '\0' )
            continue;

        if ( is_agent ) {
            m_agents.m_dumped_agent_models.emplace_back( item->get_model_name( ) );
            m_agents.m_dumped_agent_name.emplace_back( g_interfaces->m_localize->find_key( item->m_item_base_name ) );
        }

        if ( is_knife )
        {
            m_knives.m_dumped_knife_models.emplace_back( item->get_model_name( ) );
            m_knives.m_dumped_knife_name.emplace_back( g_interfaces->m_localize->find_key( item->m_item_base_name ) );
        }

        if ( is_glove ) {
            m_gloves.m_dumped_glove_models.emplace_back( item->get_model_name( ) );
            m_gloves.m_dumped_glove_name.emplace_back( g_interfaces->m_localize->find_key( item->m_item_base_name ) );
            m_gloves.m_glove_idxs.emplace_back( item->m_definition_index );
        }

        if ( !is_weapon && !is_knife && !is_glove )
            continue;

        c_dumped_item dumped_item{};
        dumped_item.m_name = g_interfaces->m_localize->find_key( item_base_name );
        dumped_item.m_def_index = item->m_definition_index;
        dumped_item.m_rarity = item->m_item_rarity;

        if ( is_knife || is_glove )
            dumped_item.m_unusual_item = true;

        for ( const auto& it : paint_kits )
        {
            c_paint_kit* paint_kit = it.m_value;
            if ( !paint_kit || paint_kit->m_id == 0 || paint_kit->m_id == 9001 )
                continue;

            const uint64_t skin_key = get_alternate_icon_key_for_weapon_paint_wear_item( item->m_definition_index, paint_kit->m_id, 0 );
            if ( alternate_icons.find_by_key( skin_key ).has_value( ) )
            {
                c_dumped_skins dumped_skin;
                dumped_skin.m_name = g_interfaces->m_localize->find_key( paint_kit->m_description_tag );
                dumped_skin.m_id = paint_kit->m_id;
                dumped_skin.m_rarity = paint_kit->m_rarity;
                dumped_item.m_dumped_skins.emplace_back( dumped_skin );
            }
        }

        if ( !dumped_item.m_dumped_skins.empty( ) && is_weapon )
            std::sort( dumped_item.m_dumped_skins.begin( ), dumped_item.m_dumped_skins.end( ), []( const c_dumped_skins& a, const c_dumped_skins& b ) { return a.m_rarity > b.m_rarity; } );

        m_dumped_items.emplace_back( dumped_item );
    }
}

void c_skins::glove_changer( c_base_view_model* view_model ) {

    c_cs_player_pawn* local_player = g_ctx->m_local_pawn;
    if ( m_gloves.m_dumped_glove_models.empty( ) )
        return;

    auto item_schema = g_interfaces->m_client->get_econ_item_system( )->get_econ_item_schema( );
    if ( !item_schema )
        return;

    c_econ_item_view* econ_glove = &local_player->m_econ_glove( );
    int currnet_model = m_gloves.m_glove_idxs[ g_cfg->skins.m_gloves.m_selected ];
    int model_skin = g_cfg->skins.m_skin_settings[ g_cfg->skins.m_selected_weapon ].m_paint_kit;

    static int last_skin = -1;
    static float last_spawn_time = -1.f;

    if ( econ_glove->m_defenition_index( ) != currnet_model || local_player->m_spawn_time_index( ) != last_spawn_time )
        econ_glove->m_defenition_index( ) = currnet_model;

    if ( model_skin != last_skin && model_skin > 0 || local_player->m_spawn_time_index( ) != last_spawn_time ) {
        static auto create_new_paint_kit = reinterpret_cast<c_paint_kit * ( __fastcall* )( c_econ_item_view* )>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 5C 24 10 56 48 83 EC 20 48 8B 01 FF 50 10 48 8B 1D ? ? ? ?" ) );

        auto paint_kit = create_new_paint_kit( econ_glove );
        if ( paint_kit && paint_kit->m_name )
        {
            last_skin = model_skin;

            auto current_paint_kit = item_schema->m_paint_kits.find_by_key( model_skin );
            if ( current_paint_kit.has_value( ) )
                paint_kit->m_name = current_paint_kit.value( )->m_name;
        }

        econ_glove->m_initialized( ) = true;
        local_player->set_body_group( );
        local_player->m_need_to_reapply_glove( ) = true;

        last_spawn_time = local_player->m_spawn_time_index( );
    }
}

void c_skins::agent_changer( int stage ) {
    auto local_player = g_ctx->m_local_pawn;
    if ( !local_player || stage != 6 )
        return;

    int team_num = local_player->m_team_num( );
    const bool is_ct = team_num == 3, is_t = team_num == 2;

    if ( is_ct )
        local_player->set_model( m_agents.m_dumped_agent_models.at( g_cfg->skins.m_agents.m_selected_ct ) );
    else if ( is_t )
        local_player->set_model( m_agents.m_dumped_agent_models.at( g_cfg->skins.m_agents.m_selected ) );
}

void c_skins::knife_changer( int stage ) {
    if ( !g_ctx->m_local_pawn || m_knives.m_dumped_knife_models.empty( ) )
        return;

    c_player_weapon_service* weapon_service = g_ctx->m_local_pawn->m_weapon_services( );
    if ( !weapon_service )
        return;

    c_cs_player_view_model_service* view_model_service = g_ctx->m_local_pawn->m_view_model_services( );
    if ( !view_model_service )
        return;

    c_base_view_model* view_model = reinterpret_cast<c_base_view_model*>( g_interfaces->m_entity_system->get_base_entity( view_model_service->m_view_model( ).get_entry_index( ) ) );
    if ( !view_model )
        return;

    if ( stage == 6 )
        glove_changer( view_model );

    c_network_utl_vector<c_base_handle>& my_weapons = weapon_service->m_weapon( );

    for ( auto i = 0; i < my_weapons.m_size; i++ )
    {
        c_econ_entity* weapon = reinterpret_cast<c_econ_entity*>( g_interfaces->m_entity_system->get_base_entity( my_weapons.m_elements[ i ].get_entry_index( ) ) );
        if ( !weapon )
            continue;

        c_attribute_container* attribute_manager = weapon->m_attribute_manager( );
        if ( !attribute_manager )
            continue;

        c_econ_item_view* item = attribute_manager->m_item( );
        if ( !item )
            continue;

        c_econ_item_definition* item_defenition = item->get_static_data( );
        if ( !item_defenition )
            continue;

        auto weapon_scene = weapon->m_scene_node( );
        if ( !weapon_scene )
            continue;

        auto view_model_scene = view_model->m_scene_node( );
        if ( !view_model_scene )
            continue;

        auto item_schema = g_interfaces->m_client->get_econ_item_system( )->get_econ_item_schema( );
        if ( !item_schema )
            return;

        bool is_knife = item_defenition->is_knife( false );

        // TO-DO: detect updated knife @opai
        int current_weapon = get_skin_config( item->m_defenition_index( ) );
        if ( !is_knife && current_weapon == CONFIG_UNKNOWN )
            continue;

        auto current_skin = &g_cfg->skins.m_skin_settings[ current_weapon ];

        if ( current_skin->m_paint_kit > 0 ) {
            auto paint_kit = item_schema->m_paint_kits.find_by_key( current_skin->m_paint_kit );
            bool use_old_model = paint_kit.has_value( ) && *reinterpret_cast<bool*>( reinterpret_cast<uintptr_t>( paint_kit.value( ) ) + 0xB2 );

            uint64_t mesh_mask = 1 + static_cast<uint64_t>( use_old_model );

            weapon->m_statrack( ) = 1337;
            weapon->m_paint_kit( ) = current_skin->m_paint_kit;

            item->m_item_id_high( ) = -1;

            weapon_scene->set_mesh_group_mask( mesh_mask );
            view_model_scene->set_mesh_group_mask( mesh_mask );
        }

        if ( is_knife && stage == 6 ) {
            int selectedKnife = g_cfg->skins.m_knives.m_selected;

            weapon->set_model( m_knives.m_dumped_knife_models.at( selectedKnife ) );
            item->m_defenition_index( ) = m_knives.m_knife_idxs.at( selectedKnife );

            if ( view_model->m_weapon( ) == weapon->get_handle( ) )
                view_model->set_model( m_knives.m_dumped_knife_models.at( selectedKnife ) );
        }
    }
}