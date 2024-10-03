#pragma once

#include "../darkside.hpp"
#include "../sdk/typedefs/c_color.hpp"

#include "bytes.hpp"

enum e_font_flags
{
	font_flags_center = 1 << 0,
	font_flags_outline = 1 << 1,
	font_flags_dropshadow = 1 << 2
};

struct c_fonts
{
public:
	ImFont* verdana;
	ImFont* verdana_small;
	ImFont* onetap_pixel;
};

class c_render {
public:
	vec3_t m_screen_size;
	vec3_t m_screen_center;
	float m_aspect_ratio;
	ImDrawList* m_background_draw_list;

	c_fonts fonts;

	void update_screen_size( ImGuiIO );

	void initialize( );

	void update_background_drawlist( ImDrawList* );

	bool world_to_screen( vec3_t&, vec3_t& );

	void line( vec3_t start_pos, vec3_t end_pos, c_color color );

	void rect( vec3_t start_pos, vec3_t end_pos, c_color color, float rounding = 0.0f );
	void rect_outline( vec3_t start_pos, vec3_t end_pos, c_color color, float thickness = 1.f, float rounding = 0.0f );

	void text( vec3_t txt_pos, c_color color, const int flags, const ImFont* font, const std::string_view& message, const int font_size );
};

inline const auto g_render = std::make_unique<c_render>( );