#include "schema.hpp"

using schema_key_value_map_t = std::unordered_map<unsigned long long, short>;
using schema_table_map_t = std::unordered_map<unsigned long long, schema_key_value_map_t>;

const char* dll_files[ 2 ] = { "client.dll", "animationsystem.dll" };

bool init_schema_fields_for_class( schema_table_map_t& table_map, const char* class_name, uint64_t class_key ) {
    for ( const char* dll_file : dll_files ) {
        c_schema_type_scope* type_scope = g_interfaces->m_schema_system->find_type_scope_for_module( dll_file );
        if ( !type_scope )
            continue;

        c_schema_class_info* class_info = type_scope->find_declared_class( class_name );
        if ( !class_info )
            continue;

        short fields_size = class_info->get_fields_size( );
        c_schema_class_field* fields = class_info->get_fields( );

        auto& key_value_map = table_map[ class_key ];

        for ( int i = 0; i < fields_size; ++i ) {
            c_schema_class_field& field = fields[ i ];

            key_value_map.emplace( fnv1a::hash_64( field.m_name ), field.m_offset );
        }

        return true;
    }

    table_map.emplace( class_key, schema_key_value_map_t{} );

    return false;
}

int16_t schema_get_offset( const char* class_name, const char* key_name ) {
    uint64_t class_name_key = fnv1a::hash_64( class_name );
    uint64_t key_name_key = fnv1a::hash_64( key_name );

    static schema_table_map_t schema_table_map;

    const auto& it = schema_table_map.find( class_name_key );
    if ( it == schema_table_map.cend( ) ) {
        if ( init_schema_fields_for_class( schema_table_map, class_name, class_name_key ) )
            return schema_get_offset( class_name, key_name );

        printf_s( "[Schema] couldn't find %s->%s \n", class_name, key_name );

        return 0;
    }

    const schema_key_value_map_t& table_map = it->second;
    if ( table_map.find( key_name_key ) == table_map.cend( ) ) {
        printf_s( "[Schema] couldn't find %s->%s \n", class_name, key_name );

        return 0;
    }

    int16_t offset = table_map.at( key_name_key );

    printf_s( "[Schema] %s->%s: 0x%X \n", class_name, key_name, offset );

    return offset;
}