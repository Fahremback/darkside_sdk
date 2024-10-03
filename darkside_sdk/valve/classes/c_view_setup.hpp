#pragma once

#include "../../sdk/typedefs/vec_t.hpp"

class c_view_setup {
private:
    std::byte pad_0000[ 0x04D8 ]; //0x0000
public:
    float m_fov; //0x04D8
    float m_viewmodel_fov; //0x04DC
    vec3_t m_origin; //0x04E0
private:
    std::byte pad_04EC[ 0xC ]; //0x04EC
public:
    vec3_t m_angles; //0x04F8
private:
    std::byte pad_0504[ 0x14 ]; //0x0504
public:
    float m_aspect_ratio; //0x0518
};