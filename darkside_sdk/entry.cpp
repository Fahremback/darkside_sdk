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

#include <fstream>

void debug_log(const char* msg) {
    std::ofstream out("c:\\airflow\\debug_darkside.txt", std::ios_base::app);
    out << msg << std::endl;
}

uintptr_t __stdcall start_address( const HMODULE h_module ) {
    debug_log("[+] start_address executing.");
    
    logger::initialize( );
    debug_log("[+] logger initialized.");

    std::error_code ec;
    std::filesystem::create_directory( "c:\\airflow\\", ec );
    std::filesystem::create_directory( "c:\\airflow\\configs\\", ec );
    debug_log("[+] dirs created.");

    g_modules->m_modules.initialize( );
    debug_log("[+] modules initialized.");
    
    g_interfaces->initialize( );
    debug_log("[+] interfaces initialized.");
    
    g_directx->initialize( );
    debug_log("[+] directx initialized.");
    
    g_hooks->initialize( );
    debug_log("[+] hooks initialized.");
    
    g_config_system->setup_values( );
    debug_log("[+] configs setup.");
    
    g_entity_system->initialize( );
    debug_log("[+] entity system initialized.");

    LOG_SUCCESS( xorstr_( "[*] DarkSide successfully injected!\n" ) );
    debug_log("[+] fully injected.");

    // Otimizado: Loop mais responsivo com verificação de bit de estado
    while ( !( GetAsyncKeyState( VK_END ) & 0x8000 ) )
    {
        // Reduzido de 100ms para 10ms para melhor responsividade
        Sleep( 10 );
    }

    destroy( h_module );

    return 0;
}

BOOL APIENTRY DllMain( HMODULE h_module, DWORD  ul_reason_for_call, LPVOID lp_reserved ) {
    if ( ul_reason_for_call == DLL_PROCESS_ATTACH ) {
        std::ofstream out("c:\\airflow\\debug_darkside.txt", std::ios_base::trunc);
        out << "[+] DLL_PROCESS_ATTACH" << std::endl;
        
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