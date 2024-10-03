#include "render.hpp"
#include "../sdk/includes/imgui/imgui_freetype.h"

void c_render::update_screen_size( ImGuiIO io ){
	const ImVec2 display_size = io.DisplaySize;

	m_screen_size = vec3_t( display_size.x, display_size.y );

	m_screen_center = m_screen_size * 0.5;

	m_aspect_ratio = m_screen_size.x / m_screen_size.y;
}

void c_render::initialize( ) {
	auto& io = ImGui::GetIO( );
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	ImFontConfig cfg;
	cfg.PixelSnapH = false;
	cfg.OversampleH = 5;
	cfg.OversampleV = 5;

	fonts.verdana = io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\Verdana.ttf", 13.f, nullptr, io.Fonts->GetGlyphRangesCyrillic( ) );
	fonts.verdana_small = io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\Verdana.ttf", 12.f, &cfg, io.Fonts->GetGlyphRangesCyrillic( ) );

	auto prev_flags = cfg.FontBuilderFlags;
	cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_Monochrome;
	fonts.onetap_pixel = io.Fonts->AddFontFromMemoryTTF( onetap_pixel_font, sizeof( onetap_pixel_font ), 8.f, &cfg, io.Fonts->GetGlyphRangesCyrillic( ) );
	cfg.FontBuilderFlags = prev_flags;

	io.FontDefault = fonts.verdana;
}

void c_render::update_background_drawlist( ImDrawList* draw_list ) {
	m_background_draw_list = draw_list;
}

bool c_render::world_to_screen( vec3_t& in, vec3_t& out ) {

	static auto screen_transofrm = reinterpret_cast<bool( __fastcall* )( vec3_t&, vec3_t& )>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "48 89 74 24 ? 57 48 83 EC 20 48 8B 05 ? ? ? ? 48 8B FA" ) );

	if ( !screen_transofrm )
		return false;

	bool on_screen = screen_transofrm( in, out );
	if ( on_screen )
		return false;

	const float screen_size_x = m_screen_size.x;
	const float screen_size_y = m_screen_size.y;

	out.x = ( ( out.x + 1.0f ) * 0.5f ) * screen_size_x;
	out.y = screen_size_y - ( ( ( out.y + 1.0f ) * 0.5f ) * screen_size_y );

	return true;
}

void c_render::line( vec3_t start_pos, vec3_t end_pos, c_color color ) {
	m_background_draw_list->AddLine( start_pos.im( ), end_pos.im( ), color.im( ) );
}

void c_render::rect( vec3_t start_pos, vec3_t end_pos, c_color color, float rounding ) {
	m_background_draw_list->AddRectFilled( start_pos.im( ), end_pos.im( ), color.im( ), rounding );
}

void c_render::rect_outline( vec3_t start_pos, vec3_t end_pos, c_color color, float thickness, float rounding ) {
	m_background_draw_list->AddRect( start_pos.im( ), end_pos.im( ), color.im( ), rounding, 0, thickness );
}

void c_render::text( vec3_t txt_pos, c_color color, const int flags, const ImFont* font, const std::string_view& message, const int font_size ) {
	m_background_draw_list->PushTextureID( font->ContainerAtlas->TexID );

	const auto size = font->CalcTextSizeA( font_size, FLT_MAX, 0.0f, message.data( ) );
	auto pos = ImVec2( txt_pos.x, txt_pos.y );
	auto outline_clr = c_color( 0, 0, 0, color.a * 0.3f );

	if ( flags & font_flags_center )
		pos = ImVec2( txt_pos.x - size.x / 2.0f, txt_pos.y );

	if ( flags & font_flags_dropshadow )
		m_background_draw_list->AddText( ImVec2( pos.x + 1, pos.y + 1 ), outline_clr.im( ), message.data( ) );

	if ( flags & font_flags_outline )
	{
		m_background_draw_list->AddText( font, font_size, ImVec2( pos.x + 1, pos.y - 1 ), outline_clr.im( ), message.data( ) );
		m_background_draw_list->AddText( font, font_size, ImVec2( pos.x - 1, pos.y + 1 ), outline_clr.im( ), message.data( ) );
		m_background_draw_list->AddText( font, font_size, ImVec2( pos.x - 1, pos.y - 1 ), outline_clr.im( ), message.data( ) );
		m_background_draw_list->AddText( font, font_size, ImVec2( pos.x + 1, pos.y + 1 ), outline_clr.im( ), message.data( ) );
		m_background_draw_list->AddText( font, font_size, ImVec2( pos.x, pos.y + 1 ), outline_clr.im( ), message.data( ) );
		m_background_draw_list->AddText( font, font_size, ImVec2( pos.x, pos.y - 1 ), outline_clr.im( ), message.data( ) );
		m_background_draw_list->AddText( font, font_size, ImVec2( pos.x + 1, pos.y ), outline_clr.im( ), message.data( ) );
		m_background_draw_list->AddText( font, font_size, ImVec2( pos.x - 1, pos.y ), outline_clr.im( ), message.data( ) );
	}

	m_background_draw_list->AddText( font, font_size, pos, color.im( ), message.data( ) );
}