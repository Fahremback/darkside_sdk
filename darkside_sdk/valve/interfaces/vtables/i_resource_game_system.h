#pragma once

#include "../../modules/modules.hpp"
#include "../../../utils/utils.hpp"

class c_base_entity;
class c_cs_player_pawn;
class c_cs_player_controller;

class i_entity_system {
public:
	template <class C = c_base_entity>
	C* get_base_entity( int index ) {
		static auto get_client_entity = reinterpret_cast<C * ( __fastcall* )( i_entity_system*, int )>( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), "E8 ? ? ? ? 48 8B F8 48 85 C0 74 ? C6 44 24", 0x1 ) );

		return get_client_entity( this, index );
	}

	c_cs_player_pawn* get_local_pawn( ) {
		static auto fn = reinterpret_cast<c_cs_player_pawn * ( __fastcall* )( int )>( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), "E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 83 7B", 0x1 ) );
		return fn( 0 );
	}

	c_cs_player_controller* get_local_controller( ) {
		static auto local_controller = reinterpret_cast<c_cs_player_controller**>( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), "E8 ?? ?? ?? ?? 48 85 C0 74 12 8B CB", 0x1 ) );
		return local_controller[ 0 ];
	}

	int get_highest_entiy_index( ) {
		static auto fn = reinterpret_cast<int( __thiscall* )( void*, int* )>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "8B 81 10 15 00" ) );

		int value = -1;
		fn( this, &value );

		return value;
	}

	c_cs_player_controller* get_local_controller_from_pawn( c_cs_player_pawn* pawn ) {
		static auto local_controller = reinterpret_cast<c_cs_player_controller * ( __fastcall* )( c_cs_player_pawn* )>( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), "E8 ? ? ? ? 48 8B F8 48 85 C0 74 ? 48 8B 48 ? E8 ? ? ? ? 48 8D 55", 0x1 ) );
		return local_controller( pawn );
	}
};