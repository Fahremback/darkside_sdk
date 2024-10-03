#pragma once

#include "../../../sdk/vfunc/vfunc.hpp"

#include <array>
#include "../../../sdk/typedefs/vec_t.hpp"
#include "../../modules/modules.hpp"
#include "../../../utils/utils.hpp"

struct ray_t
{
public:
	vec3_t m_start{};
	vec3_t m_end{};
	vec3_t m_mins{};
	vec3_t m_maxs{};
	std::byte pad_001[ 0x4 ];
	std::uint8_t type{};
};
static_assert( sizeof( ray_t ) == 0x38 );

struct surface_data_t
{
public:
	std::byte pad_001[ 0x8 ];
	float m_pen_data_modifier{};
	float m_dmg_modifier{};
	std::byte pad_002[ 0x4 ];
	int m_material{};
};

static_assert( sizeof( surface_data_t ) == 0x18 );

struct trace_hitbox_data_t
{
public:
	std::byte pad_001[ 0x38 ];
	int m_hit_group{};
	std::byte pad_002[ 0x4 ];
	int m_hitbox_id{};
};
static_assert( sizeof( trace_hitbox_data_t ) == 0x44 );

struct trace_array_element_t {
	std::byte pad_001[ 0x30 ];
};

struct trace_data_t {
	std::int32_t uk1{ };
	float uk2{ 52.0f };
	void* arr_pointer{ };
	std::int32_t uk3{ 128 };
	std::int32_t uk4{ static_cast<std::int32_t>( 0x80000000 ) };
	std::array< trace_array_element_t, 0x80 > arr = { };
	std::byte pad_001[ 0x8 ];
	std::int64_t num_update{ };
	void* pointer_update_value{ };
	std::byte pad_002[ 0xC8 ];
	vec3_t start{ }, end{ };
	std::byte pad_003[ 0x50 ];
};

struct update_value_t {
	float m_previous_length_mod{ };
	float m_current_length_mod{ };
	std::byte pad_001[ 0x8 ];
	short m_handle_index{ };
	std::byte pad_002[ 0x6 ];
};

class c_cs_player_pawn;

struct game_trace_t
{
public:
	game_trace_t( ) = default;

	surface_data_t* get_surface_data_t( );
	int get_hitbox_id( );
	int get_hit_group( );

	void* m_surface{};
	c_cs_player_pawn* m_hit_entity{};
	trace_hitbox_data_t* m_hitbox_data{};
	std::byte pad_001[ 0x38 ];
	std::uint32_t m_contents{};
	std::byte pad_002[ 0x24 ];
	vec3_t m_start{};
	vec3_t m_end{};
	vec3_t m_normal{};
	vec3_t m_pos{};
	std::byte pad_003[ 0x4 ];
	float m_fraction{};
	std::byte pad_004[ 0x6 ];
	bool m_all_solid{};
	std::byte pad_005[ 0x4D ];
};

static_assert( sizeof( game_trace_t ) == 0x108 );

struct trace_filter_t
{
public:
	std::byte pad_001[ 0x8 ];
	std::int64_t m_mask{};
	std::array<std::int64_t, 2> m_ptr{};
	std::array<std::int32_t, 4> m_skip_handles{};
	std::array<std::int16_t, 2> m_arr_collisions{};
	std::int16_t m_ptr2{};
	std::uint8_t m_ptr3{};
	std::uint8_t m_ptr4{};
	std::uint8_t m_ptr5{};

	trace_filter_t( ) = default;
	trace_filter_t( std::uint64_t mask, c_cs_player_pawn* entity, c_cs_player_pawn* player, int layer );
};
static_assert( sizeof( trace_filter_t ) == 0x40 );

class i_trace
{
	using fn_init_trace_info = void( __fastcall* ) ( game_trace_t* );
	using fn_get_trace_info = void( __fastcall* ) ( trace_data_t*, game_trace_t*, float, void* );
public:
	void init_trace( trace_filter_t& filter, c_cs_player_pawn* pawn, uint64_t mask, uint8_t layer, uint16_t unknown ) {
		using fn_init_trace_t = trace_filter_t * ( __fastcall* )( trace_filter_t&, void*, uint64_t, uint8_t, uint16_t );
		static fn_init_trace_t fn = reinterpret_cast<fn_init_trace_t>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 0F B6 41 37 33" ) );

		fn( filter, pawn, mask, layer, unknown );
	}

	void clip_trace_to_player( vec3_t& start, vec3_t& end, trace_filter_t* filter, game_trace_t* trace, float min, int length, float max ) {
		using fn_clip_trace_to_player_t = void( __fastcall* )( vec3_t&, vec3_t&, trace_filter_t*, game_trace_t*, float, int, float );
		static fn_clip_trace_to_player_t fn = reinterpret_cast<fn_clip_trace_to_player_t>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 8B C4 55 56 48 8D A8 58" ) );

		fn( start, end, filter, trace, min, length, max );
	}

	void create_trace( trace_data_t* const trace, vec3_t start, vec3_t end, const trace_filter_t& filter, int penetration_count ) {
		using fn_create_trace_t = void( __fastcall* ) ( trace_data_t*, vec3_t, vec3_t, trace_filter_t, void*, void*, void*, void*, int );
		static fn_create_trace_t fn = reinterpret_cast<fn_create_trace_t>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 5C 24 ?? 48 89 6C 24 ?? 48 89 74 24 ?? 57 41 56 41 57 48 83 EC 40 F2" ) );

		fn( trace, start, end, filter, nullptr, nullptr, nullptr, nullptr, penetration_count );
	}

	void init_trace_info( game_trace_t* const hit ) {
		static fn_init_trace_info fn = reinterpret_cast<fn_init_trace_info>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 5C 24 08 57 48 83 EC 20 48 8B D9 33 FF 48 8B 0D" ) );

		fn( hit );
	}

	void get_trace_info( trace_data_t* const trace, game_trace_t* const hit, const float unknown_float, void* unknown ) {
		static fn_get_trace_info fn = reinterpret_cast<fn_get_trace_info>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 60 48 8B E9 0F" ) );

		fn( trace, hit, unknown_float, unknown );
	}

	bool trace_shape( ray_t* ray, vec3_t start, vec3_t end, trace_filter_t* filter, game_trace_t* game_trace )
	{
		using fn_trace_shape = bool( __fastcall* )( i_trace*, ray_t*, vec3_t*, vec3_t*, trace_filter_t*, game_trace_t* );
		static fn_trace_shape o_trace_shape = reinterpret_cast<fn_trace_shape>( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name(), "E8 ? ? ? ? 80 7D ? ? 75 ? F3 0F 10 45", 0x1 ) );

		return o_trace_shape( this, ray, &start, &end, filter, game_trace );
	}

	bool clip_ray_entity( ray_t* ray, vec3_t start, vec3_t end, c_cs_player_pawn* pawn, trace_filter_t* filter, game_trace_t* game_trace )
	{
		using fn_clip_ray_entity = bool( __fastcall* )( i_trace*, ray_t*, vec3_t*, vec3_t*, c_cs_player_pawn*, trace_filter_t*, game_trace_t* );
		static fn_clip_ray_entity o_clip_ray_entity = reinterpret_cast<fn_clip_ray_entity>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 54 41 56 41 57 48 81 EC C0 00 00 00 48 8B 9C" ) );

		return o_clip_ray_entity( this, ray, &start, &end, pawn, filter, game_trace );
	}
};