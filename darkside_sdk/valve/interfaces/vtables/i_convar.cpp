#include "i_convar.hpp"
#include <cstdint>
#include "../../../sdk/includes/hash.hpp"

// to do for @shialex: fix crash (@evj_k)
// in the end this asshole did nothing and it had to be fixed by me (@evj_k)
var_iterator_t i_cvar::get_first_var_iterator( ) {
    var_iterator_t it = -1;

    VFUNC( this, void, 12, &it );
    return it;
}

var_iterator_t i_cvar::get_next_var( var_iterator_t previous ) {
    var_iterator_t it = previous;

    VFUNC( this, void, 13, &it, previous );

    return it;
}

convar_t* i_cvar::get_by_index( var_iterator_t idx ) {
    return VFUNC( this, convar_t*, 37, idx );
}

convar_t* i_cvar::get_by_name( const char* name ) {
    auto hash = fnv1a::hash_64( name );

    var_iterator_t it = get_first_var_iterator( );

    while ( true ) {
        convar_t* var = get_by_index( it );

        if ( !var )
            break;

        if ( fnv1a::hash_64( var->m_name ) == hash )
            return var;

        it = get_next_var( it );
    }

    return nullptr;
}

void i_cvar::unlock_hidden_vars( ) {
    var_iterator_t it = get_first_var_iterator( );

    while ( true ) {
        convar_t* var = get_by_index( it );

        if ( !var )
            break;

        var->m_flags &= ~( FCVAR_HIDDEN | FCVAR_DEVELOPMENTONLY );

        it = get_next_var( it );
    }
}