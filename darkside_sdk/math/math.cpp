#include "../darkside.hpp"

void c_math::angle_vectors( vec3_t& angles, vec3_t& forward )
{
	float sp, sy, cp, cy;

	DirectX::XMScalarSinCos( &sp, &cp, deg2rad( angles.x ) );
	DirectX::XMScalarSinCos( &sy, &cy, deg2rad( angles.y ) );

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

vec3_t c_math::angle_vectors( const vec3_t& angles ) {
	float	sp, sy, cp, cy;

	DirectX::XMScalarSinCos( &sp, &cp, deg2rad( angles.x ) );
	DirectX::XMScalarSinCos( &sy, &cy, deg2rad( angles.y ) );

	return vec3_t(
		cp * cy,
		cp * sy,
		-sp
	);
}

void c_math::angle_vectors( vec3_t angles, vec3_t& forward, vec3_t& right, vec3_t& up )
{
	float			angle;
	static float	sr, sp, sy, cr, cp, cy;

	angle = angles.y * ( _pi2 / 360 );
	sy = sin( angle );
	cy = cos( angle );

	angle = angles.x * ( _pi2 / 360 );
	sp = sin( angle );
	cp = cos( angle );

	angle = angles.z * ( _pi2 / 360 );
	sr = sin( angle );
	cr = cos( angle );

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;

	right.x = ( -1 * sr * sp * cy + -1 * cr * -sy );
	right.y = ( -1 * sr * sp * sy + -1 * cr * cy );
	right.z = -1 * sr * cp;

	up.x = ( cr * sp * cy + -sr * -sy );
	up.y = ( cr * sp * sy + -sr * cy );
	up.z = cr * cp;
}

void c_math::vector_angles( vec3_t vec_forward, vec3_t& vec_angles )
{
	if ( vec_forward.x == 0.f && vec_forward.y == 0.f )
	{
		vec_angles.y = 0.f;
		vec_angles.x = vec_forward.z > 0.f ? 270.f : 90.f;
	}
	else
	{
		vec_angles.y = rad2deg( std::atan2( vec_forward.y, vec_forward.x ) );
		if ( vec_angles.y < 0.f )
			vec_angles.y += 360.f;

		vec_angles.x = rad2deg( std::atan2( -vec_forward.z, vec_forward.length_2d( ) ) );
		if ( vec_angles.x < 0.f )
			vec_angles.x += 360.f;
	}

	vec_angles.x = std::remainder( vec_angles.x, 360.f );
	vec_angles.y = std::remainder( vec_angles.y, 360.f );
	vec_angles.z = std::remainder( vec_angles.z, 360.f );
}

float c_math::delta_angle( float first, float second )
{
	const float delta = first - second;
	float res = std::isfinite( delta ) ? std::remainder( delta, _pi2 ) : 0.0f;

	if ( first > second ) {
		if ( res >= _pi )
			res -= _pi2;
	}
	else {
		if ( res <= _pi )
			res += _pi;
	}

	return res;
};

float c_math::angle_diff( float src, float dst ) {
	return std::remainder(
		std::remainder( dst, 360.f ) - std::remainder( src, 360.f ),
		360.f
	);
}

vec3_t c_math::aim_direction( vec3_t src, vec3_t dst )
{
	vec3_t result;

	vec3_t delta = dst - src;

	this->vector_angles( delta, result );

	return result;
}

matrix3x4_t c_math::transform_to_matrix( c_bone_data& in ) {
	matrix3x4_t  matrix;

	vec4_t rot;
	rot = in.m_rot;
	vec3_t pos;
	pos = in.m_pos;

	matrix[ 0 ][ 0 ] = 1.0f - 2.0f * rot.y * rot.y - 2.0f * rot.z * rot.z;
	matrix[ 1 ][ 0 ] = 2.0f * rot.x * rot.y + 2.0f * rot.w * rot.z;
	matrix[ 2 ][ 0 ] = 2.0f * rot.x * rot.z - 2.0f * rot.w * rot.y;

	matrix[ 0 ][ 1 ] = 2.0f * rot.x * rot.y - 2.0f * rot.w * rot.z;
	matrix[ 1 ][ 1 ] = 1.0f - 2.0f * rot.x * rot.x - 2.0f * rot.z * rot.z;
	matrix[ 2 ][ 1 ] = 2.0f * rot.y * rot.z + 2.0f * rot.w * rot.x;

	matrix[ 0 ][ 2 ] = 2.0f * rot.x * rot.z + 2.0f * rot.w * rot.y;
	matrix[ 1 ][ 2 ] = 2.0f * rot.y * rot.z - 2.0f * rot.w * rot.x;
	matrix[ 2 ][ 2 ] = 1.0f - 2.0f * rot.x * rot.x - 2.0f * rot.y * rot.y;

	matrix[ 0 ][ 3 ] = pos.x;
	matrix[ 1 ][ 3 ] = pos.y;
	matrix[ 2 ][ 3 ] = pos.z;

	return matrix;
}

void c_math::vector_transform( vec3_t& in, matrix3x4_t& matrix, vec3_t& out ) {
	out = {
		in.dot( { matrix[ 0 ][ 0 ], matrix[ 0 ][ 1 ], matrix[ 0 ][ 2 ] } ) + matrix[ 0 ][ 3 ],
		in.dot( { matrix[ 1 ][ 0 ], matrix[ 1 ][ 1 ], matrix[ 1 ][ 2 ] } ) + matrix[ 1 ][ 3 ],
		in.dot( { matrix[ 2 ][ 0 ], matrix[ 2 ][ 1 ], matrix[ 2 ][ 2 ] } ) + matrix[ 2 ][ 3 ]
	};
}