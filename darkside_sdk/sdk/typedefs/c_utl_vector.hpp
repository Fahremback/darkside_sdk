#pragma once

template <class T>
class c_utl_vector
{
public:
    c_utl_vector( )
    {
        m_size = 0;
        m_elements = nullptr;
    }

    T& element( int i );
    const T& element( int i ) const;

    T& operator[]( int i )
    {
        return m_elements[ i ];
    }

    int count( ) const
    {
        return m_size;
    }

    int m_size;
    T* m_elements;

    int add_to_head( );
    int add_to_tail( );
    int insert_before( int elem );
    int insert_after( int elem );
    int add_to_head( const T& src );
    int add_to_tail( const T& src );
    int insert_before( int elem, const T& src );
    int insert_after( int elem, const T& src );
    void grow_vector( int num = 1 );
    void shift_elements_right( int elem, int num = 1 );
    void shift_elements_left( int elem, int num = 1 );
    void fast_remove( int elem );
    void remove( int elem );
    bool find_and_remove( const T& src );
    bool find_and_fast_remove( const T& src );
    int get_offset( const T& src ) const;
};

template <typename T>
inline T& c_utl_vector<T>::element( int i )
{
    assert( i < m_size );
    return m_elements[ i ];
}

template <typename T>
inline const T& c_utl_vector<T>::element( int i ) const
{
    assert( i < m_size );
    return m_elements[ i ];
}

template <typename T>
void c_utl_vector<T>::grow_vector( int num )
{
    m_size += num;
    if ( m_elements )
        m_elements = (T*)g_interfaces->m_mem_alloc->re_alloc( m_elements, m_size * sizeof( T ) );
    else
        m_elements = (T*)g_interfaces->m_mem_alloc->alloc( m_size * sizeof( T ) );
}

template <typename T>
inline int c_utl_vector<T>::add_to_head( )
{
    return insert_before( 0 );
}

template <typename T>
inline int c_utl_vector<T>::add_to_tail( )
{
    return insert_before( m_size );
}

template <typename T>
inline int c_utl_vector<T>::insert_after( int elem )
{
    return insert_before( elem + 1 );
}

template <typename T>
int c_utl_vector<T>::insert_before( int elem )
{
    grow_vector( );
    shift_elements_right( elem );
    Construct( &element( elem ) );
    return elem;
}

template <typename T>
inline int c_utl_vector<T>::add_to_head( const T& src )
{
    return insert_before( 0, src );
}

template <typename T>
inline int c_utl_vector<T>::add_to_tail( const T& src )
{
    return insert_before( m_size, src );
}

template <typename T>
inline int c_utl_vector<T>::insert_after( int elem, const T& src )
{
    return insert_before( elem + 1, src );
}

template <typename T>
int c_utl_vector<T>::insert_before( int elem, const T& src )
{
    grow_vector( );
    shift_elements_right( elem );
    CopyConstruct( &element( elem ), src );
    return elem;
}

template <typename T>
void c_utl_vector<T>::shift_elements_right( int elem, int num )
{
    int numToMove = m_size - elem - num;
    if ( ( numToMove > 0 ) && ( num > 0 ) )
        memmove( &element( elem + num ), &element( elem ), numToMove * sizeof( T ) );
}

template <typename T>
void c_utl_vector<T>::shift_elements_left( int elem, int num )
{
    int numToMove = m_size - elem - num;
    if ( ( numToMove > 0 ) && ( num > 0 ) )
    {
        memmove( &element( elem ), &element( elem + num ), numToMove * sizeof( T ) );

#ifdef _DEBUG
        memset( &element( m_size - num ), 0xDD, num * sizeof( T ) );
#endif
    }
}

template <typename T>
void c_utl_vector<T>::fast_remove( int elem )
{
    Destruct( &element( elem ) );
    if ( m_size > 0 )
    {
        if ( elem != m_size - 1 )
            memcpy( &element( elem ), &element( m_size - 1 ), sizeof( T ) );
        --m_size;
    }
}

template <typename T>
void c_utl_vector<T>::remove( int elem )
{
    Destruct( &element( elem ) );
    shift_elements_left( elem );
    --m_size;
}

template <typename T>
bool c_utl_vector<T>::find_and_remove( const T& src )
{
    int elem = get_offset( src );
    if ( elem != -1 )
    {
        remove( elem );
        return true;
    }
    return false;
}

template <typename T>
bool c_utl_vector<T>::find_and_fast_remove( const T& src )
{
    int elem = get_offset( src );
    if ( elem != -1 ) {
        fast_remove( elem );
        return true;
    }
    return false;
}

template <typename T>
int c_utl_vector<T>::get_offset( const T& src ) const
{
    for ( auto i = 0; i < count( ); ++i )
    {
        if ( element( i ) == src )
            return i;
    }
    return -1;
}
