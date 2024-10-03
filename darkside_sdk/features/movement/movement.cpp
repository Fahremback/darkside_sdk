#include "movement.hpp"

void c_movement::bunnyhop( c_user_cmd* user_cmd )
{
	if ( !g_cfg->misc.m_bunny_hop )
		return;

	if ( g_ctx->m_local_pawn->m_flags( ) & FL_ONGROUND )
		user_cmd->m_button_state.m_button_state &= ~IN_JUMP;
}

void c_movement::auto_strafe( c_user_cmd* user_cmd, float old_yaw )
{
	if ( !g_cfg->misc.m_auto_strafe )
		return;

	if ( g_ctx->m_local_pawn->m_flags( ) & FL_ONGROUND )
		return;

	if ( !user_cmd || !user_cmd->pb.mutable_base()->mutable_viewangles( ) )
		return;

	vec3_t velocity = g_ctx->m_local_pawn->m_vec_abs_velocity( );
	float speed_2d = velocity.length_2d( );
	if ( speed_2d < 2.f && !user_cmd->pb.mutable_base( )->forwardmove( ) && !user_cmd->pb.mutable_base( )->leftmove( ) )
		return;

	float ideal_rotation = min( rad2deg( std::asinf( 15.f / speed_2d ) ), 90.f );
	float sign = user_cmd->pb.mutable_base( )->command_number( ) % 2 ? 1.f : -1.f;

	bool move_forward = user_cmd->m_button_state.m_button_state & IN_FORWARD, move_backward = user_cmd->m_button_state.m_button_state & IN_BACK;
	bool move_left = user_cmd->m_button_state.m_button_state & IN_MOVELEFT, move_right = user_cmd->m_button_state.m_button_state & IN_MOVERIGHT;

	user_cmd->pb.mutable_base( )->set_forwardmove( speed_2d > 0.1f ? 0.f : 1.f );

	vec3_t movement_angle { 
		user_cmd->pb.mutable_base( )->viewangles( ).x( ), 
		user_cmd->pb.mutable_base( )->viewangles( ).y( ), 
		user_cmd->pb.mutable_base( )->viewangles( ).z( ) 
	};

	if ( move_forward )
		movement_angle.y += move_left ? 45.f : move_right ? -45.f : 0.f;
	else if ( move_backward )
		movement_angle.y += move_left ? 135.f : move_right ? -135.f : 180.f;
	else if ( move_left || move_right )
		movement_angle.y += move_left ? 90.f : -90.f;

	float yaw_delta = std::remainder( movement_angle.y - old_yaw, 360.f ), abs_yaw_delta = std::abs( yaw_delta );
	//old_yaw = movement_angle.y;

	if ( yaw_delta > 0.f )
		user_cmd->pb.mutable_base( )->set_leftmove( -1.f );
	else if ( yaw_delta < 0.f )
		user_cmd->pb.mutable_base( )->set_leftmove( 1.f );

	if ( abs_yaw_delta <= ideal_rotation || abs_yaw_delta >= 30.f ) {
		float vel_ang = rad2deg( std::atan2( velocity.y, velocity.x ) );
		float vel_delta = std::remainder( movement_angle.y - vel_ang, 360.f );

		float retrack_speed = ideal_rotation * ( ( g_cfg->misc.m_strafe_smooth / 100.f ) * 3 );

		if ( vel_delta <= retrack_speed || speed_2d <= 15.f ) {
			if ( -retrack_speed <= vel_delta || speed_2d <= 15.f ) {
				movement_angle.y += ideal_rotation * sign;
				user_cmd->pb.mutable_base( )->set_leftmove( sign );
			}
			else {
				movement_angle.y = vel_ang - retrack_speed;
				user_cmd->pb.mutable_base( )->set_leftmove( 1.f );
			}
		}
		else {
			movement_angle.y = vel_ang + retrack_speed;
			user_cmd->pb.mutable_base( )->set_leftmove( -1.f );
		}
	}

	const float rotation = rad2deg( user_cmd->pb.mutable_base( )->viewangles( ).y( ) - movement_angle.y );

	const float new_forward_move = std::cos( rotation ) * user_cmd->pb.mutable_base( )->forwardmove( ) - std::sin( rotation ) * user_cmd->pb.mutable_base( )->leftmove( );
	const float new_side_move = std::sin( rotation ) * user_cmd->pb.mutable_base( )->forwardmove( ) + std::cos( rotation ) * user_cmd->pb.mutable_base( )->leftmove( );

	user_cmd->pb.mutable_base( )->set_leftmove( std::clamp( new_side_move * -1.f, -1.f, 1.f ) );
	user_cmd->pb.mutable_base( )->set_forwardmove( std::clamp( new_forward_move, -1.f, 1.f ) );
}

void c_movement::limit_speed( c_user_cmd* user_cmd, c_cs_player_pawn* local_player, c_base_player_weapon* active_weapon, float max_speed ) {
	c_player_movement_service* movement_services = local_player->m_movement_services( );

	if ( !movement_services )
		return;

	vec3_t velocity = g_ctx->m_local_pawn->m_vec_abs_velocity( );

	float cmd_speed = std::sqrt( 
		( user_cmd->pb.mutable_base( )->leftmove( ) * user_cmd->pb.mutable_base( )->leftmove( ) ) 
		+ ( user_cmd->pb.mutable_base( )->forwardmove( ) * user_cmd->pb.mutable_base( )->forwardmove( ) ) 
	);

	float speed_2d = velocity.length_2d( );

	if ( cmd_speed <= 50.f
		&& speed_2d <= 50.f )
		return;

	float accelerate = g_interfaces->m_var->get_by_name( xorstr_( "sv_accelerate" ) )->get_float( );

	vec3_t view_angles = {
		g_ctx->m_user_cmd->pb.mutable_base( )->viewangles( ).x( ),
		g_ctx->m_user_cmd->pb.mutable_base( )->viewangles( ).y( ),
		g_ctx->m_user_cmd->pb.mutable_base( )->viewangles( ).z( )
	};

	vec3_t forward{ }, right{ }, up{ };
	g_math->angle_vectors( view_angles, forward, right, up );

	float diff = speed_2d - max_speed;
	float wish_speed = max_speed;

	vec3_t direction = { forward.x * user_cmd->pb.mutable_base( )->forwardmove( ) + right.x * user_cmd->pb.mutable_base( )->leftmove( ),
		forward.y * user_cmd->pb.mutable_base( )->forwardmove( ) + right.y * user_cmd->pb.mutable_base( )->leftmove( ), 0.f };

	const float max_accelerate = accelerate * INTERVAL_PER_TICK * max( 250.f, movement_services->m_max_speed( ) * movement_services->m_surface_friction( ) );

	if ( diff - max_accelerate <= 0.f
		|| speed_2d - max_accelerate - 3.f <= 0.f )
		wish_speed = max_speed;
	else {
		direction = velocity;
		wish_speed = -1.f;
	}

	if ( user_cmd->pb.mutable_base( )->forwardmove( ) > 0 )
		user_cmd->pb.mutable_base( )->set_forwardmove( wish_speed );
	else if ( user_cmd->pb.mutable_base( )->forwardmove( ) < 0 )
		user_cmd->pb.mutable_base( )->set_forwardmove( -wish_speed );
	
	if ( user_cmd->pb.mutable_base( )->leftmove( ) > 0 )
		user_cmd->pb.mutable_base( )->set_leftmove( wish_speed );
	else if ( user_cmd->pb.mutable_base( )->leftmove( ) < 0 )
		user_cmd->pb.mutable_base( )->set_leftmove( -wish_speed );
}

void c_movement::auto_stop( c_user_cmd* user_cmd, c_cs_player_pawn* local_player, c_base_player_weapon* active_weapon, bool no_spread ) {
	if ( !g_cfg->rage_bot.m_auto_stop )
		return;

	if ( no_spread )
		return;

	if ( !( local_player->m_flags( ) & FL_ONGROUND ) )
		return;

	auto remove_button = [ & ]( int button ) {
		user_cmd->m_button_state.m_button_state &= ~button;
		user_cmd->m_button_state.m_button_state2 &= ~button;
		user_cmd->m_button_state.m_button_state3 &= ~button;
	};

	remove_button( IN_SPEED );

	float wish_speed = active_weapon->get_max_speed( ) * 0.25f;

	limit_speed( user_cmd, local_player, active_weapon, wish_speed );
}

void c_movement::movement_fix( c_user_cmd* user_cmd, vec3_t angle )
{
	vec3_t wish_angle{ user_cmd->pb.mutable_base( )->viewangles( ).x( ), user_cmd->pb.mutable_base( )->viewangles( ).y( ), user_cmd->pb.mutable_base( )->viewangles( ).z( ) };
	int sign = wish_angle.x > 89.f ? -1.f : 1.f;
	wish_angle.clamp( );

	vec3_t forward, right, up, old_forward, old_right, old_up;
	vec3_t view_angles = angle;

	g_math->angle_vectors( wish_angle, forward, right, up );

	forward.z = right.z = up.x = up.y = 0.f;

	forward.normalize_in_place( );
	right.normalize_in_place( );
	up.normalize_in_place( );

	g_math->angle_vectors( view_angles, old_forward, old_right, old_up );

	old_forward.z = old_right.z = old_up.x = old_up.y = 0.f;

	old_forward.normalize_in_place( );
	old_right.normalize_in_place( );
	old_up.normalize_in_place( );

	forward *= user_cmd->pb.mutable_base( )->forwardmove( );
	right *= user_cmd->pb.mutable_base( )->leftmove( );
	up *= user_cmd->pb.mutable_base( )->upmove( );

	float fixed_forward_move = old_forward.dot( right ) + old_forward.dot( forward ) + old_forward.dot( up, true );

	float fixed_side_move = old_right.dot( right ) + old_right.dot( forward ) + old_right.dot( up, true );

	float fixed_up_move = old_up.dot( right, true ) + old_up.dot( forward, true ) + old_up.dot( up );

	user_cmd->pb.mutable_base( )->set_forwardmove( fixed_forward_move );
	user_cmd->pb.mutable_base( )->set_leftmove( fixed_side_move );
	user_cmd->pb.mutable_base( )->set_upmove( fixed_up_move );

	fixed_forward_move = sign * ( old_forward.dot( right ) + old_forward.dot( forward ) );
	fixed_side_move = old_right.dot( right ) + old_right.dot( forward );

	user_cmd->pb.mutable_base( )->set_forwardmove( std::clamp( fixed_forward_move, -1.f, 1.f ) );
	user_cmd->pb.mutable_base( )->set_leftmove( std::clamp( fixed_side_move, -1.f, 1.f ) );
}

void c_movement::on_create_move( c_user_cmd* user_cmd, float old_yaw ) {
	if ( g_ctx->m_local_pawn->m_move_type( ) == movetype_ladder || g_ctx->m_local_pawn->m_move_type( ) == movetype_noclip )
		return;

	bunnyhop( user_cmd );
	auto_strafe( user_cmd, old_yaw );
}