#pragma once

class c_env_sky;
class c_scene_light_object;
class c_aggregate_scene_object;
class c_base_scene_data;
class c_post_processing_volume;
class c_base_entiy;

class c_hook {
	void* m_function;
	void* m_detour;
	void* m_trampoline;
public:
	void hook( void* target, void* dtr ) {
		m_function = target;
		m_detour = dtr;

		MH_CreateHook( m_function, m_detour, &m_trampoline );
		MH_EnableHook( m_function );
	}

	void hook( uintptr_t target, void* dtr ) {
		hook( (void*)target, dtr );
	}

	template <typename tOriginal>
	tOriginal get_original( ) {
		return reinterpret_cast<tOriginal>( m_trampoline );
	}

	void unhook( ) {
		if ( !m_function || !m_detour )
			return;

		MH_DisableHook( m_function );
		MH_RemoveHook( m_function );
	}
};

namespace hooks
{
	namespace create_move
	{
		inline c_hook m_create_move;

		void __fastcall hk_create_move( i_csgo_input* rcx, int slot, bool active );
	}

	namespace present
	{
		inline c_hook m_present;

		HRESULT hk_present( IDXGISwapChain* swap_chain, unsigned int sync_interval, unsigned int flags );
	}

	namespace enable_cursor
	{
		inline c_hook m_enable_cursor;
		inline bool m_enable_cursor_input;

		void* hk_enable_cursor( void* rcx, bool active );
		void unhook( );
	}

	namespace validate_view_angles
	{
		inline c_hook m_validate_view_angles;

		void hk_validate_view_angles( i_csgo_input* input, void* a2 );
	}

	namespace update_global_vars
	{
		inline c_hook m_update_global_vars;

		void hk_update_global_vars( void* source_to_client, void* new_global_vars );
	}

	namespace frame_stage_notify
	{
		inline c_hook m_frame_stage_notify;

		void* hk_frame_stage_notify( void* source_to_client, int stage );
	}

	namespace override_view
	{
		inline c_hook m_override_view;

		void* hk_override_view( void* source_to_client, c_view_setup* view_setup );
	}

	namespace on_add_entity
	{
		inline c_hook m_on_add_entity;

		void* hk_on_add_entity( void* a1, c_entity_instance* entity_instance, int handle );
	}

	namespace on_remove_entity
	{
		inline c_hook m_on_remove_entity;

		void* hk_on_remove_entity( void* a1, c_entity_instance* entity_instance, int handle );
	}

	namespace on_level_init
	{
		inline c_hook m_on_level_init;

		void* hk_on_level_init( void* a1, const char* map_name );
	}

	namespace on_level_shutdown
	{
		inline c_hook m_on_level_shutdown;

		void* hk_on_level_shutdown( void* a1, const char* map_name );
	}

	namespace update_sky_box
	{
		inline c_hook m_update_sky_box;

		void* hk_update_sky_box( c_env_sky* sky );
	}

	namespace draw_light_scene
	{
		inline c_hook m_draw_light_scene;

		void* hk_draw_light_scene( void* a1, c_scene_light_object* a2, __int64 a3 );
	}

	namespace update_aggregate_scene_object
	{
		inline c_hook m_update_aggregate_scene_object;

		void* hk_update_aggregate_scene_object( c_aggregate_scene_object* a1, void* a2 );
	}

	namespace draw_aggregate_scene_object
	{
		inline c_hook m_draw_aggregate_scene_object;

		void* hk_draw_aggregate_scene_object( void* a1, void* a2, c_base_scene_data* a3, int a4, int a5, void* a6, void* a7, void* a8 );
	}

	namespace update_post_processing
	{
		inline c_hook m_update_post_processing;

		void* hk_update_post_processing( c_post_processing_volume* a1, int a2 );
	}

	namespace should_update_sequences
	{
		inline c_hook m_should_update_sequences;

		void* hk_should_update_sequences( void* a1, void* a2, void* a3 );
	}

	namespace should_draw_legs
	{
		inline c_hook m_should_draw_legs;

		void* hk_should_draw_legs( void* a1, void* a2, void* a3, void* a4, void* a5 );
	}

	namespace mark_interp_latch_flags_dirty
	{
		inline c_hook m_mark_interp_latch_flags_dirty;

		void hk_mark_interp_latch_flags_dirty( void* a1, unsigned int a2 );
	}

	namespace draw_scope_overlay
	{
		inline c_hook m_draw_scope_overlay;

		void hk_draw_scope_overlay( void* a1, void* a2 );
	}

	namespace get_field_of_view
	{
		inline c_hook m_get_field_of_view;

		float hk_get_field_of_view( void* a1 );
	}

	namespace mouse_input_enabled
	{
		inline c_hook m_mouse_input_enabled;

		bool hk_mouse_input_enabled( void* ptr );
	}
}

class c_hooks {
public:

	bool initialize( );
	void destroy( );
};

inline const auto g_hooks = std::make_unique<c_hooks>( );