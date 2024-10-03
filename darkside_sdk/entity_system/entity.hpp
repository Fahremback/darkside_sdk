#pragma once

#include "../darkside.hpp"

#define ENTITY_ALL 1
#define ENTITY_ENEMIES_ONLY 2
#define ENTITY_TEAMMATES_ONLY 3

using player_controller_pair = std::pair<c_cs_player_controller*, c_cs_player_pawn*>;

class c_entity_system {
    std::unordered_map<uint64_t, std::vector<c_entity_instance*>> entities;
public:
    void initialize( );

    void level_init( );
    void level_shutdown( );
    void add_entity( c_entity_instance*, int );
    void remove_entity( c_entity_instance*, int );

    std::vector<c_entity_instance*> get( const char* );
    std::vector<c_cs_player_pawn*> get_players( int );
    std::vector<player_controller_pair> get_players_with_controller( int );
};

inline const auto g_entity_system = std::make_unique<c_entity_system>( );