#include "protobufs.hpp"

c_subtick_move_step* c_protobuf::add_subtick_move_step( c_user_cmd* user_cmd ) {
	auto rept_field_move_steps = reinterpret_cast<google::protobuf::repeated_ptr_field_t<c_subtick_move_step>*>( (PBYTE) user_cmd->pb.mutable_base( ) + 0x18 );

	if ( rept_field_move_steps->m_rep && rept_field_move_steps->m_current_size < rept_field_move_steps->m_rep->m_allocated_size )
		subtick_move_step = rept_field_move_steps->m_rep->m_elements[ rept_field_move_steps->m_current_size++ ];
	else
		subtick_move_step = create_new_subtick_move_step( rept_field_move_steps, rept_field_move_steps->m_arena );

	return subtick_move_step;
}

c_csgo_input_history_entry_pb* c_protobuf::add_input_history( c_user_cmd* user_cmd ) {
	auto rept_field_input_history = reinterpret_cast<google::protobuf::repeated_ptr_field_t<c_csgo_input_history_entry_pb>*>( (PBYTE)user_cmd->pb.internal_default_instance() + 0x18 );

	if ( rept_field_input_history->m_rep && rept_field_input_history->m_current_size < rept_field_input_history->m_rep->m_allocated_size )
		input_history_entry = rept_field_input_history->m_rep->m_elements[ rept_field_input_history->m_current_size++ ];
	else
		input_history_entry = create_new_input_history( rept_field_input_history, rept_field_input_history->m_arena );

	return input_history_entry;
}

CSGOInterpolationInfoPB* c_protobuf::create_new_interps( CSGOInterpolationInfoPB* mutable_interpolation_info ) {

	if ( !mutable_interpolation_info->GetArena( ) )
		return nullptr;

	CSGOInterpolationInfoPB* interpolation_info_pb = reinterpret_cast<CSGOInterpolationInfoPB *( __fastcall* )( void* )>( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), xorstr_( "E8 ? ? ? ? 48 89 46 58 8B 45 AF" ), 0x1 ) )( mutable_interpolation_info->GetArena( ) );
	return interpolation_info_pb;
}

c_csgo_input_history_entry_pb* c_protobuf::create_new_input_history( google::protobuf::repeated_ptr_field_t<c_csgo_input_history_entry_pb>* rept_ptr, void* arena ) {
	static auto fn_create_new_input_history = reinterpret_cast<c_csgo_input_history_entry_pb * ( __fastcall* )( void* )>( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), xorstr_( "E8 ? ? ? ? 48 8B D0 48 8D 4E ? E8 ? ? ? ? 4C 8B F0" ), 0x1 ) );
	static auto fn_add_element_to_rep_field_container = reinterpret_cast<c_csgo_input_history_entry_pb * ( __fastcall* )( google::protobuf::repeated_ptr_field_t<c_csgo_input_history_entry_pb>*, void* )>( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), xorstr_( "E8 ? ? ? ? 48 8B D0 8B 4A ? F3 41 0F 10 47" ), 0x1 ) );

	auto input_history = fn_create_new_input_history( arena );

	return fn_add_element_to_rep_field_container( rept_ptr, input_history );
}

c_subtick_move_step* c_protobuf::create_new_subtick_move_step( google::protobuf::repeated_ptr_field_t<c_subtick_move_step>* rept_ptr, void* arena ) {
	static auto fn_create_new_subtick_move_step = reinterpret_cast<c_subtick_move_step * ( __fastcall* )( void* )>( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), xorstr_( "E8 ? ? ? ? 48 8B D0 48 8D 4F ? E8 ? ? ? ? 48 8B D0" ), 0x1 ) );
	static auto fn_add_element_to_rep_field_container = reinterpret_cast<c_subtick_move_step * ( __fastcall* )( google::protobuf::repeated_ptr_field_t<c_subtick_move_step>*, void* )>( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), xorstr_( "E8 ? ? ? ? 48 8B D0 8B 4A ? F3 41 0F 10 46" ), 0x1 ) );

	auto subtick_move = fn_create_new_subtick_move_step( arena );

	return fn_add_element_to_rep_field_container( rept_ptr, subtick_move );
}