#pragma once

#include "../../darkside.hpp"

#include "../../valve/classes/c_envy_sky.hpp"
#include "../../valve/classes/c_post_processing.hpp"
#include "../../valve/classes/c_scene_light_obj.hpp"

class c_world {
public:
	void skybox( );
	void skybox( c_env_sky* );

	void lighting( c_scene_light_object* );

	void exposure( c_cs_player_pawn* );
	void exposure( c_post_processing_volume* );
	void draw_scope_overlay( );
};

inline const auto g_world = std::make_unique<c_world>( );