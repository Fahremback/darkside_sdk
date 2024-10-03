#pragma once

#include "../schema/schema.hpp"
#include "../../sdk/typedefs/c_handle.hpp"

class c_player_camera_service {
public:
	SCHEMA( m_active_post_processing, c_base_handle, "CPlayer_CameraServices", "m_hActivePostProcessingVolume" );
	SCHEMA( m_view_punch_angle, vec3_t, "CPlayer_CameraServices", "m_vecCsViewPunchAngle" );
};