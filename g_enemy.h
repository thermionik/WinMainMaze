// g_enemy.h




#ifndef g_enemy_h
#define g_enemy_h

#include "g_world.h"
#include "g_audio.h"
#include "g_framerate.h"

class g_enemy 
{

public:
	g_enemy();
	void Die();
	void Hurt( int damage );
	void Respawn( g_world *World );
	void Update_AI( g_world *World, CollisionModel3D *model_player, g_framerate *FrameRate ); //, g_player *Player );
	void Rotate();
	void Translate();
	bool Is_Shooting();
	bool IsDead();
	 int Get_Row();
	 int Get_Col();
	float Get_xpos();
	float Get_zpos();
	float mv_matrix[16];
	void SetColliderTransform();
	CollisionModel3D *collider;

private:
	bool kill;
	bool shooting;
	bool moving_forward;
	bool moving_backward;
	bool strafe_left;
	bool strafe_right;

	int life, time, dead_time;
		float	z;	
	float	heading ;
	float	xpos;
	float	zpos;
	float	yrot;				
	float   lastx, 
		    lastz, 
			pushx, 
			pushz;

	float MONSTER_SPEED;	 
	float MONSTER_CLIP;	
	int row, col; 
};

#endif g_enemy_h


