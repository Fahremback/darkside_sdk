#pragma once

#include <memory>
#include <Windows.h>
#include <vector>

class c_opcodes {
public:
    std::vector<int> ida_to_bytes( const char* );
    unsigned char* scan( const char*, const char* );
    unsigned char* scan( const char*, const char*, int );

    unsigned char* scan_relative( const char*, const char*, int, int );
    unsigned char* scan_relative( const char*, const char*, int, int, int );

    unsigned char* scan_absolute( const char*, const char* );
    unsigned char* scan_absolute( const char*, const char*, int );
    unsigned char* scan_absolute( const char*, const char*, int, int );

    unsigned __int64 export_fn( unsigned __int64 base, const char* procedure_name );

    unsigned char* get_absolute_address( unsigned char* );
    unsigned char* get_absolute_address( unsigned char*, int );
    unsigned char* get_absolute_address( unsigned char*, int, int );

    unsigned char* resolve_relative_address( unsigned char*, int );
    unsigned char* resolve_relative_address( unsigned char*, int, int );
};

const auto g_opcodes = std::make_unique<c_opcodes>( );