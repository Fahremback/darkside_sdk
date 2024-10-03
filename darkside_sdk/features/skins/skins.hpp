#pragma once

#include "../../darkside.hpp"

class c_dumped_skins {
public:
    const char* m_name = "";
    int m_id = 0;
    int m_rarity = 0;
};

class c_dumped_item {
public:
    const char* m_name = "";
    uint16_t m_def_index = 0;
    int m_rarity = 0;
    bool m_unusual_item = false;
    std::vector<c_dumped_skins> m_dumped_skins{};
    c_dumped_skins* m_selected_skin = nullptr;
};

enum e_material_magic_numbers : std::uint32_t {
    MATERIAL_NUMBER_GLOVES = 0xF143B82A
};

class c_material_record {
public:
    std::uint32_t m_unknown;
    e_material_magic_numbers m_magic_number;
    std::uint32_t m_handle;
    std::uint32_t m_type;
};

class c_material_data {
public:
    c_material_record* m_records;
    std::int32_t m_records_size;
};

class c_skins {
public:
    std::vector<c_dumped_item> m_dumped_items;
    struct {
        std::vector<const char*> m_dumped_agent_models;
        std::vector<const char*> m_dumped_agent_name;
    } m_agents;

    struct {
        std::array<int, 20> m_knife_idxs = { 500, 503, 505, 506, 507, 508, 509, 512, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 525, 526 };
        std::vector<const char*> m_dumped_knife_models;
        std::vector<const char*> m_dumped_knife_name;
    } m_knives;

    struct {
        std::vector<const char*> m_dumped_glove_models;
        std::vector<const char*> m_dumped_glove_name;
        std::vector<int> m_glove_idxs;
    } m_gloves;

    void dump_items( );
    void glove_changer( c_base_view_model* view_model );
    void agent_changer( int stage );
    void knife_changer( int stage );
};

inline const auto g_skins = std::make_unique<c_skins>( );