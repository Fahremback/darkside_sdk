#pragma once
#include "../../../sdk/typedefs/vec_t.hpp"
#include "../../../sdk/vfunc/vfunc.hpp"
#include "../../../utils/utils.hpp"

#include "usercmd.pb.hpp"
#include "cs_usercmd.pb.hpp"
#include "network_connection.pb.hpp"
#include "networkbasetypes.pb.hpp"

#define	FL_ONGROUND				(1 << 0)
#define FL_DUCKING				(1 << 1)
#define	FL_WATERJUMP			(1 << 3)
#define FL_ONTRAIN				(1 << 4)
#define FL_INRAIN				(1 << 5)
#define FL_FROZEN				(1 << 6)
#define FL_ATCONTROLS			(1 << 7)
#define	FL_CLIENT				(1 << 8)
#define FL_FAKECLIENT			(1 << 9)
#define	FL_INWATER				(1 << 10)
#define FL_HIDEHUD_SCOPE		(1 << 11)

enum e_button
{
    IN_NONE = 0,
    IN_ALL = -1,
    IN_ATTACK = 1,
    IN_JUMP = 2,
    IN_DUCK = 4,
    IN_FORWARD = 8,
    IN_BACK = 16,
    IN_USE = 32,
    IN_TURNLEFT = 128,
    IN_TURNRIGHT = 256,
    IN_MOVELEFT = 512,
    IN_MOVERIGHT = 1024,
    IN_ATTACK2 = 2048,
    IN_RELOAD = 8192,
    IN_SPEED = 65536,
    IN_JOYAUTOSPRINT = 131072,
    IN_FIRST_MOD_SPECIFIC_BIT = 4294967296,
    IN_USEORRELOAD = 4294967296,
    IN_SCORE = 8589934592,
    IN_ZOOM = 17179869184,
    IN_JUMP_THROW_RELEASE = 34359738368,
};

class c_in_button_state
{
public:
    std::byte pad_0000[ 8 ]; //0x0000
    uint64_t m_button_state; //0x0008
    uint64_t m_button_state2; //0x0010
    uint64_t m_button_state3; //0x0018
}; //Size: 0x0020
static_assert( sizeof( c_in_button_state ) == 0x20 );

class c_user_cmd
{
public:
    std::byte pad_valve_shizo[ 0x10 ];
    c_csgo_user_cmd_pb pb;
    c_in_button_state m_button_state; //0x0040
private:
    std::byte pad_0068[ 0x8 ]; //0x0068
public:
    uint32_t random_seed; //0x0070
    float current_time;//0x0074
private:
    std::byte pad_78[ 16 ];//0x0078
};

class c_subitck_moves
{
public:
    float m_when; //0x0000
    float m_delta; //0x0004
    uint64_t m_button; //0x0008
    bool m_pressed; //0x0010
    std::byte pad_0011[ 7 ]; //0x0011
}; //Size: 0x0018

class c_cs_input_moves
{
public:
    uint64_t m_current_bits; //0x0000
    std::byte pad_0008[ 24 ]; //0x0008
    float m_forward_move; //0x0020
    float m_side_move; //0x0024
    std::byte pad_0028[ 16 ]; //0x0028
    float m_fraction; //0x0038
    std::byte pad_003C[ 4 ]; //0x003C
    int32_t m_last_buttons; //0x0040
    std::byte pad_0044[ 4 ]; //0x0044
    bool m_in_move; //0x0048
    std::byte pad_0074[ 255 ]; //0x0074
    vec3_t m_view_angles; //0x0158
    std::byte pad_0164[ 48 ]; //0x0164
}; //Size: 0x0194

class i_csgo_input
{
public:
    std::byte pad_0000[ 592 ]; //0x0000
    c_user_cmd m_commands[ 150 ]; //0x0250
    c_user_cmd m_global_command; //0x5B60
    bool m_block_shot; //0x5BF8
    bool m_in_third_person; //0x5BF9
    std::byte pad_5BFA[ 6 ]; //0x5BFA
    vec3_t m_third_person_angles; //0x5C00
    std::byte pad_5C0C[ 16 ]; //0x529C
    int32_t m_sequence_number; //0x5C1C
    float m_real_time; //0x5CD0

    vec3_t get_view_angles( ) {
        using get_view_angles_t = void* ( __fastcall* )( i_csgo_input*, int );
        static get_view_angles_t get_view_angles = reinterpret_cast<get_view_angles_t>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "4C 8B C1 85 D2 74 08 48 8D 05 ? ? ? ? C3" ) ); 

        return *reinterpret_cast<vec3_t*>( get_view_angles( this, 0 ) );
    }

    void set_view_angles( vec3_t& view_angles ) {
        using set_view_angles_t = void ( __fastcall* )( i_csgo_input*, int, vec3_t& );
        static set_view_angles_t set_view_angles = reinterpret_cast<set_view_angles_t>( g_opcodes->scan( g_modules->m_modules.client_dll.get_name( ), "85 D2 75 3F 48" ) );

        set_view_angles( this, 0, view_angles );
    }

    c_user_cmd* get_user_cmd( ) {
        return &m_commands[ m_sequence_number % 150 ];
    }

    c_user_cmd* get_next_user_cnd( ) {
        return &m_commands[ ( m_sequence_number++ ) % 150 ];
    }
};