#pragma once
#include "../modules/modules.hpp"

#include "vtables/i_csgo_input.hpp"
#include "vtables/i_localize.hpp"
#include "vtables/i_resource_game_system.h"
#include "vtables/i_schema_system.hpp"
#include "../classes/c_view_setup.hpp"
#include "vtables/i_engine_client.hpp"
#include "vtables/i_convar.hpp"
#include "vtables/i_trace.hpp"
#include "vtables/i_client.hpp"
#include "vtables/i_network_game_client.hpp"
#include "vtables/i_global_vars.hpp"
#include "vtables/i_mem_alloc.hpp"

#define INTERVAL_PER_TICK 0.015625f
#define TICK_INTERVAL (INTERVAL_PER_TICK)
#define TIME_TO_TICKS(TIME) (static_cast<int>(0.5f + static_cast<float>(TIME) / TICK_INTERVAL))
#define TICKS_TO_TIME(TICKS) (TICK_INTERVAL * static_cast<float>(TICKS))

class c_interface_register
{
public:
	using create_interface_fn = void* ( * )( );
	create_interface_fn m_create_interface_fn;
	const char* m_name;
	c_interface_register* m_next;
};

inline const c_interface_register* get_register_list( c_dll* dll )
{
	auto create_interface = dll->get_export<c_address>( "CreateInterface" );

	if ( !create_interface )
		return nullptr;

	return create_interface.add( 0x3 ).relative( ).dereference( ).as<c_interface_register*>( );
}

template <typename type_t = c_interface>
inline type_t* get_interface( c_dll* dll, const char* name )
{
	const HINSTANCE module_handle = GetModuleHandle( dll->get_name( ) );
	if ( !module_handle )
		return nullptr;

	using create_interface_t = type_t * ( * )( const char*, int* );
	const create_interface_t create_interface = reinterpret_cast<create_interface_t>( GetProcAddress( module_handle, "CreateInterface" ) );
	if ( !create_interface )
		return nullptr;

	return create_interface( name, nullptr );
};

class c_interfaces
{
public:
	i_entity_system* m_entity_system;
	void* m_input_system;
	i_csgo_input* m_csgo_input;
	i_mem_alloc* m_mem_alloc;
	i_schema_system* m_schema_system;
	i_engine_client* m_engine;
	i_client* m_client;
	i_cvar* m_var;
	i_localize* m_localize;
	i_trace* m_trace;
	i_global_vars* m_global_vars;
	i_network_client_service* m_network_client_service;

	float( __cdecl* m_random_float )( float min, float max ) = nullptr;
	int( __cdecl* m_random_seed )( int seed ) = nullptr;

	void initialize( );
};

inline const auto g_interfaces = std::make_unique<c_interfaces>( );