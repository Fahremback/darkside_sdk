#pragma once

#include <Windows.h>
#include <memory>

class c_key_handler {
public:
	bool is_pressed( int key, int key_style ) {
        switch ( key_style ) {
        case 0:
            return HIWORD( GetKeyState( key ) );
        case 1:
            return LOWORD( GetKeyState( key ) );
        case 2:
            return true;
        default: return true;
        }
	}
};

const auto g_key_handler = std::make_unique<c_key_handler>( );