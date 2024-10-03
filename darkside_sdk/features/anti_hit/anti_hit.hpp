#pragma once

#include "../../darkside.hpp"

class c_anti_hit {
	bool m_should_invert{};
	void at_target( float& yaw );
	void set_pitch( c_user_cmd* user_cmd );
	void set_yaw( c_user_cmd* user_cmd );
public:
	void on_create_move( c_user_cmd* user_cmd );
};

inline const auto g_anti_hit = std::make_unique<c_anti_hit>( );