// g_player.h


#ifndef g_player_h
#define g_player_h

#include "g_world.h"
#include "g_audio.h"
#include "g_enemy.h"

class g_player
{

public:
	void Update_Input( g_world *World, CollisionModel3D *fuel_can, 
					g_enemy *Enemy , g_fuel *Fuel , g_framerate *FrameRate);
	void Update_Mouse();
	void Rotate();
	void Translate();
	 int Get_Kills();
	bool Has_Ammo();
	bool Is_Shooting();
	void Shot();
	void Fragged_Em();
 	 int Get_Ammo();
	 int Get_Row();
   float Get_xpos();
   float Get_zpos();
	 int Get_Col();
	void Reload();
	g_player( void );
  // ~g_player( void );


private:
		bool	killed;
	    int		hits;
		float	walkbias;
		float	walkbiasangle;
		float	lookupdown;
		float	z;	
		float	heading;
		float	xpos;
		float	zpos;
		float	yrot;				// Y Rotation
		float   lastx, 
			    lastz, 
				pushx, 
				pushz;
		int		kills;
		    int		fuel;		bool	flame;
		bool	tp, lp, left_click, right_click, mp, ku,	// Keys/Buttons are up/down? 
				kd, kl, kr, wp, space_bar, ml;

		bool	bullets;		


		POINT	mpos;				// Mouse Position	( Add )
		float PLAYER_SPEED;	 
		float PLAYER_CLIP;		
		int row, col;
};

#endif g_player_h