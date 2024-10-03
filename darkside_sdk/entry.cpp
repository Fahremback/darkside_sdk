#include "darkside.hpp"
#include "directx/directx.hpp"
#include "entity_system/entity.hpp"

void destroy( HMODULE h_module ) {
    g_hooks->destroy( );
    logger::shutdown( );
    g_entity_system->level_shutdown( );
    FreeConsole( );

    FreeLibraryAndExitThread( h_module, 0 );
}

uintptr_t __stdcall start_address( const HMODULE h_module ) {
    logger::initialize( );

    std::filesystem::create_directory( "c:\\airflow\\" );
    std::filesystem::create_directory( "c:\\airflow\\configs\\" );

    g_modules->m_modules.initialize( );
    g_interfaces->initialize( );
    g_directx->initialize( );
    g_hooks->initialize( );
    g_config_system->setup_values( );
    g_entity_system->initialize( );

    LOG_SUCCESS( xorstr_( "[*] DarkSide successfully injected!\n" ) );

    while ( !GetAsyncKeyState( VK_END ) )
    {
        Sleep( 100 );
    }

    destroy( h_module );

    return 0;
}

BOOL APIENTRY DllMain( HMODULE h_module, DWORD  ul_reason_for_call, LPVOID lp_reserved ) {
    if ( ul_reason_for_call == DLL_PROCESS_ATTACH ) {
        auto current_process = WINCALL( GetCurrentProcess )( );
        auto priority_class = WINCALL( GetPriorityClass )( current_process );

        if ( priority_class != HIGH_PRIORITY_CLASS
            && priority_class != REALTIME_PRIORITY_CLASS )
            WINCALL( SetPriorityClass )( current_process, HIGH_PRIORITY_CLASS );

        WINCALL( CreateThread )( NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>( start_address ), h_module, NULL, NULL );

        return true;
    }

    return false;
}