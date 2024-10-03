#pragma once

#include "../../../sdk/vfunc/vfunc.hpp"
#include "../../modules/modules.hpp"

class i_network_client {
public:
	int get_client_tick( ) {
		return vmt::call_virtual<int>( this, 5 );
	}

	int get_server_tick( ) {
		return vmt::call_virtual<int>( this, 6 );
	}

	const char* get_map_name( ) {
		return *reinterpret_cast<const char**>( reinterpret_cast<std::uintptr_t>( this ) + 0x240 );
	}

	bool is_connected( ) {
		return std::string_view{ get_map_name( ) }.find( "maps/<empty>.vpk" ) == std::string::npos;
	}

	void set_prediction( bool value ) {
		*(bool*)( std::uintptr_t( this ) + 0xF8 ) = value;
	}

	void set_delta_tick( int tick ) {
		*(int*)( std::uintptr_t( this ) + 0x25C ) = tick;
	}

	OFFSET( m_predicted, bool, 0xF8 );
	OFFSET( m_delta_tick, int, 0x25C );
};

class i_network_client_service
{
public:
	i_network_client* get_network_client( ) {
		return vmt::call_virtual<i_network_client*>( this, 23 );
	}
};