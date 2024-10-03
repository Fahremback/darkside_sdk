#include "vec_t.hpp"

void vec3_t::clamp( float minVal, float maxVal ) {
	x = ( x < minVal ) ? minVal : ( x > maxVal ) ? maxVal : x;
	y = ( y < minVal ) ? minVal : ( y > maxVal ) ? maxVal : y;
	z = ( z < minVal ) ? minVal : ( z > maxVal ) ? maxVal : z;
}

vec3_t& vec3_t::clamp( ) {
	x = std::clamp( x, -89.f, 89.f );
	y = std::clamp( std::remainder( y, 360.0f ), -180.f, 180.f );
	z = 0.f;

	return *this;
}

ImVec2 vec3_t::im( )
{
	// round up pixel positions
	// cuz in subpixels render fucks up and break bbox or just have "jiggly" positions
	// @MIRACLE - AURA
	return ImVec2( static_cast<int>( x ), static_cast<int>( y ) );
}

float vec3_t::dot( float* v )
{
	return this->x * v[ 0 ] + this->y * v[ 1 ] + this->z * v[ 2 ];
}

float vec3_t::dot( vec3_t v, bool additional )
{
	if ( additional )
		return this->x * v.y + this->y * v.x + this->z * v.z;

	return this->x * v.x + this->y * v.y + this->z * v.z;
}

void vec3_t::vector_cross_product( vec3_t a, vec3_t b, vec3_t& result )
{
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
}

vec3_t vec3_t::cross( vec3_t other )
{
	vec3_t res;
	vector_cross_product( *this, other, res );

	return res;
}

vec3_t vec3_t::transform( matrix3x4_t in )
{
	return { dot( in[ 0 ] ) + in[ 0 ][ 3 ], dot( in[ 1 ] ) + in[ 1 ][ 3 ], dot( in[ 2 ] ) + in[ 2 ][ 3 ] };
}

float vec3_t::normalize_in_place( )
{
	const float length = this->length( );
	const float radius = 1.0f / ( length + FLT_EPSILON );

	x *= radius;
	y *= radius;
	z *= radius;

	return length;
}

bool vec3_t::is_zero( ) {
	return ( x == 0.f && y == 0.f && z == 0.f );
}

vec3_t vec3_t::angles( ) {
	float distance2D = this->length_2d( );

	float pitch = rad2deg( std::atan2f( z, distance2D ) );
	float yaw = rad2deg( std::atan2f( y, x ) );

	return vec3_t( -pitch, yaw, 0 );
}

vec3_t vec3_t::angles( float pitch, float yaw ) {
	float pitchRadians = deg2rad( pitch );
	float yawRadians = deg2rad( yaw );

	float x = std::cosf( pitchRadians ) * std::cosf( yawRadians );
	float y = std::cosf( pitchRadians ) * std::sinf( yawRadians );
	float z = -std::sinf( pitchRadians );

	return vec3_t( x, y, z );
}

vec3_t vec3_t::angles( vec3_t vec ) {
	return this->angles( vec.x, vec.y );
}

vec3_t vec3_t::to( vec3_t vec ) {
	vec3_t direction = vec - *this;

	return direction.normalize( );
}

float vec3_t::dist( vec3_t vec ) {
	return ( *this - vec ).length( );
}

float vec3_t::dist_sqr( vec3_t vec ) {
	return ( *this - vec ).length_sqr( );
}

float vec3_t::dist_2d( vec3_t vec ) {
	return ( *this - vec ).length_2d( );
}

float vec3_t::dist_2d_sqr( vec3_t vec ) {
	return ( *this - vec ).length_2d_sqr( );
}

float vec3_t::length( ) {
	return std::sqrtf( this->length_sqr( ) );
}

float vec3_t::length_sqr( ) {
	return x * x + y * y + z * z;
}

float vec3_t::length_2d( ) {
	return std::sqrtf( this->length_2d_sqr( ) );
}

float vec3_t::length_2d_sqr( ) {
	return x * x + y * y;
}

vec3_t vec3_t::normalize( ) {
	float length = this->length( );

	return *this / length;
}

float vec3_t::normalize_movement( )
{
	vec3_t res = *this;
	float l = res.length( );
	if ( l != 0.0f )
	{
		res /= l;
	}
	else
	{
		res.x = res.y = res.z = 0.0f;
	}
	return l;
}

vec3_t vec3_t::normalize_angle( ) {
	while ( x < -180.0f )
		x += 360.0f;
	while ( x > 180.0f )
		x -= 360.0f;

	while ( y < -180.0f )
		y += 360.0f;
	while ( y > 180.0f )
		y -= 360.0f;

	while ( z < -180.0f )
		z += 360.0f;
	while ( z > 180.0f )
		z -= 360.0f;

	return *this;
}

float vec3_t::fov( vec3_t vec ) {
	return ( *this - vec ).normalize_angle( ).length( );
}