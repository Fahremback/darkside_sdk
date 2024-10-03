#pragma once

#include <memory>
#include <unordered_map>

#include "../../valve/interfaces/interfaces.hpp"
#include "../../sdk/includes/hash.hpp"

short schema_get_offset( const char* className, const char* keyName );

#define SCHEMA(varName, type, className, keyName)																		\
	type& varName() {																									\
		static const short offset = schema_get_offset(className, keyName);												\
																														\
		return *reinterpret_cast<type*>(reinterpret_cast<unsigned __int64>(this) + offset);								\
	}

#define SCHEMA_ARRAY(varName, type, className, keyName)																	\
	type* varName() {																									\
		static const short offset = schema_get_offset(className, keyName);												\
																														\
		return reinterpret_cast<type*>(reinterpret_cast<unsigned __int64>(this) + offset);								\
	}

#define OFFSET( type, name, offset )                                                              \
    __forceinline std::add_lvalue_reference_t< type > name( ) const {                             \
        return *reinterpret_cast< type* >( reinterpret_cast< std::uintptr_t >( this ) + offset ); \
    }