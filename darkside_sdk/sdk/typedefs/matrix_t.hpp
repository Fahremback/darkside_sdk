#pragma once

#include "vec_t.hpp"

class matrix2x4_t {
public:
	vec3_t get_origin( ) {
		return vec3_t( _11, _12, _13 );
	}

	void set_origin( int nIndex, vec3_t vecValue ) {
		this[ nIndex ]._11 = vecValue.x;
		this[ nIndex ]._12 = vecValue.y;
		this[ nIndex ]._13 = vecValue.z;
	}

	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
		};
	};
};