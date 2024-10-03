#include "directx.hpp"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

LRESULT hk_wnd_proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	ImGui_ImplWin32_WndProcHandler( hWnd, uMsg, wParam, lParam );

	if ( uMsg == WM_KEYUP && wParam == VK_INSERT )
		g_menu->m_opened = !g_menu->m_opened;

	static auto original = hooks::enable_cursor::m_enable_cursor.get_original<decltype( &hooks::enable_cursor::hk_enable_cursor )>( );
	original( g_interfaces->m_input_system, g_menu->m_opened ? false : hooks::enable_cursor::m_enable_cursor_input );

	return CallWindowProc( g_directx->m_window_proc_original, hWnd, uMsg, wParam, lParam );
}

void c_directx::initialize( ) {
	uint8_t* ptr = g_opcodes->scan_relative( g_modules->m_modules.rendersystem_dll.get_name( ), "66 0F 7F 0D ? ? ? ? 66 0F 7F 05 ? ? ? ? 0F 1F 40 00", 0x0, 0x4, 0x8 );

	if ( !ptr )
		return;

	uint8_t* next_ptr = **reinterpret_cast<uint8_t***>( ptr );
	if ( !next_ptr )
		return;

	m_swap_chain = *reinterpret_cast<IDXGISwapChain**>( next_ptr + 0x170 );
	if ( !m_swap_chain )
		return;

	m_present_address = vmt::get_v_method( m_swap_chain, 8 );
}

void c_directx::unitialize( ) {
	if ( !m_window_proc_original )
		return;

	SetWindowLongPtr( m_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( m_window_proc_original ) );
}

std::once_flag init_flag;

void c_directx::start_frame( IDXGISwapChain* swap_chain ) {
	ID3D11Texture2D* back_buffer;
	DXGI_SWAP_CHAIN_DESC desc;
	swap_chain->GetDesc( &desc );

	m_window = desc.OutputWindow;

	std::call_once( init_flag, [&]( )
	{
		swap_chain->GetDevice( __uuidof( ID3D11Device ), reinterpret_cast<void**>( &m_device ) );
		m_device->GetImmediateContext( &m_device_context );
		swap_chain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &back_buffer ) );

		m_device->CreateRenderTargetView( back_buffer, nullptr, &m_render_target );

		if ( back_buffer )
			back_buffer->Release( );

		m_window_proc_original = reinterpret_cast<WNDPROC>( SetWindowLongPtr( m_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( hk_wnd_proc ) ) );

		ImGui::CreateContext( );
		ImGui_ImplWin32_Init( m_window );
		ImGui_ImplDX11_Init( m_device, m_device_context );

		g_render->initialize( );
	} );
}

void c_directx::new_frame( ) {
	ImGuiIO io = ImGui::GetIO( );

	g_render->update_screen_size( io );

	ImGui_ImplDX11_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
	ImGui::NewFrame( );
}

void c_directx::end_frame( ) {
	ImGui::Render( );

	m_device_context->OMSetRenderTargets( 1, &m_render_target, nullptr );

	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );
}