#pragma once

#include <cassert>

template <class T>
inline T* construct( T* memory )
{
    return ::new ( memory ) T;
}

template <class T, typename ARG1>
inline T* construct( T* memory, ARG1 a1 )
{
    return ::new ( memory ) T( a1 );
}

template <class T, typename ARG1, typename ARG2>
inline T* construct( T* memory, ARG1 a1, ARG2 a2 )
{
    return ::new ( memory ) T( a1, a2 );
}

template <class T, typename ARG1, typename ARG2, typename ARG3>
inline T* construct( T* memory, ARG1 a1, ARG2 a2, ARG3 a3 )
{
    return ::new ( memory ) T( a1, a2, a3 );
}

template <class T, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
inline T* construct( T* memory, ARG1 a1, ARG2 a2, ARG3 a3, ARG4 a4 )
{
    return ::new ( memory ) T( a1, a2, a3, a4 );
}

template <class T, typename ARG1, typename ARG2, typename ARG3, typename ARG4, typename ARG5>
inline T* construct( T* memory, ARG1 a1, ARG2 a2, ARG3 a3, ARG4 a4, ARG5 a5 )
{
    return ::new ( memory ) T( a1, a2, a3, a4, a5 );
}

template <class T>
inline T* copy_construct( T* memory, const T& src )
{
    return ::new ( memory ) T( src );
}

template <class T>
inline void destruct( T* memory ) {
    memory->~T( );

#ifdef _DEBUG
    memset( memory, 0xDD, sizeof( T ) );
#endif
}

template <class T, class I = int>
class c_utl_memory
{
public:
    c_utl_memory( int nGrowSize = 0, int nInitSize = 0 );
    c_utl_memory( T* memory, int numElements );
    c_utl_memory( const T* memory, int numElements );
    ~c_utl_memory( );

    void init( int nGrowSize = 0, int nInitSize = 0 );

    class iterator_t
    {
    public:
        iterator_t( I i ) : index( i ) { }

        I index;

        bool operator==( const iterator_t& it ) const
        {
            return index == it.index;
        }
        bool operator!=( const iterator_t& it ) const
        {
            return index != it.index;
        }
    };

    iterator_t first( ) const
    {
        return iterator_t( is_idx_valid( 0 ) ? 0 : invalid_index( ) );
    }

    iterator_t next( const iterator_t& it ) const
    {
        return iterator_t( is_idx_valid( it.index + 1 ) ? it.index + 1 : invalid_index( ) );
    }

    I get_index( const iterator_t& it ) const
    {
        return it.index;
    }
    bool is_idx_after( I i, const iterator_t& it ) const
    {
        return i > it.index;
    }
    bool is_valid_iterator( const iterator_t& it ) const
    {
        return is_idx_valid( it.index );
    }
    iterator_t invalid_iterator( ) const
    {
        return iterator_t( invalid_index( ) );
    }

    T& operator[]( I i );
    const T& operator[]( I i ) const;
    T& element( I i );
    const T& element( I i ) const;

    bool is_idx_valid( I i ) const;

    static const I INVALID_INDEX = (I)-1;
    static I invalid_index( ) {
        return INVALID_INDEX;
    }

    T* base( );
    const T* base( ) const;

    void set_external_buffer( T* memory, int numElements );
    void set_external_buffer( const T* memory, int numElements );
    void assume_memory( T* memory, int nSize );
    T* detach( );
    void* detach_memory( );

    void swap( c_utl_memory<T, I>& mem );
    void convert_to_growable_memory( int nGrowSize );
    int num_allocated( ) const;
    int count( ) const;
    void grow( int num = 1 );
    void ensure_capacity( int num );
    void purge( );
    void purge( int numElements );
    bool is_externally_allocated( ) const;
    bool is_read_only( ) const;
    void set_grow_size( int size );
protected:
    void validate_grow_size( ) const { }

    enum {
        EXTERNAL_BUFFER_MARKER = -1,
        EXTERNAL_CONST_BUFFER_MARKER = -2,
    };
public:
    T* m_memory;
    int m_allocation_count;
    int m_grow_size;
};

template <class T, class I>
c_utl_memory<T, I>::c_utl_memory( int nGrowSize, int nInitAllocationCount ) : m_memory( 0 ), m_allocation_count( nInitAllocationCount ), m_grow_size( nGrowSize )
{
    validate_grow_size( );
    assert( nGrowSize >= 0 );
    if ( m_allocation_count )
    {
        m_memory = (T*)g_interfaces->m_mem_alloc->alloc( m_allocation_count * sizeof( T ) );
    }
}

template <class T, class I>
c_utl_memory<T, I>::c_utl_memory( T* memory, int numElements ) : m_memory( memory ), m_allocation_count( numElements )
{
    m_grow_size = EXTERNAL_BUFFER_MARKER;
}

template <class T, class I>
c_utl_memory<T, I>::c_utl_memory( const T* memory, int numElements ) : m_memory( (T*)memory ), m_allocation_count( numElements )
{
    m_grow_size = EXTERNAL_CONST_BUFFER_MARKER;
}

template <class T, class I>
c_utl_memory<T, I>::~c_utl_memory( )
{
    purge( );
}

template <class T, class I>
void c_utl_memory<T, I>::init( int nGrowSize /*= 0*/, int nInitSize /*= 0*/ )
{
    purge( );

    m_grow_size = nGrowSize;
    m_allocation_count = nInitSize;
    validate_grow_size( );
    assert( nGrowSize >= 0 );
    if ( m_allocation_count ) {
        m_memory = (T*)g_interfaces->m_mem_alloc->alloc( m_allocation_count * sizeof( T ) );
    }
}

template <class T>
__forceinline void V_swap( T& x, T& y )
{
    T temp = x;
    x = y;
    y = temp;
}

template <class T, class I>
void c_utl_memory<T, I>::swap( c_utl_memory<T, I>& mem )
{
    V_swap( m_grow_size, mem.m_grow_size );
    V_swap( m_memory, mem.m_memory );
    V_swap( m_allocation_count, mem.m_allocation_count );
}

template <class T, class I>
void c_utl_memory<T, I>::convert_to_growable_memory( int nGrowSize )
{
    if ( !is_externally_allocated( ) )
        return;

    m_grow_size = nGrowSize;
    if ( m_allocation_count )
    {
        int nNumBytes = m_allocation_count * sizeof( T );
        T* memory = (T*)g_interfaces->m_mem_alloc->alloc( nNumBytes );
        memcpy( memory, m_memory, nNumBytes );
        m_memory = memory;
    }
    else
    {
        m_memory = NULL;
    }
}

template <class T, class I>
void c_utl_memory<T, I>::set_external_buffer( T* memory, int numElements )
{
    purge( );

    m_memory = memory;
    m_allocation_count = numElements;
    m_grow_size = EXTERNAL_BUFFER_MARKER;
}

template <class T, class I>
void c_utl_memory<T, I>::set_external_buffer( const T* memory, int numElements )
{
    purge( );

    m_memory = const_cast<T*>( memory );
    m_allocation_count = numElements;

    m_grow_size = EXTERNAL_CONST_BUFFER_MARKER;
}

template <class T, class I>
void c_utl_memory<T, I>::assume_memory( T* memory, int numElements )
{
    purge( );
    m_memory = memory;
    m_allocation_count = numElements;
}

template <class T, class I>
void* c_utl_memory<T, I>::detach_memory( )
{
    if ( is_externally_allocated( ) )
        return NULL;

    void* memory = m_memory;
    m_memory = 0;
    m_allocation_count = 0;
    return memory;
}

template <class T, class I>
inline T* c_utl_memory<T, I>::detach( )
{
    return (T*)detach_memory( );
}

template <class T, class I>
inline T& c_utl_memory<T, I>::operator[]( I i )
{
    assert( !is_read_only( ) );
    assert( is_idx_valid( i ) );
    return m_memory[ i ];
}

template <class T, class I>
inline const T& c_utl_memory<T, I>::operator[]( I i ) const
{
    assert( is_idx_valid( i ) );
    return m_memory[ i ];
}

template <class T, class I>
inline T& c_utl_memory<T, I>::element( I i )
{
    assert( !is_read_only( ) );
    assert( is_idx_valid( i ) );
    return m_memory[ i ];
}

template <class T, class I>
inline const T& c_utl_memory<T, I>::element( I i ) const
{
    assert( is_idx_valid( i ) );
    return m_memory[ i ];
}

template <class T, class I>
bool c_utl_memory<T, I>::is_externally_allocated( ) const
{
    return ( m_grow_size < 0 );
}

template <class T, class I>
bool c_utl_memory<T, I>::is_read_only( ) const
{
    return ( m_grow_size == EXTERNAL_CONST_BUFFER_MARKER );
}

template <class T, class I>
void c_utl_memory<T, I>::set_grow_size( int nSize )
{
    assert( !is_externally_allocated( ) );
    assert( nSize >= 0 );
    m_grow_size = nSize;
    validate_grow_size( );
}

template <class T, class I>
inline T* c_utl_memory<T, I>::base( )
{
    assert( !is_read_only( ) );
    return m_memory;
}

template <class T, class I>
inline const T* c_utl_memory<T, I>::base( ) const
{
    return m_memory;
}

template <class T, class I>
inline int c_utl_memory<T, I>::num_allocated( ) const
{
    return m_allocation_count;
}

template <class T, class I>
inline int c_utl_memory<T, I>::count( ) const
{
    return m_allocation_count;
}

template <class T, class I>
inline bool c_utl_memory<T, I>::is_idx_valid( I i ) const
{
    long x = i;
    return ( x >= 0 ) && ( x < m_allocation_count );
}

inline int UtlMemory_CalcNewAllocationCount( int nAllocationCount, int nGrowSize, int nNewSize, int nBytesItem )
{
    if ( nGrowSize )
    {
        nAllocationCount = ( ( 1 + ( ( nNewSize - 1 ) / nGrowSize ) ) * nGrowSize );
    }
    else
    {
        if ( !nAllocationCount )
        {
            nAllocationCount = ( 31 + nBytesItem ) / nBytesItem;
        }

        while ( nAllocationCount < nNewSize ) {
#ifndef _X360
            nAllocationCount *= 2;
#else
            int nNewAllocationCount = ( nAllocationCount * 9 ) / 8; // 12.5 %
            if ( nNewAllocationCount > nAllocationCount )
                nAllocationCount = nNewAllocationCount;
            else
                nAllocationCount *= 2;
#endif
        }
    }

    return nAllocationCount;
}

template <class T, class I>
void c_utl_memory<T, I>::grow( int num ) {
    assert( num > 0 );

    if ( is_externally_allocated( ) )
    {
        assert( 0 );
        return;
    }

    auto oldAllocationCount = m_allocation_count;
    int nAllocationRequested = m_allocation_count + num;

    int nNewAllocationCount = UtlMemory_CalcNewAllocationCount( m_allocation_count, m_grow_size, nAllocationRequested, sizeof( T ) );

    if ( (int)(I)nNewAllocationCount < nAllocationRequested )
    {
        if ( (int)(I)nNewAllocationCount == 0 && (int)(I)( nNewAllocationCount - 1 ) >= nAllocationRequested )
        {
            --nNewAllocationCount;
        }
        else
        {
            if ( (int)(I)nAllocationRequested != nAllocationRequested )
            {
                assert( 0 );
                return;
            }
            while ( (int)(I)nNewAllocationCount < nAllocationRequested )
            {
                nNewAllocationCount = ( nNewAllocationCount + nAllocationRequested ) / 2;
            }
        }
    }

    m_allocation_count = nNewAllocationCount;

    if ( m_memory ) {
        auto ptr = g_interfaces->m_mem_alloc->alloc( m_allocation_count * sizeof( T ) );

        memcpy( ptr, m_memory, oldAllocationCount * sizeof( T ) );
        m_memory = (T*)ptr;
    }
    else {
        m_memory = (T*)g_interfaces->m_mem_alloc->alloc( m_allocation_count * sizeof( T ) );
    }
}

template <class T, class I>
inline void c_utl_memory<T, I>::ensure_capacity( int num )
{
    if ( m_allocation_count >= num )
        return;

    if ( is_externally_allocated( ) )
    {
        assert( 0 );
        return;
    }
    m_allocation_count = num;

    if ( m_memory )
    {
        m_memory = (T*)g_interfaces->m_mem_alloc->re_alloc( m_memory, m_allocation_count * sizeof( T ) );
    }
    else {
        m_memory = (T*)g_interfaces->m_mem_alloc->alloc( m_allocation_count * sizeof( T ) );
    }
}

template <class T, class I>
void c_utl_memory<T, I>::purge( )
{
    if ( !is_externally_allocated( ) )
    {
        if ( m_memory )
        {
            g_interfaces->m_mem_alloc->free( (void*)m_memory );
            m_memory = 0;
        }
        m_allocation_count = 0;
    }
}

template <class T, class I>
void c_utl_memory<T, I>::purge( int numElements )
{
    assert( numElements >= 0 );

    if ( numElements > m_allocation_count )
    {
        assert( numElements <= m_allocation_count );
        return;
    }

    if ( numElements == 0 )
    {
        purge( );
        return;
    }

    if ( is_externally_allocated( ) )
    {
        return;
    }

    if ( numElements == m_allocation_count )
    {
        return;
    }

    if ( !m_memory )
    {
        assert( m_memory );
        return;
    }
    m_allocation_count = numElements;
    m_memory = (T*)g_interfaces->m_mem_alloc->re_alloc( m_memory, m_allocation_count * sizeof( T ) );
}