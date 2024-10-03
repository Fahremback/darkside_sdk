#pragma once

#include "../../../sdk/vfunc/vfunc.hpp"

enum e_cvar_flags : int
{
	FCVAR_NONE = 0,
	FCVAR_UNREGISTERED = ( 1 << 0 ),
	FCVAR_DEVELOPMENTONLY = ( 1 << 1 ),
	FCVAR_GAMEDLL = ( 1 << 2 ),
	FCVAR_CLIENTDLL = ( 1 << 3 ),
	FCVAR_HIDDEN = ( 1 << 4 ),
	FCVAR_PROTECTED = ( 1 << 5 ),
	FCVAR_SPONLY = ( 1 << 6 ),
	FCVAR_ARCHIVE = ( 1 << 7 ),
	FCVAR_NOTIFY = ( 1 << 8 ),
	FCVAR_USERINFO = ( 1 << 9 ),
	FCVAR_CHEAT = ( 1 << 14 ),
	FCVAR_PRINTABLEONLY = ( 1 << 10 ),
	FCVAR_UNLOGGED = ( 1 << 11 ),
	FCVAR_NEVER_AS_STRING = ( 1 << 12 ),
	FCVAR_REPLICATED = ( 1 << 13 ),
	FCVAR_DEMO = ( 1 << 16 ),
	FCVAR_DONTRECORD = ( 1 << 17 ),
	FCVAR_RELOAD_MATERIALS = ( 1 << 20 ),
	FCVAR_RELOAD_TEXTURES = ( 1 << 21 ),
	FCVAR_NOT_CONNECTED = ( 1 << 22 ),
	FCVAR_MATERIAL_SYSTEM_THREAD = ( 1 << 23 ),
	FCVAR_ARCHIVE_XBOX = ( 1 << 24 ),
	FCVAR_ACCESSIBLE_FROM_THREADS = ( 1 << 25 ),
	FCVAR_SERVER_CAN_EXECUTE = ( 1 << 28 ),
	FCVAR_SERVER_CANNOT_QUERY = ( 1 << 29 ),
	FCVAR_CLIENTCMD_CAN_EXECUTE = ( 1 << 30 ),
	FCVAR_MATERIAL_THREAD_MASK = ( FCVAR_RELOAD_MATERIALS | FCVAR_RELOAD_TEXTURES | FCVAR_MATERIAL_SYSTEM_THREAD )
};

union convar_value_t
{
	bool m_i1;
	short m_i16;
	unsigned short m_u16;
	int m_i32;
	unsigned int m_u32;
	long long m_i64;
	unsigned long long m_u64;
	float m_fl;
	double m_db;
	const char* m_sz;
};

class convar_t {
public:
	const char* m_name;
	convar_t* m_next;
	char pad_0008[ 0x10 ];
	const char* m_description;
	unsigned int m_type;
	unsigned int m_registered;
	unsigned int m_flags;
	char padding_2[ 0x8 ];
	convar_value_t m_value;

	bool get_bool( ) const {
		return m_value.m_i1;
	}
	int get_int( ) const {
		return m_value.m_i32;
	}
	float get_float( ) const {
		return m_value.m_fl;
	}
};

using var_iterator_t = unsigned long long;

class i_cvar {
public:
	var_iterator_t get_first_var_iterator( );
	var_iterator_t get_next_var( var_iterator_t previous );

	convar_t* get_by_index( var_iterator_t idx );
	convar_t* get_by_name( const char* name );

	void unlock_hidden_vars( );
};