#pragma once

#include "../../../sdk/vfunc/vfunc.hpp"

class i_localize {
public:
	const char* find_key( const char* key ) {
		return vmt::call_virtual<const char*>( this, 17, key );
	}
};