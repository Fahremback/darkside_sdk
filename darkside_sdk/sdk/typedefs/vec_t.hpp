#pragma once

#include "../includes/imgui/imgui.h"

#include <cmath>
#include <algorithm>

constexpr auto _pi = 3.14159265358979323846;

constexpr auto _pi2 = _pi * 2;

constexpr auto _rad_pi = 180 / _pi;

constexpr auto _deg_pi = _pi / 180;

// convert angle in degrees to radians
#define deg2rad(degrees) (degrees * _deg_pi)
// convert angle in radians to degrees
#define rad2deg(radians) (radians * _rad_pi)

class vec4_t {
public:
	float x, y, z, w;

	vec4_t( float _x = 0, float _y = 0, float _z = 0, float _w = 0 ) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	vec4_t& operator+=( const vec4_t& v )
	{
		x += v.x; y += v.y; z += v.z; w += v.w;
		return *this;
	}

	vec4_t& operator-=( const vec4_t& v )
	{
		x -= v.x; y -= v.y; z -= v.z; w -= v.w;
		return *this;
	}

	vec4_t& operator*=( float fl )
	{
		x *= fl;
		y *= fl;
		z *= fl;
		w *= fl;
		return *this;
	}

	vec4_t& operator*=( const vec4_t& v )
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}

	vec4_t& operator/=( const vec4_t& v )
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}

	vec4_t& operator+=( float fl )
	{
		x += fl;
		y += fl;
		z += fl;
		w += fl;
		return *this;
	}

	vec4_t& operator/=( float fl )
	{
		x /= fl;
		y /= fl;
		z /= fl;
		w /= fl;
		return *this;
	}
	vec4_t& operator-=( float fl )
	{
		x -= fl;
		y -= fl;
		z -= fl;
		w -= fl;
		return *this;
	}
};

class vec2_t {
public:
	float x, y;

	vec2_t( float _x = 0, float _y = 0, float _z = 0 ) {
		x = _x;
		y = _y;
	}

	vec2_t operator+( vec2_t vec ) {
		return vec2_t( x + vec.x, y + vec.y );
	}

	vec2_t operator+( float n ) {
		return vec2_t( x + n, y + n );
	}

	vec2_t operator+=( vec2_t vec ) {
		x += vec.x;
		y += vec.y;

		return *this;
	}

	vec2_t operator+=( float n ) {
		x += n;
		y += n;

		return *this;
	}

	vec2_t operator-( vec2_t vec ) {
		return vec2_t( x - vec.x, y - vec.y );
	}

	vec2_t operator-( float n ) {
		return vec2_t( x - n, y - n );
	}

	vec2_t operator-=( vec2_t vec ) {
		x -= vec.x;
		y -= vec.y;

		return *this;
	}

	vec2_t operator-=( float n ) {
		x -= n;
		y -= n;

		return *this;
	}

	vec2_t operator/( vec2_t vec ) {
		return vec2_t( x / vec.x, y / vec.y );
	}

	vec2_t operator/( float n ) {
		return vec2_t( x / n, y / n );
	}

	vec2_t operator/=( vec2_t vec ) {
		x /= vec.x;
		y /= vec.y;

		return *this;
	}

	vec2_t operator/=( float n ) {
		x /= n;
		y /= n;

		return *this;
	}

	vec2_t operator*( vec2_t vec ) {
		return vec2_t( x * vec.x, y * vec.y );
	}

	vec2_t operator*( float n ) {
		return vec2_t( x * n, y * n );
	}

	vec2_t operator*=( vec2_t vec ) {
		x *= vec.x;
		y *= vec.y;

		return *this;
	}

	vec2_t operator*=( float n ) {
		x *= n;
		y *= n;

		return *this;
	}

	bool operator==( vec2_t vec ) {
		return x == vec.x && y == vec.y;
	}

	bool operator!=( vec2_t vec ) {
		return !( *this == vec );
	}
};

class matrix3x4_t;

class vec3_t {
public:
	float x, y, z;

	vec3_t( float _x = 0, float _y = 0, float _z = 0 ) {
		x = _x;
		y = _y;
		z = _z;
	}

	vec3_t operator+( vec3_t& vec ) {
		return vec3_t( x + vec.x, y + vec.y, z + vec.z );
	}

	vec3_t operator+( const vec3_t& vec ) {
		return vec3_t( x + vec.x, y + vec.y, z + vec.z );
	}

	vec3_t operator+( const vec3_t& vec ) const {
		return vec3_t( x + vec.x, y + vec.y, z + vec.z );
	}

	vec3_t operator+( float n ) {
		return vec3_t( x + n, y + n, z + n );
	}

	vec3_t operator+=( vec3_t vec ) {
		x += vec.x;
		y += vec.y;
		z += vec.z;

		return *this;
	}

	vec3_t operator+=( float n ) {
		x += n;
		y += n;
		z += n;

		return *this;
	}

	vec3_t operator-( vec3_t vec ) {
		return vec3_t( x - vec.x, y - vec.y, z - vec.z );
	}

	vec3_t operator-( float n ) {
		return vec3_t( x - n, y - n, z - n );
	}

	vec3_t operator-=( vec3_t vec ) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;

		return *this;
	}

	vec3_t operator-=( float n ) {
		x -= n;
		y -= n;
		z -= n;

		return *this;
	}

	vec3_t operator/( vec3_t vec ) {
		return vec3_t( x / vec.x, y / vec.y, z / vec.z );
	}

	vec3_t operator/( float n ) {
		return vec3_t( x / n, y / n, z / n );
	}

	vec3_t operator/( int n ) {
		return vec3_t( x / n, y / n, z / n );
	}

	vec3_t operator/=( vec3_t vec ) {
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;

		return *this;
	}

	vec3_t operator/=( float n ) {
		x /= n;
		y /= n;
		z /= n;

		return *this;
	}

	vec3_t operator*( vec3_t vec ) {
		return vec3_t( x * vec.x, y * vec.y, z * vec.z );
	}

	vec3_t operator*( float n ) {
		return vec3_t( x * n, y * n, z * n );
	}

	vec3_t operator*=( vec3_t vec ) {
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;

		return *this;
	}

	vec3_t operator*=( float n ) {
		x *= n;
		y *= n;
		z *= n;

		return *this;
	}

	bool operator==( vec3_t vec ) {
		return x == vec.x && y == vec.y && z == vec.z;
	}

	bool operator!=( vec3_t vec ) {
		return !( *this == vec );
	}

	void clamp( float minVal, float maxVal );

	vec3_t& clamp( );

	ImVec2 im( );

	float dot( float* v );

	float dot( vec3_t v, bool additional = false );

	void vector_cross_product( vec3_t a, vec3_t b, vec3_t& result );

	vec3_t cross( vec3_t other );

	vec3_t transform( matrix3x4_t in );

	float normalize_in_place( );

	bool is_zero( );

	vec3_t angles( );

	vec3_t angles( float pitch, float yaw );

	vec3_t angles( vec3_t vec );

	vec3_t to( vec3_t vec );

	float dist( vec3_t vec );

	float dist_sqr( vec3_t vec );

	float dist_2d( vec3_t vec );

	float dist_2d_sqr( vec3_t vec );

	float length( );

	float length_sqr( );

	float length_2d( );

	float length_2d_sqr( );

	vec3_t normalize( );

	float normalize_movement( );

	vec3_t normalize_angle( );

	float fov( vec3_t vec );
};

struct alignas( 16 ) vec_aligned : vec3_t
{
	vec_aligned( ) = default;

	explicit vec_aligned( const vec3_t& vecBase )
	{
		this->x = vecBase.x;
		this->y = vecBase.y;
		this->z = vecBase.z;
		this->w = 0.0f;
	}

	constexpr vec_aligned& operator=( const vec3_t& vecBase )
	{
		this->x = vecBase.x;
		this->y = vecBase.y;
		this->z = vecBase.z;
		this->w = 0.0f;
		return *this;
	}

	float w = 0.0f;
};


#pragma pack(push, 4)
using matrix3x3_t = float[ 3 ][ 3 ];

class matrix3x4_t {
public:
	matrix3x4_t( ) = default;

	constexpr matrix3x4_t(
		const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13,
		const float m20, const float m21, const float m22, const float m23 )
	{
		arr_data[ 0 ][ 0 ] = m00;
		arr_data[ 0 ][ 1 ] = m01;
		arr_data[ 0 ][ 2 ] = m02;
		arr_data[ 0 ][ 3 ] = m03;
		arr_data[ 1 ][ 0 ] = m10;
		arr_data[ 1 ][ 1 ] = m11;
		arr_data[ 1 ][ 2 ] = m12;
		arr_data[ 1 ][ 3 ] = m13;
		arr_data[ 2 ][ 0 ] = m20;
		arr_data[ 2 ][ 1 ] = m21;
		arr_data[ 2 ][ 2 ] = m22;
		arr_data[ 2 ][ 3 ] = m23;
	}

	constexpr matrix3x4_t( const vec3_t& vecForward, const vec3_t& vecLeft, const vec3_t& vecUp, const vec3_t& vecOrigin )
	{
		set_forward( vecForward );
		set_left( vecLeft );
		set_up( vecUp );
		set_origin( vecOrigin );
	}

	float* operator[]( const int nIndex )
	{
		return arr_data[ nIndex ];
	}

	const float* operator[]( const int nIndex ) const
	{
		return arr_data[ nIndex ];
	}

	constexpr void set_forward( const vec3_t& vecForward )
	{
		arr_data[ 0 ][ 0 ] = vecForward.x;
		arr_data[ 1 ][ 0 ] = vecForward.y;
		arr_data[ 2 ][ 0 ] = vecForward.z;
	}

	constexpr void set_left( const vec3_t& vecLeft )
	{
		arr_data[ 0 ][ 1 ] = vecLeft.x;
		arr_data[ 1 ][ 1 ] = vecLeft.y;
		arr_data[ 2 ][ 1 ] = vecLeft.z;
	}

	constexpr void set_up( const vec3_t& vecUp )
	{
		arr_data[ 0 ][ 2 ] = vecUp.x;
		arr_data[ 1 ][ 2 ] = vecUp.y;
		arr_data[ 2 ][ 2 ] = vecUp.z;
	}

	constexpr void set_origin( const vec3_t& vecOrigin )
	{
		arr_data[ 0 ][ 3 ] = vecOrigin.x;
		arr_data[ 1 ][ 3 ] = vecOrigin.y;
		arr_data[ 2 ][ 3 ] = vecOrigin.z;
	}

	vec3_t get_forward( ) const
	{
		return { arr_data[ 0 ][ 0 ], arr_data[ 1 ][ 0 ], arr_data[ 2 ][ 0 ] };
	}

	vec3_t get_left( ) const
	{
		return { arr_data[ 0 ][ 1 ], arr_data[ 1 ][ 1 ], arr_data[ 2 ][ 1 ] };
	}

	vec3_t get_up( ) const
	{
		return { arr_data[ 0 ][ 2 ], arr_data[ 1 ][ 2 ], arr_data[ 2 ][ 2 ] };
	}

	vec3_t get_origin( ) const
	{
		return { arr_data[ 0 ][ 3 ], arr_data[ 1 ][ 3 ], arr_data[ 2 ][ 3 ] };
	}

	constexpr matrix3x4_t concat_transforms( const matrix3x4_t& mat_other ) const
	{
		return {
			arr_data[ 0 ][ 0 ] * mat_other.arr_data[ 0 ][ 0 ] + arr_data[ 0 ][ 1 ] * mat_other.arr_data[ 1 ][ 0 ] + arr_data[ 0 ][ 2 ] * mat_other.arr_data[ 2 ][ 0 ],
			arr_data[ 0 ][ 0 ] * mat_other.arr_data[ 0 ][ 1 ] + arr_data[ 0 ][ 1 ] * mat_other.arr_data[ 1 ][ 1 ] + arr_data[ 0 ][ 2 ] * mat_other.arr_data[ 2 ][ 1 ],
			arr_data[ 0 ][ 0 ] * mat_other.arr_data[ 0 ][ 2 ] + arr_data[ 0 ][ 1 ] * mat_other.arr_data[ 1 ][ 2 ] + arr_data[ 0 ][ 2 ] * mat_other.arr_data[ 2 ][ 2 ],
			arr_data[ 0 ][ 0 ] * mat_other.arr_data[ 0 ][ 3 ] + arr_data[ 0 ][ 1 ] * mat_other.arr_data[ 1 ][ 3 ] + arr_data[ 0 ][ 2 ] * mat_other.arr_data[ 2 ][ 3 ] + arr_data[ 0 ][ 3 ],

			arr_data[ 1 ][ 0 ] * mat_other.arr_data[ 0 ][ 0 ] + arr_data[ 1 ][ 1 ] * mat_other.arr_data[ 1 ][ 0 ] + arr_data[ 1 ][ 2 ] * mat_other.arr_data[ 2 ][ 0 ],
			arr_data[ 1 ][ 0 ] * mat_other.arr_data[ 0 ][ 1 ] + arr_data[ 1 ][ 1 ] * mat_other.arr_data[ 1 ][ 1 ] + arr_data[ 1 ][ 2 ] * mat_other.arr_data[ 2 ][ 1 ],
			arr_data[ 1 ][ 0 ] * mat_other.arr_data[ 0 ][ 2 ] + arr_data[ 1 ][ 1 ] * mat_other.arr_data[ 1 ][ 2 ] + arr_data[ 1 ][ 2 ] * mat_other.arr_data[ 2 ][ 2 ],
			arr_data[ 1 ][ 0 ] * mat_other.arr_data[ 0 ][ 3 ] + arr_data[ 1 ][ 1 ] * mat_other.arr_data[ 1 ][ 3 ] + arr_data[ 1 ][ 2 ] * mat_other.arr_data[ 2 ][ 3 ] + arr_data[ 1 ][ 3 ],

			arr_data[ 2 ][ 0 ] * mat_other.arr_data[ 0 ][ 0 ] + arr_data[ 2 ][ 1 ] * mat_other.arr_data[ 1 ][ 0 ] + arr_data[ 2 ][ 2 ] * mat_other.arr_data[ 2 ][ 0 ],
			arr_data[ 2 ][ 0 ] * mat_other.arr_data[ 0 ][ 1 ] + arr_data[ 2 ][ 1 ] * mat_other.arr_data[ 1 ][ 1 ] + arr_data[ 2 ][ 2 ] * mat_other.arr_data[ 2 ][ 1 ],
			arr_data[ 2 ][ 0 ] * mat_other.arr_data[ 0 ][ 2 ] + arr_data[ 2 ][ 1 ] * mat_other.arr_data[ 1 ][ 2 ] + arr_data[ 2 ][ 2 ] * mat_other.arr_data[ 2 ][ 2 ],
			arr_data[ 2 ][ 0 ] * mat_other.arr_data[ 0 ][ 3 ] + arr_data[ 2 ][ 1 ] * mat_other.arr_data[ 1 ][ 3 ] + arr_data[ 2 ][ 2 ] * mat_other.arr_data[ 2 ][ 3 ] + arr_data[ 2 ][ 3 ]
		};
	}

	float arr_data[ 3 ][ 4 ] = {};
};

#pragma pack(pop)

struct quaternion_t {
	constexpr quaternion_t( const float x = 0.0f, const float y = 0.0f, const float z = 0.0f, const float w = 0.0f ) :
		x( x ), y( y ), z( z ), w( w ) { }

	bool is_valid( ) const
	{
		return ( std::isfinite( x ) && std::isfinite( y ) && std::isfinite( z ) && std::isfinite( w ) );
	}

	matrix3x4_t to_matrix( const vec3_t& vec_origin = {} ) const
	{
		matrix3x4_t mat_out;

#ifdef _DEBUG // precalculate common multiplications
		const float x2 = this->x + this->x, y2 = this->y + this->y, z2 = this->z + this->z;
		const float xx = this->x * x2, xy = this->x * y2, xz = this->x * z2;
		const float yy = this->y * y2, yz = this->y * z2;
		const float zz = this->z * z2;
		const float wx = this->w * x2, wy = this->w * y2, wz = this->w * z2;

		mat_out[ 0 ][ 0 ] = 1.0f - ( yy + zz );
		mat_out[ 1 ][ 0 ] = xy + wz;
		mat_out[ 2 ][ 0 ] = xz - wy;

		mat_out[ 0 ][ 1 ] = xy - wz;
		mat_out[ 1 ][ 1 ] = 1.0f - ( xx + zz );
		mat_out[ 2 ][ 1 ] = yz + wx;

		mat_out[ 0 ][ 2 ] = xz + wy;
		mat_out[ 1 ][ 2 ] = yz - wx;
		mat_out[ 2 ][ 2 ] = 1.0f - ( xx + yy );
#else // let the compiler optimize calculations itself
		mat_out[ 0 ][ 0 ] = 1.0f - 2.0f * this->y * this->y - 2.0f * this->z * this->z;
		mat_out[ 1 ][ 0 ] = 2.0f * this->x * this->y + 2.0f * this->w * this->z;
		mat_out[ 2 ][ 0 ] = 2.0f * this->x * this->z - 2.0f * this->w * this->y;

		mat_out[ 0 ][ 1 ] = 2.0f * this->x * this->y - 2.0f * this->w * this->z;
		mat_out[ 1 ][ 1 ] = 1.0f - 2.0f * this->x * this->x - 2.0f * this->z * this->z;
		mat_out[ 2 ][ 1 ] = 2.0f * this->y * this->z + 2.0f * this->w * this->x;

		mat_out[ 0 ][ 2 ] = 2.0f * this->x * this->z + 2.0f * this->w * this->y;
		mat_out[ 1 ][ 2 ] = 2.0f * this->y * this->z - 2.0f * this->w * this->x;
		mat_out[ 2 ][ 2 ] = 1.0f - 2.0f * this->x * this->x - 2.0f * this->y * this->y;
#endif

		mat_out[ 0 ][ 3 ] = vec_origin.x;
		mat_out[ 1 ][ 3 ] = vec_origin.y;
		mat_out[ 2 ][ 3 ] = vec_origin.z;
		return mat_out;
	}

	float x, y, z, w;
};

struct alignas( 16 ) quaternion_aligned : quaternion_t
{
	quaternion_aligned& operator=( const quaternion_t& quat_other )
	{
		this->x = quat_other.x;
		this->y = quat_other.y;
		this->z = quat_other.z;
		this->w = quat_other.w;
		return *this;
	}
};