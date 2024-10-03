#pragma once

#include "../../../sdk/vfunc/vfunc.hpp"
#include "../../../typedefs/c_utl_map.hpp"
#include "../../../sdk/includes/fnv1a.hpp"
#include "../../../sdk/includes/hash.hpp"

class c_econ_item_definition {
public:
	bool is_weapon( ) {
		return get_stickers_supported_count( ) >= 4;
	}

	bool is_agent( ) {
		static auto type_custom_player = fnv1a::hash_64( "#Type_CustomPlayer" );
		if ( fnv1a::hash_64( m_item_type_name ) != type_custom_player )
			return false;

		return get_stickers_supported_count( ) >= 1;
	}

	bool is_knife( bool exclude_default ) {
		static auto csgo_type_knife = fnv1a::hash_64( "#CSGO_Type_Knife" );
		if ( fnv1a::hash_64( m_item_type_name ) != csgo_type_knife )
			return false;

		return exclude_default ? m_definition_index >= 500 : true;
	}

	bool is_glove( bool exclude_default ) {
		static auto type_hands = fnv1a::hash_64( "#Type_Hands" );
		if ( fnv1a::hash_64( m_item_type_name ) != type_hands )
			return false;

		const bool default_glove = m_definition_index == 5028 || m_definition_index == 5029;
		return exclude_default ? !default_glove : true;
	}

	bool is_weapon_case( ) {
		static auto type_weapon_case = fnv1a::hash_64( "#CSGO_Type_WeaponCase" );
		return fnv1a::hash_64( m_item_type_name ) == type_weapon_case;
	}

	bool is_key( ) {
		static auto tool_weapon_case_key_tag = fnv1a::hash_64( "#CSGO_Tool_WeaponCase_KeyTag" );
		return fnv1a::hash_64( m_item_type_name ) == tool_weapon_case_key_tag;
	}

	const char* get_model_name( ) {
		return *reinterpret_cast<const char**>( (uintptr_t)( this ) + 0xD8 );
	}

	int get_stickers_supported_count( ) {
		return *reinterpret_cast<int*>( (uintptr_t)( this ) + 0x100 );
	}

	const char* get_simple_weapon_name( ) {
		return *reinterpret_cast<const char**>( (uintptr_t)( this ) + 0x1F0 );
	}

	int get_loadout_slot( ) {
		return *reinterpret_cast<int*>( (uintptr_t)( this ) + 0x2E8 );
	}
public:
	std::byte pad_001[ 0x8 ];  // vtable
	void* m_kv_item; // 0x8
	std::uint16_t m_definition_index; // 0x10
	std::byte pad_002[ 0x1e ];
	bool m_enabled; // 0x30
	std::byte pad_003[ 0xf ];
	std::uint8_t m_min_item_level; // 0x40
	std::uint8_t m_max_item_level; // 0x41
	std::uint8_t m_item_rarity; // 0x42
	std::uint8_t m_item_quality; // 0x43
	std::uint8_t m_forced_item_quality; // 0x44
	std::uint8_t m_default_drop_item_quality; // 0x45
	std::uint8_t m_default_drop_quantity; // 0x46
	std::byte pad_004[ 0x19 ];
	std::uint8_t m_popularity_seed; // 0x60
	std::byte pad_005[ 0x7 ];
	void* m_portraits_kv; // 0x68
	char* m_item_base_name; // 0x70
	bool m_proper_name; // 0x78
	std::byte pad_006[ 0x7 ];
	char* m_item_type_name; // 0x80
	std::byte pad_007[ 0x8 ];
	char* m_item_desc; // 0x90
	std::uint32_t m_expiration_time_stamp; // 0x98
	std::uint32_t m_creation_time_stamp; // 0x9c
	char* m_inventory_model; // 0xa0
	char* m_inventory_image; // 0xa8
	std::byte pad_008[ 0x18 ];
	int m_inventory_image_position[ 2 ]; // 0xc8
	int m_inventory_image_size[ 2 ]; // 0xd0
	char* m_base_display_model; // 0xd8
	bool m_load_on_demand; // 0xe0
	std::byte pad_009[ 0x1 ];
	bool m_hide_body_groups_deployed_only; // 0xe2
	std::byte pad_010[ 0x5 ];
	char* m_world_display_model; // 0xe8
	char* m_holstered_model; // 0xf0
	char* m_world_extra_wearable_model; // 0xf8
	std::uint32_t m_sticker_slots; // 0x100
	std::byte pad_011[ 0x4 ];
	char* m_icon_default_image; // 0x108
	bool m_attach_to_hands; // 0x110
	bool m_attach_to_hands_vm_only; // 0x111
	bool m_flip_view_model; // 0x112
	bool m_act_as_wearable; // 0x113
	std::byte pad_012[ 0x24 ];
	std::uint32_t m_item_type; // 0x138
	std::byte pad_013[ 0x4 ];
	char* m_brass_model_override; // 0x140
	char* m_zoom_in_sound_path; // 0x148
	char* m_zoom_out_sound_path; // 0x150
	std::byte pad_014[ 0x18 ];
	std::uint32_t m_sound_material_id; // 0x170
	bool m_disable_style_selection; // 0x174
	std::byte pad_015[ 0x13 ];
	char* m_particle_file; // 0x188
	char* m_particle_snapshot_file; // 0x190
	std::byte pad_016[ 0x40 ];
	char* m_item_classname; // 0x1d8
	char* m_item_log_classname; // 0x1e0
	char* m_item_icon_classname; // 0x1e8
	char* m_definition_name; // 0x1f0
	bool m_hidden; // 0x1f8
	bool m_should_show_in_armory; // 0x1f9
	bool m_base_item; // 0x1fa
	bool m_flexible_loadout_default; // 0x1fb
	bool m_imported; // 0x1fc
	bool m_one_per_account_cdkey; // 0x1fd
	std::byte pad_017[ 0xa ];
	char* m_armory_desc; // 0x208
	std::byte pad_018[ 0x8 ];
	char* m_armory_remap; // 0x218
	char* m_store_remap; // 0x220
	char* m_class_token; // 0x228
	char* m_slot_token; // 0x230
	std::uint32_t m_drop_type; // 0x238
	std::byte pad_019[ 0x4 ];
	char* m_holiday_restriction; // 0x240
	std::uint32_t m_sub_type; // 0x248
	std::byte pad_020[ 0xc ];
	std::uint32_t m_equip_region_mask; // 0x258
	std::uint32_t m_equip_region_conflict_mask; // 0x25c
	std::byte pad_021[ 0x50 ];
	bool m_public_item; // 0x2b0
	bool m_ignore_in_collection_view; // 0x2b1
	std::byte pad_022[ 0x36 ];
	int m_loadout_slot; // 0x2e8
	std::byte pad_023[ 0x94 ];
};

struct c_alternate_icon_data
{
	const char* m_simple_name;
	const char* m_large_simple_name;
	std::byte pad_001[ 0x10 ];
};

struct c_paint_kit
{
	std::uint64_t m_id;
	const char* m_name;
	const char* m_description_string;
	const char* m_description_tag;
	std::byte pad_001[ 0x8 ];
	const char* m_pattern;
	const char* m_normal;
	const char* m_logo_material;
	std::byte pad_002[ 0x4 ];
	std::uint32_t m_rarity;
	std::uint32_t m_style;
	float m_rgba_color[ 4 ][ 4 ];
	float m_rgba_logo_color[ 4 ][ 4 ];
	float m_wear_default;
	float m_wear_remap_min;
	float m_wear_remap_max;
	std::uint8_t m_fixed_seed;
	std::uint8_t m_phong_exponent;
	std::uint8_t m_phong_albedo_boost;
	std::uint8_t m_phong_intensity;
	float m_pattern_scale;
	float m_pattern_offset_x_start;
	float m_pattern_offset_x_end;
	float m_pattern_offset_y_start;
	float m_pattern_offset_y_end;
	float m_pattern_rotate_start;
	float m_pattern_rotate_end;
	float m_logo_scale;
	float m_logo_offset_x;
	float m_logo_offset_y;
	float m_logo_rotation;
	bool m_ignore_weapon_size_scale;
	std::byte pad_003[ 0x3 ];
	std::uint32_t m_view_model_exponent_override_size;
	bool m_only_first_material;
	bool m_use_normal_model;
	bool m_use_legacy_model;
	std::byte pad_004[ 0x1 ];
	float m_pearlescent;
	const char* m_vmt_path;
	std::byte pad_005[ 0x8 ];
	const char* m_composite_material_path;
	void* m_vmt_overrides;
	std::byte pad_006[ 0x8 ];
};

class c_sticker_kit
{
public:
	int m_id;
	int m_rarity;
	const char* m_name;
	const char* m_description;
	const char* m_item_name;
	const char* m_material_path;
	const char* m_material_path_no_drips;
	const char* m_inventory_image;
	int m_tournament_id;
	int m_tournament_team_id;
	int m_tournament_player_id;
	bool m_material_path_is_absolute;
	bool m_cannot_trade;
	std::byte pad_001[ 0x2 ];
	float m_rotate_start;
	float m_rotate_end;
	float m_scale_min;
	float m_scale_max;
	float m_wear_min;
	float m_wear_max;
	const char* m_icon_url_small;
	const char* m_icon_url_large;
	void* m_kv_item;
};

inline constexpr std::uint64_t get_alternate_icon_key_for_weapon_paint_wear_item( std::uint16_t def_idx, std::uint32_t paint_id, std::uint32_t wear )
{
	return ( def_idx << 16 ) + ( paint_id << 2 ) + wear;
}

class c_econ_item_schema {
public:
	std::byte pad_001[ 0x128 ];
	c_utl_map<int, c_econ_item_definition*> m_sorted_item_definition_map;
	std::byte pad_002[ 0x128 ];
	c_utl_map<uint64_t, c_alternate_icon_data> m_alternate_icons_map;
	std::byte pad_003[ 0x50 ];
	c_utl_map<int, c_paint_kit*> m_paint_kits;
	c_utl_map<int, c_sticker_kit*> m_sticker_kits;
};

class c_econ_item_system {
public:
	c_econ_item_schema* get_econ_item_schema( ) {
		return *reinterpret_cast<c_econ_item_schema**>( (uintptr_t)( this ) + 0x8 );
	}
};

class i_client {
public:
	c_econ_item_system* get_econ_item_system( ) {
		return vmt::call_virtual<c_econ_item_system*>( this, 123 );
	}

	float get_lerp_time( ) {
		return vmt::call_virtual<float>( this, 166 );
	}
};