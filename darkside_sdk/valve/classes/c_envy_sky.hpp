#pragma once

#include "../schema/schema.hpp"
#include "../../sdk/typedefs/c_handle.hpp"
#include "../../sdk/typedefs/c_color.hpp"

#include "c_cs_player_pawn.hpp"

class c_env_sky : public c_base_entity {
public:
	SCHEMA( m_tint_color, c_byte_color, "C_EnvSky", "m_vTintColor" );
	SCHEMA( m_brightness_scale, float, "C_EnvSky", "m_flBrightnessScale" );
	SCHEMA( m_fog_type, int, "C_EnvSky", "m_nFogType" );
};