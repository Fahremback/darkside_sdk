#pragma once

class i_global_vars {
public:
    float m_real_time; //0x0000
    int m_frame_count; //0x0004
    float m_absolute_frame_time; //0x0008
    float m_absolute_frame_start_time_std_dev; //0x000C
    int m_max_clients; //0x0010
private:
    std::byte pad_0014[ 0x14 ]; //0x0014
public:
    void* m_current_time_update; //0x0028
    float m_frame_time; //0x0030
    int m_current_time; //0x0034
    int m_player_time; //0x0038
private:
    std::byte pad_003C[ 0x4 ];
    float N00000236; //0x0040
    std::byte pad_0044[ 0x4 ]; //0x0044
public:
    std::int32_t m_tick_count; //0x0048
private:
    std::byte pad_004C[ 0x8 ]; //0x004C
public:
    float m_tick_fraction; //0x0054
    void* m_net_channel; //0x0058
private:
    std::byte pad_0060[ 0x108 ]; //0x0060
public:
    void* m_network_string_table_container; //0x0168
private:
    std::byte pad_0170[ 0x40 ]; //0x0170
public:
    char* m_current_map; //0x01B0
    char* m_current_map_name; //0x01B8
};