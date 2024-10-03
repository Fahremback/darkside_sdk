#pragma once

#include <Windows.h>
#include <string>
#include <memory>
#include <vector>

class c_address;

class c_dll {
	HMODULE m_dll;
	size_t m_size;
	char* m_name;
public:
	c_dll( ) : m_dll( nullptr ), m_size( 0 ), m_name( nullptr ) { }
	c_dll( const char* name ) : m_dll( GetModuleHandleA( name ) ), m_size( 0 ), m_name( nullptr ) {
		if ( !m_dll )
			return;

		auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>( m_dll );
		auto nt_header = reinterpret_cast<PIMAGE_NT_HEADERS>( reinterpret_cast<uintptr_t>( m_dll ) + dos_header->e_lfanew );
		m_size = nt_header->OptionalHeader.SizeOfImage;

		if ( !name )
			return;

		m_name = new char[ 256 ];
		strcpy_s( m_name, 256, name );
	}
	c_dll( HMODULE dll ) : m_dll( dll ) {
		auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>( m_dll );
		auto nt_header = reinterpret_cast<PIMAGE_NT_HEADERS>( reinterpret_cast<uintptr_t>( m_dll ) + dos_header->e_lfanew );
		m_size = nt_header->OptionalHeader.SizeOfImage;

		GetModuleFileNameA( m_dll, m_name, sizeof( m_name ) );

		m_name = strrchr( m_name, '\\' ) + 1;
	}

	template<typename type_t>
	type_t get_export( const char* name ) {
		auto address = GetProcAddress( m_dll, name );
		return *reinterpret_cast<type_t*>( &address );
	}

	// get base as ptr
	uintptr_t get( ) {
		return reinterpret_cast<uintptr_t>( m_dll );
	}

	const char* get_name( ) {
		return m_name;
	}
};

class c_address {
	uintptr_t m_address;
public:
	c_address( ) : m_address( 0 ) { }
	c_address( const uintptr_t address ) : m_address( address ) { }
	c_address( const void* address ) : m_address( reinterpret_cast<uintptr_t>( address ) ) { }

	operator bool( ) const
	{
		return m_address != 0;
	}

	template <typename type_t = uintptr_t>
	type_t as( ) const
	{
		// c-style casts work everywhere so we use them
		return (type_t)( m_address );
	}

	template <typename type_t = uintptr_t>
	type_t as_fn( ) const
	{
		// c-style casts work everywhere so we use them
		return reinterpret_cast<decltype( type_t )>( m_address );
	}

	template <typename type_t = c_address>
	type_t dereference( ) const
	{
		return *reinterpret_cast<type_t*>( m_address );
	}
	template <typename type_t = c_address>
	type_t relative( ) const
	{
		return m_address + *reinterpret_cast<int32_t*>( m_address ) + 0x4;
	}
	template <typename type_t = c_address>
	type_t add( size_t offset ) const
	{
		return m_address + offset;
	}
	template <typename type_t = c_address>
	type_t sub( size_t offset ) const
	{
		return m_address - offset;
	}

	template < typename type_t = c_address >
	type_t abs( std::ptrdiff_t offset = 0x3, std::ptrdiff_t length = 7 ) const
	{
		std::uintptr_t base = m_address;

		const auto displacement = *reinterpret_cast<std::int32_t*>( base + offset );

		base += displacement;
		base += length;

		return (type_t)( base );
	}
};

class c_interface {
	c_address m_interface;
public:
	c_interface( ) : m_interface( nullptr ) { }
	c_interface( c_address address ) : m_interface( address ) { }
	c_interface( void* address ) : m_interface( address ) { }

	operator bool( ) const
	{
		return m_interface;
	}

	template <typename type_t = uintptr_t>
	type_t as( ) const
	{
		return m_interface.as<type_t>( );
	}

	template <typename ret_t = uintptr_t>
	ret_t get_virtual_function( size_t index ) // returns the function pointer in vtable
	{
		return m_interface.dereference( ).dereference( ).add( index * sizeof( uintptr_t ) ).as<ret_t>( );
	}

	template <typename ret_t = uintptr_t>
	ret_t get_virtual_function_address( size_t index ) // returns the function address instead of vtable pointer
	{
		return get_virtual_function<c_address>( index ).dereference( ).as<ret_t>( );
	}

	template <typename ret_t, size_t index, class... args_t>
	ret_t call_virtual_function( args_t... args )
	{
		using fn_t = ret_t( __thiscall* )( void*, args_t... );
		fn_t fn = get_virtual_function<fn_t>( index );
		return fn( m_interface.as<void*>( ), args... );
	}
};

#define OFFSET(name, type, offset) \
	type name( ) { return c_address(this).add( offset ).as<type>( ); } \
	type name( ) const { return c_address(this).add( offset ).as<type>( ); }

class c_modules {
private:
	struct modules_t {
		c_dll client_dll{};
		c_dll engine2_dll{};
		c_dll schemasystem_dll{};
		c_dll tier0_dll{};
		c_dll navsystem_dll{};
		c_dll rendersystem_dll{};
		c_dll localize_dll{};
		c_dll scenesystem_dll{};
		c_dll materialsystem2_dll{};
		c_dll resourcesystem_dll{};
		c_dll input_system{};
		c_dll animation_system{};

		void initialize( );
	};

public:
	modules_t m_modules{ };
};

inline const auto g_modules = std::make_unique<c_modules>( );