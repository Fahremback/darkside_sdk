#include "../darkside.hpp"

std::vector<int> c_opcodes::ida_to_bytes( const char* pattern )
{
    std::vector<int> bytes = std::vector<int>{};
    char* start = const_cast<char*>( pattern );
    char* end = const_cast<char*>( pattern ) + strlen( pattern );

    for ( char* current = start; current < end; ++current ) {
        // multiple format, we can use both '?' and '??'
        if ( *current == '?' ) {
            ++current;

            if ( *current == '?' )
                ++current;

            bytes.push_back( -1 );
        }
        else {
            bytes.push_back( strtoul( current, &current, 16 ) );
        }
    }

    return bytes;
}

uint8_t* c_opcodes::scan( const char* module_name, const char* pattern ) {
    void* module_handle = WINCALL( GetModuleHandle )( module_name );
    if ( module_handle == nullptr )
        return nullptr;

    PIMAGE_DOS_HEADER dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>( module_handle );
    PIMAGE_NT_HEADERS nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>( reinterpret_cast<uint8_t*>( module_handle ) + dos_header->e_lfanew );

    auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
    auto pattern_bytes = ida_to_bytes( pattern );
    auto scan_bytes = reinterpret_cast<uint8_t*>( module_handle );

    auto pattern_size = pattern_bytes.size( );
    auto pattern_data = pattern_bytes.data( );

    for ( unsigned int i = 0; i < size_of_image - pattern_size; i++ ) {
        bool found = true;

        for ( unsigned int j = 0; j < pattern_size; ++j ) {
            // skip if current character is '?'
            if ( pattern_data[ j ] == -1 )
                continue;

            if ( scan_bytes[ i + j ] != pattern_data[ j ] ) {
                found = false;

                break;
            }
        }

        if ( found )
            return &scan_bytes[ i ];
    }

    return nullptr;
}

uint8_t* c_opcodes::scan( const char* module_name, const char* pattern, int offset )
{
    return scan( module_name, pattern ) + offset;
}

uint8_t* c_opcodes::scan_relative( const char* module_name, const char* pattern, int offset, int rva_offset ) {
    uint8_t* address = scan( module_name, pattern, offset );
    if ( address == nullptr )
        return nullptr;

    return resolve_relative_address( address, rva_offset, 0 );
}

uint8_t* c_opcodes::scan_relative( const char* module_name, const char* pattern, int offset, int rva_offset, int rip_offset ) {
    uint8_t* address = scan( module_name, pattern, offset );
    if ( address == nullptr )
        return nullptr;

    return resolve_relative_address( address, rva_offset, rip_offset );
}

uint8_t* c_opcodes::scan_absolute( const char* module_name, const char* pattern ) {
    uint8_t* address = scan( module_name, pattern );
    if ( address == nullptr )
        return nullptr;

    return get_absolute_address( address );
}

uint8_t* c_opcodes::scan_absolute( const char* module_name, const char* pattern, int pre_offset ) {
    uint8_t* address = scan( module_name, pattern );
    if ( address == nullptr )
        return nullptr;

    return get_absolute_address( address, pre_offset );
}

uint8_t* c_opcodes::scan_absolute( const char* module_name, const char* pattern, int pre_offset, int post_offset ) {
    uint8_t* address = scan( module_name, pattern );
    if ( address == nullptr )
        return nullptr;

    return get_absolute_address( address, pre_offset, post_offset );
}

unsigned __int64 c_opcodes::export_fn( unsigned __int64 base, const char* procedure_name )
{
    const auto dos_headers = reinterpret_cast<const IMAGE_DOS_HEADER*>( base );
    if ( dos_headers->e_magic != IMAGE_DOS_SIGNATURE )
        return 0;

    const auto nt_headers = reinterpret_cast<const IMAGE_NT_HEADERS64*>( base + dos_headers->e_lfanew );
    if ( nt_headers->Signature != IMAGE_NT_SIGNATURE )
        return 0;

    const std::uintptr_t export_directory_size = nt_headers->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].Size;
    const std::uintptr_t export_directory_address = nt_headers->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress;

    if ( export_directory_size == 0u || export_directory_address == 0u )
        return 0;

    const auto exports = reinterpret_cast<const IMAGE_EXPORT_DIRECTORY*>( base + export_directory_address );
    const auto names_rva = reinterpret_cast<const std::uint32_t*>( base + exports->AddressOfNames );
    const auto name_ordinals_rva = reinterpret_cast<const std::uint16_t*>( base + exports->AddressOfNameOrdinals );
    const auto functions_rva = reinterpret_cast<const std::uint32_t*>( base + exports->AddressOfFunctions );

    std::size_t right = exports->NumberOfNames, left = 0u;
    while ( right != left )
    {
        const std::size_t middle = left + ( ( right - left ) >> 1u );
        const int result = strcmp( procedure_name, reinterpret_cast<const char*>( base + names_rva[ middle ] ) );

        if ( result == 0 )
        {
            const std::uint32_t function_rva = functions_rva[ name_ordinals_rva[ middle ] ];

            if ( function_rva >= export_directory_address && function_rva - export_directory_address < export_directory_size )
                break;

            return base + function_rva;
        }

        if ( result > 0 )
            left = middle + 1;
        else
            right = middle;
    }
    return 0;
}

uint8_t* c_opcodes::get_absolute_address( unsigned char* address, int pre_offset, int post_offset ) {
    address += pre_offset;
    address += *reinterpret_cast<std::int32_t*>( address ) + 0x4;
    address += post_offset;

    return address;
}

uint8_t* c_opcodes::get_absolute_address( unsigned char* address, int pre_offset ) {
    return get_absolute_address( address, pre_offset, 0 );
}

uint8_t* c_opcodes::get_absolute_address( unsigned char* address ) {
    return get_absolute_address( address, 0, 0 );
}

uint8_t* c_opcodes::resolve_relative_address( unsigned char* address, int rva_offset, int rip_offset ) {
    uint32_t RVA = *reinterpret_cast<uint32_t*>( address + rva_offset );
    uint64_t RIP = reinterpret_cast<uint64_t>( address ) + rip_offset;

    return reinterpret_cast<uint8_t*>( RVA + RIP );
}

uint8_t* c_opcodes::resolve_relative_address( unsigned char* address, int rva_offset ) {
    return resolve_relative_address( address, rva_offset, 0x0 );
}