#pragma once

#include "../schema/schema.hpp"
#include "../../sdk/typedefs/c_handle.hpp"

class c_entity_instance;

class c_cs_player_controller : public c_entity_instance {
public:
	SCHEMA( m_player_name, const char*, "CCSPlayerController", "m_sSanitizedPlayerName" );
	SCHEMA( m_tick_base, int, "CBasePlayerController", "m_nTickBase" );
	SCHEMA( m_pawn, c_base_handle, "CBasePlayerController", "m_hPawn" );
	SCHEMA( m_pawn_is_alive, bool, "CCSPlayerController", "m_bPawnIsAlive" );

	void physics_run_think( ) {
		static auto physics_run_think_ = reinterpret_cast<void* ( __fastcall* )( void* )>( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), "E8 ? ? ? ? 49 8B D6 48 8B CE E8 ? ? ? ? 48 8B 06", 0x1 ) );
		physics_run_think_( this );
	}
};