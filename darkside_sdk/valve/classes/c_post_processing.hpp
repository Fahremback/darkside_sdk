#pragma once

#include "../schema/schema.hpp"
#include "../../sdk/typedefs/c_handle.hpp"
#include "../../sdk/typedefs/c_color.hpp"

class c_post_processing_volume {
public:
	SCHEMA( m_exposure_control, bool, "C_PostProcessingVolume", "m_bExposureControl" );
	SCHEMA( m_min, float, "C_PostProcessingVolume", "m_flMinExposure" );
	SCHEMA( m_max, float, "C_PostProcessingVolume", "m_flMaxExposure" );
	SCHEMA( m_fade_speed_up, float, "C_PostProcessingVolume", "m_flExposureFadeSpeedUp" );
	SCHEMA( m_fade_speed_down, float, "C_PostProcessingVolume", "m_flExposureFadeSpeedDown" );
};