#pragma once

#include <Windows.h>
#include <cstdint>
#include <cstdio>
#include <intrin.h>
#include <xmmintrin.h>
#include <array>
#include <vector>
#include <algorithm>
#include <cctype>
#include <string>
#include <chrono>
#include <thread>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <deque>
#include <functional>
#include <map>
#include <shlobj.h>
#include <filesystem>
#include <streambuf>
#include <string>
#include <algorithm>
#include <mutex>
#include <filesystem>
#include <intrin.h>
#include <strsafe.h>
#include <Psapi.h>
#include <thread>
#include <dxgi.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <xmmintrin.h>

#ifdef _DEBUG
#define WINCALL(func) func
#else
#define WINCALL(func) LI_FN(func).cached()
#endif

#include "sdk/includes/lazy_importer.hpp"
#include "sdk/includes/xor.hpp"
#include "sdk/includes/fnv1a.hpp"

#include "sdk/includes/minhook/MinHook.h"

#include "sdk/console/console.hpp"

#include "sdk/typedefs/vec_t.hpp"

#include "sdk/typedefs/matrix_t.hpp"

#include "math/math.hpp"

#include "utils/key_handler.hpp"

#include"utils/utils.hpp"

#include "sdk/config_system/config_system.hpp"

#include "menu/menu.hpp"

#include "sdk/includes/imgui/imgui.h"
#include "sdk/includes/imgui/imgui_impl_dx11.h"
#include "sdk/includes/imgui/imgui_impl_win32.h"

#include "valve/modules/modules.hpp"
#include "valve/classes/c_cs_player_pawn.hpp"
#include "valve/classes/c_cs_player_controller.hpp"

#include "hooks/hooks.hpp"

struct globals_t {
	c_user_cmd* m_user_cmd;
	c_cs_player_pawn* m_local_pawn;
	c_cs_player_controller* m_local_controller;
};
inline const auto g_ctx = std::make_unique<globals_t>( );