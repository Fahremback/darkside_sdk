#pragma once

#include "../../../sdk/vfunc/vfunc.hpp"

enum EClientFrameStage : int
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE,
	FRAME_RENDER_START,
	FRAME_RENDER_END,
	FRAME_NET_UPDATE_END,
	FRAME_NET_CREATION,
	FRAME_SIMULATE_END
};

class c_networked_client_info {
	std::byte pad_001[ 0x4 ];
public:
	int m_render_tick;
	float m_render_tick_fraction;
	int m_player_tick_count;
	float m_player_tick_fraction;
private:
	std::byte pad_002[ 0x4 ];
public:
	struct {
	private:
		std::byte pad_022[ 0xC ];
	public:
		vec3_t m_eye_pos;
	} *m_local_data;
private:
	std::byte pad_003[ 0x8 ];
};

class i_engine_client {
public:
	bool is_in_game( ) {
		return vmt::call_virtual<bool>( this, 35 );
	}

	bool is_connected( ) {
		return vmt::call_virtual<bool>( this, 36 );
	}

	int get_local_player_index( ) {
		int idx = -1;

		vmt::call_virtual<void>( this, 47, &idx, 0 );
		return idx + 1;
	}

	c_networked_client_info* get_networked_client_info( ) {
		c_networked_client_info client_info;

		vmt::call_virtual<void*>( this, 176, &client_info );
		return &client_info;
	}
};