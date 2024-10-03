#pragma once
#include "../typedefs/c_color.hpp"

#include <vector>
#include <string>

#include <memory>

class c_dumped_item;

#define g_cfg g_config_system

template <typename T>
struct item_t {
	T* pointer;
	std::string category;
	std::string name;
	T default_value;
};

class c_config_system {
private:
	std::vector<item_t<bool>> booleans;
	std::vector<item_t<int>> ints;
	std::vector<item_t<float>> floats;
	std::vector<item_t<int>> keybinds;
	std::vector<item_t<std::string>> strings;
	std::vector<item_t<c_color>> colors_rgb;
public:
	void push_item( bool* pointer, std::string category, std::string name, bool def_value );
	void push_item( int* pointer, std::string category, std::string name, int def_value );
	void push_item( float* pointer, std::string category, std::string name, float def_value );
	void push_item( c_color* pointer, std::string category, std::string name, c_color def_value );

	void setup_values( );
	void read( std::string path );
	void save( std::string path );
	void remove( std::string path );
	void refresh( );
	void reset( );
	void menu( );
	std::vector<std::string> config_files;

	struct rage_bot_t
	{
		bool m_enabled;
		bool m_hitboxes[ 6 ];
		bool m_multi_points[ 6 ];
		bool m_static_scale;
		bool m_silent = true;
		int m_hitbox_scale[ 6 ] = { 50 };
		int m_minimum_damage = 50;
		int m_minimum_damage_override = 10;
		int m_hit_chance = 50;
		bool m_auto_fire = true;
		bool m_auto_stop;

		int m_override_damage_key_bind, m_override_damage_key_bind_style;

		// TO-DO: make settings for weapon groups
		struct {

		} weapon_group[ 7 ];
	} rage_bot;

	struct anti_hit_t
	{
		bool m_enabled;
		bool m_at_target;
		bool m_override_pitch;
		bool m_override_yaw;
		int m_pitch_amount = 0;
		int m_yaw_amount = 0;
		int m_jitter_amount = 0;
	} anti_hit;

	struct visuals_t
	{
		struct player_esp_t {
			bool m_bounding_box = true;
			bool m_health_bar = true;
			bool m_name = true;
			bool m_weapon = true;
			bool m_flags = true;
			bool m_teammates = false;

			c_color m_name_color{ 1.f, 1.f, 1.f, 1.f };
		} m_player_esp;
	} visuals;

	struct world_esp_t {
		bool m_enable_thirdperson;
		bool m_enable_override_fov;
		int m_distance;
		int m_override_fov;

		int m_thirdperson_key_bind, m_thirdperson_key_bind_style;
	} world_esp;

	struct world_modulation_t {
		c_color m_sky{ 0.36f, 0.36f, 0.36f }, m_sky_clouds{ 0.37f, 0.84f, 0.98f, 1.f }, m_wall{ 1.f, 1.f, 1.f }, m_lighting{ 0.35f, 0.35f, 0.52f };
		int m_exposure = 20;
		bool m_render_fog;

		int m_removals;
	} world;

	struct misc_t
	{
		bool m_bunny_hop;
		bool m_auto_strafe;
		int m_strafe_smooth = 70;
	} misc;

	struct skins_t {
		struct
		{
			int m_selected = 0;
			int m_selected_ct = 0;
		} m_agents;

		struct skin_data_t
		{
			int m_paint_kit = 0;
			int m_item_def_index = 0;

			int m_prev_skin = -1;
		} m_skin_settings[ 100 ];

		struct
		{
			int m_selected = 0;
		} m_knives;

		struct
		{
			int m_selected = 0;
		} m_gloves;

		int m_selected_weapon = 0;
		int m_selected_type = 0;
		c_dumped_item* m_selected_item = nullptr;
	} skins;
};

inline const auto g_config_system = std::make_unique<c_config_system>( );