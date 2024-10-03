#include "menu.hpp"

#include "../darkside.hpp"

#include "../features/skins/skins.hpp"

void c_menu::draw( ) {
	if ( !m_opened )
		return;

    ImGui::SetNextWindowSize( ImVec2( 512.0f, 515.0f ), ImGuiCond_Once );
    ImGui::Begin( "cs2", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse );

    ImGui::BeginChild( "tabs" , ImVec2{ 150, 0 }, true );
    {
        static constexpr const char* tabs[]{ "Rage", "Anti-aim", "World esp", "Player esp", "Misc", "Skins", "Settings" };

        for ( std::size_t i{ }; i < IM_ARRAYSIZE( tabs ); ++i ) {
            if ( ImGui::Selectable( tabs[ i ], m_selected_tab == i ) ) {
                m_selected_tab = i;
            }
        }
    }
    ImGui::EndChild( );
    ImGui::SameLine( );

    if ( m_selected_tab == 0 ) {
        ImGui::BeginChild( "ragebot", ImVec2{ 0, 0 }, true );
        {
            const char* hitboxes[] = { "head", "chest", "stomach", "arms", "legs", "feet" };
            const char* multi_points[] = { "head", "chest", "stomach", "arms", "legs", "feet" };

            ImGui::Checkbox( "enabled", &g_cfg->rage_bot.m_enabled );
            ImGui::Checkbox( "silent", &g_cfg->rage_bot.m_silent );
            ImGui::MultiCombo( "hiboxes", g_cfg->rage_bot.m_hitboxes, hitboxes, IM_ARRAYSIZE( hitboxes ) );
            ImGui::MultiCombo( "multi points", g_cfg->rage_bot.m_multi_points, multi_points, IM_ARRAYSIZE( multi_points ) );

            if ( g_cfg->rage_bot.m_multi_points[ 0 ]
                || g_cfg->rage_bot.m_multi_points[ 1 ]
                || g_cfg->rage_bot.m_multi_points[ 2 ]
                || g_cfg->rage_bot.m_multi_points[ 3 ] 
                || g_cfg->rage_bot.m_multi_points[ 4 ]
                || g_cfg->rage_bot.m_multi_points[ 5 ] ) {
                ImGui::Checkbox( "static scale", &g_cfg->rage_bot.m_static_scale );

                if ( g_cfg->rage_bot.m_static_scale ) {
                    for ( int i = 0; i < IM_ARRAYSIZE( multi_points ); i++ ) {
                        if ( g_cfg->rage_bot.m_multi_points[ i ] )
                            ImGui::SliderInt( std::format("{0} scale", multi_points[ i ] ).c_str( ), &g_cfg->rage_bot.m_hitbox_scale[ i ], 1, 100 );
                    }
                }
            }

            ImGui::SliderInt( "minimum damage", &g_cfg->rage_bot.m_minimum_damage, 1, 100 );
            ImGui::SliderInt( "damage override", &g_cfg->rage_bot.m_minimum_damage_override, 1, 100 );
            ImGui::Keybind( "damage override keybind", &g_cfg->rage_bot.m_override_damage_key_bind, &g_cfg->rage_bot.m_override_damage_key_bind_style );
            ImGui::SliderInt( "hit chance", &g_cfg->rage_bot.m_hit_chance, 1, 100 );
            ImGui::Checkbox( "auto fire", &g_cfg->rage_bot.m_auto_fire );
            ImGui::Checkbox( "auto stop", &g_cfg->rage_bot.m_auto_stop );
        }
        ImGui::EndChild( );
    }

    if ( m_selected_tab == 1 ) {
        ImGui::BeginChild( "anti-hit", ImVec2{ 0, 0 }, true );
        {
            ImGui::Checkbox( "enabled", &g_cfg->anti_hit.m_enabled );
            ImGui::Checkbox( "override pitch", &g_cfg->anti_hit.m_override_pitch );
            if ( g_cfg->anti_hit.m_override_pitch )
                ImGui::SliderInt( "##override_pitch", &g_cfg->anti_hit.m_pitch_amount, -89, 89 );
            ImGui::Checkbox( "override yaw", &g_cfg->anti_hit.m_override_yaw );
            if ( g_cfg->anti_hit.m_override_yaw ) {
                ImGui::Checkbox( "at target", &g_cfg->anti_hit.m_at_target );
                ImGui::SliderInt( "##override_yaw", &g_cfg->anti_hit.m_yaw_amount, 0, 180 );
                ImGui::SliderInt( "jiter amount", &g_cfg->anti_hit.m_jitter_amount, 0, 180 );
            }  
        }
        ImGui::EndChild( );
    }

    if ( m_selected_tab == 2 ) {
        ImGui::BeginChild( "world esp ", ImVec2{ 0, 0 }, true );
        {
            ImGui::Checkbox( "enabled thirdperson", &g_cfg->world_esp.m_enable_thirdperson );
            ImGui::SliderInt( "override distance", &g_cfg->world_esp.m_distance, 0, 180 );

            ImGui::Keybind( "thirdperson keybind", &g_cfg->world_esp.m_thirdperson_key_bind, &g_cfg->world_esp.m_thirdperson_key_bind_style );

            ImGui::Checkbox( "render fog", &g_cfg->world.m_render_fog );
            ImGui::ColorEdit4( "sky color", reinterpret_cast<float*>( &g_cfg->world.m_sky ), ImGuiColorEditFlags_NoInputs );
            ImGui::ColorEdit4( "sky and clouds color", reinterpret_cast<float*>( &g_cfg->world.m_sky_clouds ), ImGuiColorEditFlags_NoInputs );
            ImGui::ColorEdit4( "wall color", reinterpret_cast<float*>( &g_cfg->world.m_wall ), ImGuiColorEditFlags_NoInputs );
            ImGui::ColorEdit4( "lighting color", reinterpret_cast<float*>( &g_cfg->world.m_lighting ), ImGuiColorEditFlags_NoInputs );
            ImGui::SliderInt( "exposure ", reinterpret_cast<int*>( &g_cfg->world.m_exposure ), 1, 100 );
        }
        ImGui::EndChild( );
    }

    if ( m_selected_tab == 3 ) {
        ImGui::BeginChild( "player esp ", ImVec2{ 0, 0 }, true );
        {
            ImGui::Checkbox( "teammates", &g_cfg->visuals.m_player_esp.m_teammates );
            ImGui::Checkbox( "bounding box", &g_cfg->visuals.m_player_esp.m_bounding_box );
            ImGui::Checkbox( "health bar", &g_cfg->visuals.m_player_esp.m_health_bar );
            ImGui::Checkbox( "name", &g_cfg->visuals.m_player_esp.m_name );
            ImGui::Checkbox( "weapon", &g_cfg->visuals.m_player_esp.m_weapon );

            ImGui::ColorEdit4( "name color", reinterpret_cast<float*>( &g_cfg->visuals.m_player_esp.m_name_color ), ImGuiColorEditFlags_NoInputs );
        }
        ImGui::EndChild( );
    }

    if ( m_selected_tab == 4 ) {
        ImGui::BeginChild( "misc", ImVec2{ 165, 0 }, true );
        {
            
        }
        ImGui::EndChild( );
        ImGui::SameLine( );
        ImGui::BeginChild( "misc_movement", ImVec2{ 165, 0 }, true );
        {
            static constexpr const char* types[]{ "local view", "directional" };

            ImGui::Checkbox( "bunny hop", &g_cfg->misc.m_bunny_hop );
            ImGui::Checkbox( "auto strafe", &g_cfg->misc.m_auto_strafe );
            if ( g_cfg->misc.m_auto_strafe ) {
                ImGui::Text( "auto strafer smooth" );
                ImGui::SliderInt( "##auto_strafer_smooth", &g_cfg->misc.m_strafe_smooth, 1, 99 );
            }
        }
        ImGui::EndChild( );
    }

    if ( m_selected_tab == 5 ) {
        ImGui::BeginChild( "misc", ImVec2{ 0, 0 }, true );
        {
            static constexpr const char* type[]{ "weapons", "knives", "gloves", "agents" };
            ImGui::Combo( "Types", &g_cfg->skins.m_selected_type, type, IM_ARRAYSIZE( type ) );

            if ( g_cfg->skins.m_selected_type == 0 ) {
                static std::vector<const char*> weapon_names{};

                if ( weapon_names.size( ) < g_skins->m_dumped_items.size( ) )
                    for ( auto& item : g_skins->m_dumped_items )
                        weapon_names.emplace_back( item.m_name );

                ImGui::Combo( "Weapons", &g_cfg->skins.m_selected_weapon, weapon_names.data( ), weapon_names.size( ) );
                auto& selected_entry = g_cfg->skins.m_skin_settings[ g_cfg->skins.m_selected_weapon ];

                if ( ImGui::BeginListBox( "##skins" ) )
                {
                    auto& selected_weapon_entry = g_skins->m_dumped_items[ g_cfg->skins.m_selected_weapon ];

                    for ( auto& skin : selected_weapon_entry.m_dumped_skins )
                    {
                        ImGui::PushID( &skin );
                        if ( ImGui::Selectable( skin.m_name, selected_entry.m_paint_kit == skin.m_id ) )
                        {
                            if ( selected_weapon_entry.m_selected_skin == &skin )
                                selected_weapon_entry.m_selected_skin = nullptr;
                            else
                            {
                                selected_weapon_entry.m_selected_skin = &skin;
                                selected_entry.m_paint_kit = skin.m_id;
                            }
                        }
                        ImGui::PopID( );
                    }
                    ImGui::EndListBox( );
                }
            }

            if ( g_cfg->skins.m_selected_type == 1 )
                ImGui::Combo( "##knifes", &g_cfg->skins.m_knives.m_selected, g_skins->m_knives.m_dumped_knife_name.data( ), g_skins->m_knives.m_dumped_knife_name.size( ) );

            if ( g_cfg->skins.m_selected_type == 2 )
                ImGui::Combo( "##gloves", &g_cfg->skins.m_gloves.m_selected, g_skins->m_gloves.m_dumped_glove_name.data( ), g_skins->m_gloves.m_dumped_glove_name.size( ) );

            if ( g_cfg->skins.m_selected_type == 3 ) {
                ImGui::Combo( "##agents ct", &g_cfg->skins.m_agents.m_selected_ct, g_skins->m_agents.m_dumped_agent_name.data( ), g_skins->m_agents.m_dumped_agent_name.size( ) );
                ImGui::Combo( "##agents t", &g_cfg->skins.m_agents.m_selected, g_skins->m_agents.m_dumped_agent_name.data( ), g_skins->m_agents.m_dumped_agent_name.size( ) );
            }
        }
        ImGui::EndChild( );
    }

    if ( m_selected_tab == 6 ) {
        g_config_system->menu( );
    }

    ImGui::End( );
}

void c_menu::on_create_move( )
{
    if ( !m_opened )
        return;

    g_ctx->m_user_cmd->m_button_state.m_button_state &= ~( IN_ATTACK | IN_ATTACK2 );
    g_ctx->m_user_cmd->m_button_state.m_button_state2 &= ~( IN_ATTACK | IN_ATTACK2 );
    g_ctx->m_user_cmd->m_button_state.m_button_state3 &= ~( IN_ATTACK | IN_ATTACK2 );
}
