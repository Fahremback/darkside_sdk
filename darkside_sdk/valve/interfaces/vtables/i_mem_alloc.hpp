#pragma once

#include "../../../sdk/vfunc/vfunc.hpp"
#include <cstddef>

class i_mem_alloc {
public:
	auto alloc( std::size_t size ) {
		return VFUNC( this, void*( __thiscall* )( void*, std::size_t ), 1, size );
	}
	
	auto re_alloc( const void* memory, std::size_t size ) {
		return VFUNC( this, void*( __thiscall* )( void*, const void*, std::size_t ), 2, memory, size );
	}

	auto free( const void* memory ) {
		return VFUNC( this, void( __thiscall* )( void*, const void* ), 3, memory );
	}

	auto get_size( const void* memory ) {
		return VFUNC( this, std::size_t( __thiscall* )( void*, const void* ), 21, memory );
	}
};