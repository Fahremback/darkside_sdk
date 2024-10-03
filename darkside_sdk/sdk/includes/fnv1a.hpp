#pragma once
#include <cstddef>
#include <string>

using hash32_t = std::uint32_t;

#define HASH( str )                                                                                                                                                                                                                  \
  []( )                                                                                                                                                                                                                              \
  {                                                                                                                                                                                                                                  \
    hash32_t out{ c_fnv1a::get( str ) };                                                                                                                                                                                   \
                                                                                                                                                                                                                                     \
    return out;                                                                                                                                                                                                                      \
  }( )

#define CONST_HASH( str ) c_fnv1a::get( str )

class c_fnv1a
{
private:
    enum : std::uint32_t
    {
        PRIME = 0x1000193u,
        BASIS = 0x811C9DC5u
    };

    static __forceinline constexpr std::size_t ct_strlen( const char* str, bool include_nullchar = false )
    {
        std::size_t out{ };

        while ( str[ ++out ] != '\0' )
            ;

        if ( include_nullchar )
            ++out;

        return out;
    }

public:
    static __forceinline constexpr hash32_t get( const std::uint8_t* data, const std::size_t len )
    {
        hash32_t out{ BASIS };

        for ( std::size_t i = 0; i < len; ++i )
            out = ( out ^ data[ i ] ) * PRIME;

        return out;
    }

    static __forceinline constexpr hash32_t get( const char* str )
    {
        hash32_t out{ BASIS };
        size_t len{ ct_strlen( str ) };

        for ( std::size_t i = 0; i < len; ++i )
            out = ( out ^ str[ i ] ) * PRIME;

        return out;
    }

    static __forceinline hash32_t get( const std::string& str )
    {
        return get( (std::uint8_t*)str.c_str( ), str.size( ) );
    }
};