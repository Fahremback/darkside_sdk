#include "../../darkside.hpp"

void c_modules::modules_t::initialize( ) {
	client_dll = c_dll( xorstr_( "client.dll" ) );
	engine2_dll = c_dll( xorstr_( "engine2.dll" ) );
	schemasystem_dll = c_dll( xorstr_( "schemasystem.dll" ) );
	tier0_dll = c_dll( xorstr_( "tier0.dll" ) );
	navsystem_dll = c_dll( xorstr_( "navsystem.dll" ) );
	rendersystem_dll = c_dll( xorstr_( "rendersystemdx11.dll" ) );
	localize_dll = c_dll( xorstr_( "localize.dll" ) );
	scenesystem_dll = c_dll( xorstr_( "scenesystem.dll" ) );
	materialsystem2_dll = c_dll( xorstr_( "materialsystem2.dll" ) );
	resourcesystem_dll = c_dll( xorstr_( "resourcesystem.dll" ) );
	input_system = c_dll( xorstr_( "inputsystem.dll" ) );
	animation_system = c_dll( xorstr_( "animationsystem.dll" ) );

	LOG_INFO( xorstr_( "[+] Modules initialization completed!" ) );
}