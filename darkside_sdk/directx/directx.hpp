#pragma once

#include "../darkside.hpp"

#include "../render/render.hpp"

class c_directx {
	IDXGISwapChain* m_swap_chain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_device_context;
	ID3D11RenderTargetView* m_render_target;
public:
	void initialize( );
	void unitialize( );
	void start_frame( IDXGISwapChain* swap_chain );
	void new_frame( );
	void end_frame( );

	void* m_present_address;
	WNDPROC m_window_proc_original = 0;
	HWND m_window;
};

inline const auto g_directx = std::make_unique<c_directx>( );