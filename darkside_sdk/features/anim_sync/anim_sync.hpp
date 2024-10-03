#pragma once

#include "../../darkside.hpp"

class c_anim_sync {
	void set_playback_rate( c_base_anim_graph* base_anim_graph );
	void on_sequence_update( c_base_anim_graph* base_anim_graph );
	float get_finished_cycle_rate( c_base_anim_graph* a1 );
public:
	void on_frame_stage( );
};

inline const auto g_anim_sync = std::make_unique<c_anim_sync>( );