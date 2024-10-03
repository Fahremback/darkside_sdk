#pragma once

#define VFUNC(_class, ret_type, index, ...) vmt::call_virtual<ret_type>(_class, index, __VA_ARGS__)

namespace vmt {
    template <typename T = void*>
    inline T get_v_method( void* class_, unsigned int index ) {
        if ( !class_ )
            return T{};

        void** table = *static_cast<void***>( class_ );
        if ( !table )
            return T{};

        return reinterpret_cast<T>( table[ index ] );
    }

    template <typename T, typename... Args>
    inline T call_virtual( void* class_, unsigned int index, Args... args ) {
        auto func = get_v_method<T( __thiscall* )( void*, Args... )>( class_, index );

        return func( class_, args... );
    }
}