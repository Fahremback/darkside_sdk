#pragma once

#include "../schema/schema.hpp"
#include "../../sdk/typedefs/c_handle.hpp"
#include "../../sdk/typedefs/c_strong_handle.hpp"
#include "../../sdk/typedefs/c_utl_vector.hpp"
#include "../../sdk/typedefs/c_utl_memory.hpp"

#include "c_player_camera_service.hpp"
#include "c_model.hpp"

#include "../../sdk/typedefs/matrix_t.hpp"

#include "game_enums.hpp"

class c_econ_item_definition;

#define GET_CLASS_INFO(var_name, dll_name, class_name)                                                                \
    static c_schema_type_scope* type_scope = g_interfaces->m_schema_system->find_type_scope_for_module(dll_name);     \
    if (!type_scope) {                                                                                                \
        printf("Failed to get type scope: %s", dll_name);                                                             \
                                                                                                                      \
        return false;                                                                                                 \
    }                                                                                                                 \
                                                                                                                      \
    static c_schema_class_info* var_name = type_scope->find_declared_class(class_name);                               \
    if (!class_info) {                                                                                                \
        printf("Failed to get class info: %s", class_name);                                                           \
                                                                                                                      \
        return false;                                                                                                 \
    }

class c_skeleton_instace;

class c_game_scene_node {
public:
	SCHEMA( m_origin, vec3_t, "CGameSceneNode", "m_vecOrigin" );
	SCHEMA( m_abs_origin, vec3_t, "CGameSceneNode", "m_vecAbsOrigin" );

	c_skeleton_instace* get_skeleton_instance( ) {
		return vmt::call_virtual<c_skeleton_instace*>( this, 8 );
	}

	void set_mesh_group_mask( uint64_t mask ) {
		static auto fn = reinterpret_cast<void( __thiscall* ) ( void*, uint64_t )>( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), "E8 ? ? ? ? 48 8B 5C 24 ? 4C 8B 7C 24 ? 4C 8B 74 24 ?", 0x1 ) );
		return fn( this, mask );
	}
};

struct alignas( 16 ) c_bone_data {
	vec3_t m_pos;
	float m_scale;
	vec4_t m_rot;
};

class c_model_state {
public:
	SCHEMA( m_model, c_strong_handle<c_model>, "CModelState" , "m_hModel" );

	OFFSET( c_bone_data*, get_bone_data, 0x80 );
};

class c_skeleton_instace : public c_game_scene_node {
	std::byte pad_003[ 0x1CC ]; //0x0000
public:
	int m_bone_count; //0x01CC
private:
	std::byte pad_002[ 0x18 ]; //0x01D0
public:
	int m_mask; //0x01E8
private:
	std::byte pad_001[ 0x4 ]; //0x01EC
public:
	matrix2x4_t* m_bone_cache; //0x01F0

	SCHEMA( m_model_state, c_model_state, "CSkeletonInstance", "m_modelState" );
	SCHEMA( m_hitbox_set, uint8_t, "CSkeletonInstance", "m_nHitboxSet" );

	void calc_world_space_bones( std::uint32_t bone_mask ) {
		static const auto fn = reinterpret_cast< void( __fastcall* )( void*, unsigned int ) >( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC D0" ) );
		return fn( this, bone_mask );
	}
};

class c_entity_identity {
public:
	SCHEMA( m_name, const char*, "CEntityIdentity", "m_name" );
	SCHEMA( m_designer_name, const char*, "CEntityIdentity", "m_designerName" );
	SCHEMA( m_flags, std::uint32_t, "CEntityIdentity", "m_flags" );

	bool is_valid( ) {
		return m_index( ) != INVALID_EHANDLE_INDEX;
	}

	int get_entry_index( ) {
		if ( !is_valid( ) )
			return ENT_ENTRY_MASK;

		return m_index( ) & ENT_ENTRY_MASK;
	}

	int get_serial_number( ) {
		return m_index( ) >> NUM_SERIAL_NUM_SHIFT_BITS;
	}

	bool is_type( const char* name ) {
		return fnv1a::hash_64( name ) == fnv1a::hash_64( m_designer_name( ) );
	}

	OFFSET( int, m_index, 0x10 );
};

class c_entity_instance {
public:
	c_base_handle get_handle( ) {
		c_entity_identity* identity = m_entity( );
		if ( identity == nullptr )
			return c_base_handle( );

		return c_base_handle( identity->m_index( ), identity->get_serial_number( ) - ( identity->m_flags( ) & 1 ) );
	}

	c_schema_class_info* get_schema_class_info( ) {
		c_schema_class_info* class_info = nullptr;

		vmt::call_virtual<void>( this, 38, &class_info );

		return class_info;
	}

	const char* get_entity_class_name( ) {
		c_schema_class_info* class_info = get_schema_class_info( );

		return class_info->get_name( );
	}

	bool is_player( ) {
		return is_player_pawn( ) || is_player_controller( );
	}

	bool is_player_pawn( ) {
		GET_CLASS_INFO( class_info, "client.dll", "C_CSPlayerPawn" );

		return get_schema_class_info( ) == class_info;
	}

	bool is_player_controller( ) {
		GET_CLASS_INFO( class_info, "client.dll", "CCSPlayerController" );

		return get_schema_class_info( ) == class_info;
	}

	SCHEMA( m_entity, c_entity_identity*, "CEntityInstance", "m_pEntity" );
};

class c_collision {
public:
	SCHEMA( m_mins, vec3_t, "CCollisionProperty", "m_vecMins" );
	SCHEMA( m_maxs, vec3_t, "CCollisionProperty", "m_vecMaxs" );

	std::uint16_t get_collision_mask( ) {
		return *reinterpret_cast<std::uint16_t*>( reinterpret_cast<std::uintptr_t>( this ) + 0x38 );
	}

	SCHEMA( m_solid_flags, std::uint8_t, "CCollisionProperty", "m_usSolidFlags" );
	SCHEMA( m_collision_group, std::uint8_t, "CCollisionProperty", "m_CollisionGroup" );
};

class c_base_anim_graph {
public:
	SCHEMA( m_sequence_finished, bool, "CBaseAnimGraphController", "m_bSequenceFinished" );
	SCHEMA( m_sound_sync_time, float, "CBaseAnimGraphController", "m_flSoundSyncTime" );
	SCHEMA( m_active_ik_chain_mask, std::uintptr_t, "CBaseAnimGraphController", "m_nActiveIKChainMask" );
	SCHEMA( m_sequence, int, "CBaseAnimGraphController", "m_hSequence" );
	SCHEMA( m_seq_start_time, int, "CBaseAnimGraphController", "m_flSeqStartTime" );
	SCHEMA( m_seq_fixed_cycle, int, "CBaseAnimGraphController", "m_flSeqFixedCycle" );
	SCHEMA( m_anim_loop_mode, int, "CBaseAnimGraphController", "m_nAnimLoopMode" );
	SCHEMA( m_playback_rate, float, "CBaseAnimGraphController", "m_flPlaybackRate" );
	SCHEMA( m_animation_inputs_changed, bool, "CBaseAnimGraphController", "m_bNetworkedAnimationInputsChanged" );
	SCHEMA( m_sequence_changed, bool, "CBaseAnimGraphController", "m_bNetworkedSequenceChanged" );
	SCHEMA( m_last_update_skipped, bool, "CBaseAnimGraphController", "m_bLastUpdateSkipped" );

	OFFSET( float, m_cached_sequence_cycle_rate, 0x14CC );

	void write_new_sequence( int& sequence ) {
		sequence = m_sequence( );
	}
};

class c_body_component : public c_entity_instance {
public:
	c_base_anim_graph* get_base_anim_graph_controller( ) {
		return reinterpret_cast<c_base_anim_graph*>( std::uintptr_t( this ) + 0x460 );
	}
};

class c_base_entity : public c_entity_instance {
public:
	SCHEMA( m_health, int, "C_BaseEntity", "m_iHealth" );
	SCHEMA( m_flags, int, "C_BaseEntity", "m_fFlags" );
	SCHEMA( m_move_type, int, "C_BaseEntity", "m_MoveType" );
	SCHEMA( m_vec_base_velocity, vec3_t, "C_BaseEntity", "m_vecBaseVelocity" );
	SCHEMA( m_vec_velocity, vec3_t, "C_BaseEntity", "m_vecVelocity" );
	SCHEMA( m_vec_abs_velocity, vec3_t, "C_BaseEntity", "m_vecAbsVelocity" );
	SCHEMA( m_collision, c_collision*, "C_BaseEntity", "m_pCollision" );
	SCHEMA( m_owner_entity, c_base_handle, "C_BaseEntity", "m_hOwnerEntity" );
	SCHEMA( m_scene_node, c_game_scene_node*, "C_BaseEntity", "m_pGameSceneNode" );
	SCHEMA( m_team_num, int, "C_BaseEntity", "m_iTeamNum" );
	SCHEMA( m_sim_time, float, "C_BaseEntity", "m_flSimulationTime" );
	SCHEMA( m_body_component, c_body_component*, "C_BaseEntity", "m_CBodyComponent" );

	void set_body_group( ) {
		static auto fn = reinterpret_cast<void( __fastcall* )( void*, int, unsigned int )>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "85 D2 0F 88 ? ? ? ? 55 56 57" ) );
		fn( this, 0, 1 );
	}

	bool is_weapon( ) {
		return vmt::call_virtual<bool>( this, 156 );
	}

	bool is_view_model( ) {
		return vmt::call_virtual<bool>( this, 242 );
	}

	void set_model( const char* model_name ) {
		static auto fn = reinterpret_cast<void* ( __fastcall* )( void*, const char* )>( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), "E8 ? ? ? ? 41 8B 54 24 ? 49 8B 0C 24", 0x1 ) );
		fn( this, model_name );
	}

	c_hitboxsets* get_hitbox_set( unsigned int index ) {
		using fn_get_hitbox_set = std::int64_t( __fastcall* )( void*, unsigned int );
		static auto fn = reinterpret_cast<fn_get_hitbox_set>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 8B DA 48 8B F9 E8 ? ? ? ? 48 8B F0 48 85 C0 0F 84" ) );
	
		return reinterpret_cast<c_hitboxsets*>( fn( this, index ) );
	}
};

class c_econ_item_view
{
public:
	SCHEMA( m_defenition_index, uint16_t, "C_EconItemView", "m_iItemDefinitionIndex" );
	SCHEMA( m_item_id, uint64_t, "C_EconItemView", "m_iItemID" );
	SCHEMA( m_item_id_low, uint32_t, "C_EconItemView", "m_iItemIDLow" );
	SCHEMA( m_item_id_high, uint32_t, "C_EconItemView", "m_iItemIDHigh" );
	SCHEMA( m_account_id, uint32_t, "C_EconItemView", "m_iAccountID" );
	SCHEMA( m_initialized, bool, "C_EconItemView", "m_bInitialized" );
	SCHEMA( m_custom_name, const char*, "C_EconItemView", "m_szCustomName" );
	SCHEMA( m_custom_name_override, const char*, "C_EconItemView", "m_szCustomNameOverride" );

	int get_custom_paint_kit( ) {
		return vmt::call_virtual<int>( this, 2 );
	}

	c_econ_item_definition* get_static_data( ) {
		return vmt::call_virtual<c_econ_item_definition*>( this, 13 );
	}
};

class c_attribute_container {
public:
	SCHEMA_ARRAY( m_item, c_econ_item_view, "C_AttributeContainer", "m_Item" );
};

class c_player_weapon_service {
public:
	SCHEMA( m_active_weapon, c_base_handle, "CPlayer_WeaponServices", "m_hActiveWeapon" );
	SCHEMA( m_last_weapon, c_base_handle, "CPlayer_WeaponServices", "m_hLastWeapon" );
	SCHEMA( m_weapon, c_network_utl_vector<c_base_handle>, "CPlayer_WeaponServices", "m_hMyWeapons" );
};

class c_base_view_model : public c_base_entity {
public:
	SCHEMA( m_weapon, c_base_handle, "C_BaseViewModel", "m_hWeapon" );
};

class c_cs_player_view_model_service {
public:
	SCHEMA( m_view_model, c_base_handle, "CCSPlayer_ViewModelServices", "m_hViewModel" );
};

class c_econ_entity : public c_base_entity
{
public:
	SCHEMA_ARRAY( m_attribute_manager, c_attribute_container, "C_EconEntity", "m_AttributeManager" );
	SCHEMA( m_paint_kit, int, "C_EconEntity", "m_nFallbackPaintKit" );
	SCHEMA( m_seed, int, "C_EconEntity", "m_nFallbackSeed" );
	SCHEMA( m_xuid_low, uint32_t, "C_EconEntity", "m_OriginalOwnerXuidLow" );
	SCHEMA( m_xuid_high, uint32_t, "C_EconEntity", "m_OriginalOwnerXuidHigh" );
	SCHEMA( m_wear, float, "C_EconEntity", "m_flFallbackWear" );
	SCHEMA( m_statrack, int, "C_EconEntity", "m_nFallbackStatTrak" );
};

class c_player_movement_service {
public:
	SCHEMA( m_max_speed, float, "CPlayer_MovementServices", "m_flMaxspeed" );
	SCHEMA( m_surface_friction, float, "CPlayer_MovementServices_Humanoid", "m_flSurfaceFriction" );

	void set_prediction_command( c_user_cmd* user_cmd ) {

		vmt::call_virtual<void>( this, 34, user_cmd );
	}

	void reset_prediction_command( ) {
		vmt::call_virtual<void>( this, 35 );
	}
};

using firing_float_t = float[ 2 ];

class c_cs_weapon_base_v_data
{
public:
	SCHEMA( m_max_clip1, int32_t, "CBasePlayerWeaponVData", "m_iMaxClip1" );
	SCHEMA( m_max_clip2, int32_t, "CBasePlayerWeaponVData", "m_iMaxClip2" );
	SCHEMA( m_default_clip1, int32_t, "CBasePlayerWeaponVData", "m_iDefaultClip1" );
	SCHEMA( m_default_clip2, int32_t, "CBasePlayerWeaponVData", "m_iDefaultClip2" );
	SCHEMA( m_weight, int32_t, "CBasePlayerWeaponVData", "m_iWeight" );
	SCHEMA( m_name, const char*, "CCSWeaponBaseVData", "m_szName" );
	SCHEMA( m_cycle_time, firing_float_t, "CCSWeaponBaseVData", "m_flCycleTime" );
	SCHEMA( m_damage, int, "CCSWeaponBaseVData", "m_nDamage" );
	SCHEMA( m_armor_ratio, float, "CCSWeaponBaseVData", "m_flArmorRatio" );
	SCHEMA( m_range, float, "CCSWeaponBaseVData", "m_flRange" );
	SCHEMA( m_range_modifier, float, "CCSWeaponBaseVData", "m_flRangeModifier" );
	SCHEMA( m_penetration, float, "CCSWeaponBaseVData", "m_flPenetration" );
	SCHEMA( m_headshot_multiplier, float, "CCSWeaponBaseVData", "m_flHeadshotMultiplier" );
	SCHEMA( m_spread, float, "CCSWeaponBaseVData", "m_flSpread" );
	SCHEMA( m_bullets, float, "CCSWeaponBaseVData", "m_nNumBullets" );
	SCHEMA( m_weapon_type, int, "CCSWeaponBaseVData", "m_WeaponType" );
};

class c_base_player_weapon : public c_econ_entity
{
public:
	SCHEMA( m_next_primary_attack, int32_t, "C_BasePlayerWeapon", "m_nNextPrimaryAttackTick" );
	SCHEMA( m_next_primary_attack_ratio, float, "C_BasePlayerWeapon", "m_flNextPrimaryAttackTickRatio" );
	SCHEMA( m_next_secondary_attack, int32_t, "C_BasePlayerWeapon", "m_nNextSecondaryAttackTick" );
	SCHEMA( m_next_secondary_attack_ratio, float, "C_BasePlayerWeapon", "m_flNextSecondaryAttackTickRatio" );
	SCHEMA( m_clip1, int32_t, "C_BasePlayerWeapon", "m_iClip1" );
	SCHEMA( m_clip2, int32_t, "C_BasePlayerWeapon", "m_iClip2" );

	SCHEMA( m_in_reload, bool, "C_CSWeaponBase", "m_bInReload" );

	SCHEMA( m_burst_mode, bool, "C_CSWeaponBase", "m_bBurstMode" );
	SCHEMA( m_burst_shots_remaining, int, "C_CSWeaponBaseGun", "m_iBurstShotsRemaining" );

	c_cs_weapon_base_v_data* get_weapon_data( ) {
		return *reinterpret_cast<c_cs_weapon_base_v_data**>( reinterpret_cast<uintptr_t>( this ) + 0x360 );
	}

	float get_max_speed( ) {
		return vmt::call_virtual<float>( this, 340 );
	}

	float get_inaccuracy( ) {
		using fn_get_inaccuracy_t = float( __fastcall* )( void* );
		static fn_get_inaccuracy_t fn = reinterpret_cast<fn_get_inaccuracy_t>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 5C 24 ? 55 56 57 48 81 EC ? ? ? ? 0F 29 B4 24" ) );

		return fn( this );
	}

	float get_spread( ) {
		using fn_get_spread_t = float( __fastcall* )( void* );
		static fn_get_spread_t fn = reinterpret_cast<fn_get_spread_t>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 83 EC ? 48 63 91" ) );

		return fn( this );
	}

	void update_accuracy_penality( ) {
		using fn_update_accuracy_penality_t = void( __fastcall* )( void* );
		static fn_update_accuracy_penality_t fn = reinterpret_cast<fn_update_accuracy_penality_t>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 E8 ? ? ? ? 48 8B D8 48 85 C0 0F 84 ? ? ? ? 44 0F 29 44 24" ) );

		fn( this );
	}
};

class c_cs_weapon_base : public c_base_player_weapon {
public:
	SCHEMA( m_zoom_level, int, "C_CSWeaponBaseGun", "m_zoomLevel" );
	SCHEMA( m_burst_mode, bool, "C_CSWeaponBase", "m_bBurstMode" );
};

class c_base_cs_grenade : public c_cs_weapon_base {
public:
	SCHEMA( m_held_by_player, bool, "C_BaseCSGrenade", "m_bIsHeldByPlayer" );
	SCHEMA( m_pin_pulled, bool, "C_BaseCSGrenade", "m_bPinPulled" );
	SCHEMA( m_throw_time, float, "C_BaseCSGrenade", "m_fThrowTime" );
	SCHEMA( m_throw_strength, float, "C_BaseCSGrenade", "m_flThrowStrength" );
};

class c_cs_player_item_service {
public:
	SCHEMA( m_has_helmet, bool, "CCSPlayer_ItemServices", "m_bHasHelmet" );
};

class c_cs_player_pawn : public c_base_entity {
public:
	SCHEMA( m_weapon_services, c_player_weapon_service*, "C_BasePlayerPawn", "m_pWeaponServices" );
	SCHEMA( m_controller, c_base_handle, "C_BasePlayerPawn", "m_hController" );
	SCHEMA( m_view_model_services, c_cs_player_view_model_service*, "C_CSPlayerPawnBase", "m_pViewModelServices" );
	SCHEMA( m_econ_glove, c_econ_item_view, "C_CSPlayerPawn", "m_EconGloves" );
	SCHEMA( m_spawn_time_index, float, "C_CSPlayerPawnBase", "m_flLastSpawnTimeIndex" );
	SCHEMA( m_need_to_reapply_glove, bool, "C_CSPlayerPawn", "m_bNeedToReApplyGloves" );
	SCHEMA( m_armor_value, int, "C_CSPlayerPawn", "m_ArmorValue" );
	SCHEMA( m_movement_services, c_player_movement_service*, "C_BasePlayerPawn", "m_pMovementServices" );
	SCHEMA( m_camera_services, c_player_camera_service*, "C_BasePlayerPawn", "m_pCameraServices" );
	SCHEMA( m_item_services, c_cs_player_item_service*, "C_BasePlayerPawn", "m_pItemServices" );
	SCHEMA( m_scoped, c_cs_player_item_service*, "C_CSPlayerPawn", "m_bIsScoped" );

	vec3_t get_eye_pos( ) {
		vec3_t view_;
		vmt::call_virtual<void>( this, 166, &view_ );
		return view_;
	}

	vec3_t get_bone_position( int index ) {
		using GetBonePosition_t = int( __fastcall* )( void*, int, vec3_t&, vec3_t& );
		static GetBonePosition_t fn = reinterpret_cast<GetBonePosition_t>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 4D 8B F1 49 8B E8" ) );

		vec3_t position;
		vec3_t rotation;

		fn( this, index, position, rotation );

		return position;
	}

	vec3_t get_bone_rotation( int index ) {
		using GetBonePosition_t = int( __fastcall* )( void*, int, vec3_t&, vec3_t& );
		static GetBonePosition_t fn = reinterpret_cast<GetBonePosition_t>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 4D 8B F1 49 8B E8" ) );

		vec3_t position;
		vec3_t rotation;

		fn( this, index, position, rotation );

		return rotation;
	}

	bool is_alive( ) {
		return m_health( ) > 0;
	}

	bool has_armor( int hitgroup ) {
		if ( hitgroup == 1 )
			return this->m_item_services( )->m_has_helmet( );

		return this->m_armor_value( );
	}

	std::uint32_t get_owner_handle_index( ) {
		std::uint32_t result = -1;
		if ( this && m_collision( ) && !( m_collision( )->m_solid_flags( ) & 4 ) )
			result = this->m_owner_entity( ).get_entry_index( );

		return result;
	}

	c_base_player_weapon* get_active_weapon( ) {
		c_player_weapon_service* weapon_services = this->m_weapon_services( );
		if ( weapon_services ) {
			c_base_player_weapon* active_weapon = reinterpret_cast<c_base_player_weapon*>( g_interfaces->m_entity_system->get_base_entity( weapon_services->m_active_weapon( ).get_entry_index( ) ) );
			if ( active_weapon && active_weapon->is_weapon( ) )
				return active_weapon;
		}

		return nullptr;
	}

	bool is_throwing( ) {
		c_base_player_weapon* active_weapon = this->get_active_weapon( );

		if ( !active_weapon )
			return false;

		c_base_cs_grenade* grenade = reinterpret_cast<c_base_cs_grenade*>( active_weapon );
		c_cs_weapon_base_v_data* weapon_data = active_weapon->get_weapon_data( );

		if ( !grenade || !weapon_data )
			return false;

		if ( weapon_data->m_weapon_type( ) == WEAPONTYPE_GRENADE && grenade->m_throw_time( ) != 0.f )
			return true;

		return false;
	}

	int get_bone_index( const char* name ) {
		static const auto fn = reinterpret_cast< int( __fastcall* )( void*, const char* ) >( g_opcodes->scan_absolute( g_modules->m_modules.client_dll.get_name( ), "E8 ? ? ? ? 33 DB 89 47 08", 0x1 ) );
		return fn( this, name );
	}
};