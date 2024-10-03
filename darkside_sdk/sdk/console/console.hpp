#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <stdarg.h>

namespace logger
{
    inline void initialize( )
    {
        // allocate console
        AllocConsole( );

        // set console title
        SetConsoleTitleA( xorstr_( "DarkSide SDK" ) );

        // set output handle
        freopen_s( reinterpret_cast<FILE**>( stdout ), "CONOUT$", "w", stdout );
        freopen_s( reinterpret_cast<FILE**>( stderr ), "CONOUT$", "w", stderr );
        freopen_s( reinterpret_cast<FILE**>( stdin ), "CONIN$", "r", stdin );
    }

    inline void shutdown( )
    {
        // close console window
        PostMessageA( GetConsoleWindow( ), WM_CLOSE, 0, 0 );

        // free console
        FreeConsole( );
    }

    inline void log( const char* file, int line, WORD color, const char* fmt, ... )
    {
        // Get current time
        SYSTEMTIME time;
        GetLocalTime( &time );

        // Save the current console attributes
        HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        GetConsoleScreenBufferInfo( hConsole, &consoleInfo );
        WORD originalAttrs = consoleInfo.wAttributes;

        // Print the time
        printf( xorstr_("[%02d:%02d:%02d] "), time.wHour, time.wMinute, time.wSecond );

        // remove path from file
        const char* file_name = strrchr( file, '\\' ) + 1;

        // Set the desired text color for file:line
        SetConsoleTextAttribute( hConsole, color );
        printf( xorstr_( "%s:%d:" ), file_name, line );

        // Reset to original console attributes
        SetConsoleTextAttribute( hConsole, originalAttrs );
        printf( " " );

        // Print the rest of the log message
        va_list args;
        va_start( args, fmt );
        vprintf( fmt, args );
        va_end( args );

        // Print a newline
        printf( "\n" );
    }
}

#define LOG(fmt, ...) logger::log(__FILE__, __LINE__, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, fmt, __VA_ARGS__)
#define LOG_ERROR(fmt, ...) logger::log(__FILE__, __LINE__, FOREGROUND_INTENSITY | FOREGROUND_RED, fmt, __VA_ARGS__)
#define LOG_WARNING(fmt, ...) logger::log(__FILE__, __LINE__, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, fmt, __VA_ARGS__)
#define LOG_SUCCESS(fmt, ...) logger::log(__FILE__, __LINE__, FOREGROUND_INTENSITY | FOREGROUND_GREEN, fmt, __VA_ARGS__)
#define LOG_INFO(fmt, ...) logger::log(__FILE__, __LINE__, FOREGROUND_INTENSITY | FOREGROUND_BLUE, fmt, __VA_ARGS__)