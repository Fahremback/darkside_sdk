#pragma once
#include <cstdint>

#define INVALID_EHANDLE_INDEX 0xFFFFFFFF
#define ENT_ENTRY_MASK 0x7FFF
#define NUM_SERIAL_NUM_SHIFT_BITS 15
#define ENT_MAX_NETWORKED_ENTRY 16384

class c_base_handle
{
public:
	c_base_handle( ) noexcept :
		m_index( INVALID_EHANDLE_INDEX ) { }

	c_base_handle( const int nEntry, const int nSerial ) noexcept {
		m_index = nEntry | ( nSerial << NUM_SERIAL_NUM_SHIFT_BITS );
	}

	bool operator!=( const c_base_handle& other ) const noexcept {
		return m_index != other.m_index;
	}

	bool operator==( const c_base_handle& other ) const noexcept {
		return m_index == other.m_index;
	}

	bool operator<( const c_base_handle& other ) const noexcept {
		return m_index < other.m_index;
	}

	bool is_valid( ) const noexcept {
		return m_index != INVALID_EHANDLE_INDEX;
	}

	int get_entry_index( ) const noexcept {
		return static_cast<int>( m_index & ENT_ENTRY_MASK );
	}

	int get_serial_number( ) const noexcept {
		return static_cast<int>( m_index >> NUM_SERIAL_NUM_SHIFT_BITS );
	}

	inline int to_int( ) const {
		return static_cast<int>( m_index );
	}

private:
	std::uint32_t m_index;
};

template <typename T>
class c_network_utl_vector {
public:
	unsigned int m_size;
	T* m_elements;
};