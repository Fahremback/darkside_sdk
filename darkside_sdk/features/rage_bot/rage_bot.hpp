#pragma once

#include "../../darkside.hpp"
#include "../auto_wall/auto_wall.hpp"
#include "../eng_pred/eng_pred.hpp"

struct lag_record_t {
	vec3_t m_origin{};
	c_cs_player_pawn* m_pawn{};
	c_skeleton_instace* m_skeleton = nullptr;
	c_bone_data m_bone_data[ 128 ];
	c_bone_data m_bone_data_backup[ 128 ];
	float m_simulation_time{};
	vec3_t m_vec_mins{};
	vec3_t m_vec_maxs{};
	vec3_t m_vec_velocity{};

	bool m_throwing{};

	lag_record_t( ) {}

	inline lag_record_t( c_cs_player_pawn* pawn ) {
		this->store( pawn );
	}

	inline void store( c_cs_player_pawn* pawn ) {
		if ( !pawn || !pawn->is_player_pawn( ) || !pawn->is_alive( ) )
			return;

		auto scene_node = pawn->m_scene_node( );
		if ( !scene_node )
			return;

		auto skeleton_instance = pawn->m_scene_node( )->get_skeleton_instance( );
		if ( !skeleton_instance )
			return;

		auto collision = pawn->m_collision( );
		if ( !collision )
			return;

		static auto setup_bones = reinterpret_cast<void( __fastcall* )( void*, int )>( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), "E8 ? ? ? ? 49 8B 96 ? ? ? ? 48 8B CE", 0x1 ) );

		m_pawn = pawn;

		m_skeleton = skeleton_instance;
		m_origin = scene_node->m_abs_origin( );

		setup_bones( m_skeleton, 128 );

		std::memcpy( m_bone_data, skeleton_instance->m_bone_cache, sizeof( matrix2x4_t ) * skeleton_instance->m_bone_count );

		m_simulation_time = pawn->m_sim_time( );
		m_vec_mins = collision->m_mins( );
		m_vec_maxs = collision->m_maxs( );
		m_vec_velocity = pawn->m_vec_velocity( );

		m_throwing = pawn->is_throwing( );
	}

	inline void apply( c_cs_player_pawn* pawn ) {
		if ( !pawn || !pawn->is_player_pawn( ) || !pawn->is_alive( ) )
			return;

		auto scene_node = pawn->m_scene_node( );
		if ( !scene_node )
			return;

		auto skeleton_instance = scene_node->get_skeleton_instance( );
		if ( !skeleton_instance )
			return;

		std::memcpy( m_bone_data_backup, skeleton_instance->m_bone_cache, sizeof( matrix2x4_t ) * skeleton_instance->m_bone_count );

		std::memcpy( skeleton_instance->m_bone_cache, m_bone_data, sizeof( matrix2x4_t ) * skeleton_instance->m_bone_count );
	}

	inline void reset( c_cs_player_pawn* pawn ) {
		if ( !pawn || !pawn->is_player_pawn( ) || !pawn->is_alive( ) )
			return;

		auto scene_node = pawn->m_scene_node( );
		if ( !scene_node )
			return;

		auto skeleton_instance = scene_node->get_skeleton_instance( );
		if ( !skeleton_instance )
			return;

		std::memcpy( skeleton_instance->m_bone_cache, m_bone_data_backup, sizeof( matrix2x4_t ) * skeleton_instance->m_bone_count );
	}

	bool is_valid( ) {
		float dead_time = ( g_prediction->get_local_data( )->m_tick_base * INTERVAL_PER_TICK ) - m_simulation_time;

		return dead_time < 0.2f;
	}
};


struct hitbox_data_t
{
	hitbox_data_t( ) = default;

	int m_num_hitbox;
	int m_num_bone;
	int m_num_hitgroup;
	c_hitbox* m_hitbox_data;
	float m_radius;
	matrix3x4_t m_matrix;
	vec3_t m_mins;
	vec3_t m_maxs;
	bool m_invalid_data{ true };
};

struct aim_point_t {
	aim_point_t( vec3_t point, int hitbox, bool center = false ) {
		this->m_point = point;
		this->m_hitbox = hitbox;
		this->m_center = center;
	}
	
	vec3_t m_point{};
	int m_hitbox{};
	bool m_center{};
};

struct aim_target_t {
    lag_record_t* m_lag_record{};
    std::unique_ptr<aim_point_t> m_best_point{};
    c_cs_player_pawn* m_pawn{nullptr};

    aim_target_t() = default;

    inline aim_target_t( lag_record_t* record ) {
		this->store( record );
	}

	inline void store( lag_record_t* record ) {
		if ( !record || !record->m_pawn || !record->m_pawn->is_player_pawn( ) )
			return;

		m_lag_record = record;
		m_pawn = record->m_pawn;
	}

    inline void reset() {
        m_lag_record = nullptr;
        m_pawn = nullptr;
		m_best_point.reset( );
    }
};

class c_rage_bot {
	bool m_debug = true;

	std::vector<int> m_hitboxes;
	std::unordered_map<int, std::deque<lag_record_t>> m_lag_records{};
	std::unordered_map<int, aim_target_t> m_aim_targets{};
	aim_target_t* m_best_target{};

	inline int get_hitbox_from_menu( int hitbox ) {
		switch ( hitbox ) {
		case HITBOX_HEAD:
			return 0;
		case HITBOX_CHEST:
		case HITBOX_LOWER_CHEST:
		case HITBOX_UPPER_CHEST:
			return 1;
		case HITBOX_PELVIS:
		case HITBOX_STOMACH:
			return 2;
		case HITBOX_RIGHT_HAND:
		case HITBOX_LEFT_HAND:
		case HITBOX_RIGHT_UPPER_ARM:
		case HITBOX_RIGHT_FOREARM:
		case HITBOX_LEFT_UPPER_ARM:
		case HITBOX_LEFT_FOREARM:
			return 3;
		case HITBOX_RIGHT_THIGH:
		case HITBOX_LEFT_THIGH:
		case HITBOX_RIGHT_CALF:
		case HITBOX_LEFT_CALF:
			return 4;
		case HITBOX_RIGHT_FOOT:
		case HITBOX_LEFT_FOOT:
			return 5;
		}

		return -1;
	}

	hitbox_data_t get_hitbox_data( c_cs_player_pawn* player_pawn, const int hitbox_id ) {
		if ( !player_pawn )
			return hitbox_data_t( );

		auto game_scene = player_pawn->m_scene_node( );
		if ( !game_scene )
			return hitbox_data_t( );

		auto skeleton = game_scene->get_skeleton_instance( );
		if ( !skeleton )
			return hitbox_data_t( );

		auto& model_state = skeleton->m_model_state( );
		auto& model = model_state.m_model( );
		if ( !model )
			return hitbox_data_t( );

		auto hitbox_data = model->get_hitbox( hitbox_id );
		if ( !hitbox_data )
			return hitbox_data_t( );

		const auto bone_index = player_pawn->get_bone_index( hitbox_data->m_bone_name );
		const auto bones = model_state.get_bone_data( );

		hitbox_data_t hitbox;
		hitbox.m_num_hitbox = hitbox_id;
		hitbox.m_num_bone = bone_index;
		hitbox.m_hitbox_data = hitbox_data;
		hitbox.m_radius = hitbox_data->m_shape_radius;
		hitbox.m_num_hitgroup = hitbox_data->m_group_id;
		hitbox.m_matrix = g_math->transform_to_matrix( bones[ bone_index ] );
		hitbox.m_mins = hitbox_data->m_min_bounds;
		hitbox.m_maxs = hitbox_data->m_max_bounds;
		hitbox.m_invalid_data = false;

		return hitbox;
	}

	inline lag_record_t* get_first_record( int handle ) {
		if ( m_lag_records.find( handle ) == m_lag_records.end( ) )
			return nullptr;

		auto& records = m_lag_records[ handle ];

		if ( records.empty( ) )
			return nullptr;

		auto record = &records.front( );
		if ( !record->is_valid( ) )
			return nullptr;

		return record;
	}

	inline lag_record_t* get_last_record( int handle ) {
		if ( m_lag_records.find( handle ) == m_lag_records.end( ) )
			return nullptr;

		auto& records = m_lag_records[ handle ];

		if ( records.empty( ) )
			return nullptr;

		auto record = &records.back( );
		if ( record->is_valid( ) )
			return nullptr;

		return record;
	}

	struct spread_data_t
	{
		float m_random[ 2 ];
		float m_inaccuracy[ 2 ];
		float m_spread[ 2 ];
	} m_spread_data;

	uint32_t get_random_seed( vec3_t view_angles, int player_tick ) {
		using get_random_seed_t = __int64( __fastcall* )( int, vec3_t* a1, int player_render_tick );
		static auto fn = reinterpret_cast<get_random_seed_t>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), xorstr_( "48 81 EC ? ? ? ? F3 0F 10 0A" ) ) );
		return fn( 0, &view_angles, player_tick );
	}

	vec3_t get_removed_aim_punch_angle( c_cs_player_pawn* local_player ) {
		using fnUpdateAnimPunch = void( __fastcall* )( void*, vec3_t*, float, bool );
		static fnUpdateAnimPunch fn = reinterpret_cast<fnUpdateAnimPunch>(
			g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), xorstr_( "E8 ?? ?? ?? ?? F3 0F 10 44 24 ?? 48 8D 8D ?? ?? ?? ?? F3 0F 58 46" ), 0x1 ) );

		vec3_t aim_punch = {};
		fn( local_player, &aim_punch, 0.0f, true );
		return aim_punch;
	}

	void store_hitboxes( );
	lag_record_t* select_record( int handle );
	void find_targets( );
	vec3_t extrapolate( c_cs_player_pawn* pawn, vec3_t pos );
	float calc_point_scale( vec3_t& point, const float& hitbox_radius );
	vec3_t transform_point( matrix3x4_t matrix, vec3_t point );
	std::vector<vec3_t> calculate_sphere_points( float radius, int num_points );
	std::vector<vec3_t> calculate_points( int num_points, float radius, float height, matrix3x4_t matrix, hitbox_data_t hitbox );
	bool multi_points( lag_record_t* record, int hitbox, std::vector<aim_point_t>& points );
	aim_point_t select_points( lag_record_t* record, float& damage );
	aim_target_t* get_nearest_target( );
	void select_target( );
	bool weapon_is_at_max_accuracy( c_cs_weapon_base_v_data* weapon_data, float inaccuracy );
	vec3_t calculate_spread_angles( vec3_t angle, int random_seed, float weapon_inaccuarcy, float weapon_spread );
	int calculate_hit_chance( c_cs_player_pawn* pawn, vec3_t angles, c_base_player_weapon* active_weapon, c_cs_weapon_base_v_data* weapon_data, bool no_spread );
	void process_backtrack( lag_record_t* record );
	bool can_shoot( c_cs_player_pawn* pawn, c_base_player_weapon* active_weapon );
	void process_attack( c_user_cmd* user_cmd, vec3_t angle );
public:
	void debug( c_cs_player_pawn* pawn );

	void store_records( );
	void on_create_move( );
};

const auto g_rage_bot = std::make_unique<c_rage_bot>( );