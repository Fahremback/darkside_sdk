#pragma once

#include "../sdk/typedefs/vec_t.hpp"
#include "../valve/classes/c_cs_player_pawn.hpp"

class c_math {
public:
	void sin_cos( const float rad, float& sin, float& cos ) {
		sin = std::sin( rad );
		cos = std::cos( rad );
	}

	float normalize_yaw( float yaw ) {
		while ( yaw > 180.0f )
			yaw -= 360.0f;

		while ( yaw < -180.0f )
			yaw += 360.0f;

		return yaw;
	}

	void angle_vectors( vec3_t& angles, vec3_t& forward );
	vec3_t angle_vectors( const vec3_t& angles );
	void angle_vectors( vec3_t angles, vec3_t& forward, vec3_t& right, vec3_t& up );
	void vector_angles( vec3_t vec_forward, vec3_t& vec_angles );
	float delta_angle( float first, float second );
	float angle_diff( float src, float dst );
	vec3_t aim_direction( vec3_t src, vec3_t dst );
	matrix3x4_t transform_to_matrix( c_bone_data& in );
	void vector_transform( vec3_t& in, matrix3x4_t& matrix, vec3_t& out );
};

inline const auto g_math = std::make_unique<c_math>( );