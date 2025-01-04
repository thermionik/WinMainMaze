#include "g_util.h"
//#include "g_world.h"
#include "g_player.h"


g_player::g_player ()
{
	kills = 0;
	bullets = true;
	flame = false;
	fuel = 5;
	xpos = 12;
	zpos = 12;
	walkbiasangle = 0;
	PLAYER_SPEED = 0.8f;
	PLAYER_CLIP	 = 3.0f;
}

void g_player::Rotate ()
{
	GLfloat sceneroty = 360.0f - yrot;
	glRotatef(lookupdown,1.0f,0,0);
	glRotatef(sceneroty,0,1.0f,0);
}

void g_player::Reload ()
{
	fuel += fuel_increment;
	bullets = true;
}

void g_player::Update_Mouse ()
{
	GetCursorPos(&mpos);				// Get The Current Mouse Position	( Add )
	SetCursorPos(320,240);				// Set The Mouse Position To The Center Of The Window	( Add )
	heading += (float)(320 - mpos.x)/100 * 15;         // Update The Direction For Movement		( Add )
	yrot = heading;                                   // Update The Y Rotation	( Add )
	lookupdown += (float)(240 - mpos.y)/100 * 15;      // Update The X Rotation	( Add )
}

int g_player::Get_Row()
{
	return row;
}

int g_player::Get_Col()
{
	return col;
}

int g_player::Get_Ammo ()
{
	return fuel;
}

int g_player::Get_Kills ()
{
	return kills;
}

void g_player::Translate()
{
	GLfloat xtrans = xpos;
	GLfloat ztrans = zpos;
	GLfloat ytrans = -walkbias - 1.0;
	//GLfloat ytrans = -4.5;
	glTranslatef(-xtrans, ytrans, -ztrans);
}

void g_player::Update_Input( g_world *World, CollisionModel3D *fuel_can, 
					g_enemy *Enemy , g_fuel *Fuel , g_framerate *FrameRate )
{

	float player_speed = PLAYER_SPEED*FrameRate->speedfactor;

	if(MOUSE_DOWN(MOUSE_LEFT) && !ml)
	{
		  ml=true;
  		  if( this->Get_Ammo() > 0) {
			 //Sound.Play( &shoot, 0 );
  			 flame = true;   fuel -= delta_fuel;

  		  }

	}
	if( !MOUSE_DOWN(MOUSE_LEFT) )
		ml=false;

	if (keys['E'] && !ku)
	{
		float lastx, lastz;
		int pushcol, pushrow;
		lastx = xpos;
		lastz = zpos;
		xpos -= (float)sin(heading*piover180) * PLAYER_CLIP;
		zpos -= (float)cos(heading*piover180) * PLAYER_CLIP;
		
		// perform collision detection 
		row = (lastx + ground_radius)/(2*ground_radius);
		col = (zpos  + ground_radius)/(2*ground_radius);
		int zu = World->block[col][row];	
		if (((zu == 0) || (zu == 66) || (zu == 6) || (zu == 4) || (zu == 77))//|| (zu == 13) || (zu == 12) || (zu == 11)
					   && (col < max_block) && (row < max_block) && (col >= 0)
					   && (row >= 0) && (zpos >= 0)) 
		{		

			pushz = zpos;
			pushz -= (cos(heading*piover180) * 8.1f);
			pushcol = (pushz + ground_radius) / (2 * ground_radius);
			zpos = lastz;
			zpos -= (float)cos(heading*piover180) * player_speed;		

			if (zu == 4)
			{
				if (World->block[pushcol][row] == 0)
				{
					//play push sound
					//	Sound.Play( &pushsound, 0 );
					World->block[col][row] = 0;
					World->block[pushcol][row] = 4;
				}
				else
				{
					zpos = lastz;
					col = (zpos + ground_radius) / (2 * ground_radius);
				}
			}
		}
		else
		{
			zpos = lastz;	
			col = (zpos + ground_radius)/(2*ground_radius);
		//	heading--;
		}
		row = (xpos + ground_radius)/(2*ground_radius);
		col = (lastz + ground_radius)/(2*ground_radius);
		zu = World->block[col][row];	
		if (((zu == 0) || (zu == 66) || (zu == 6) || (zu == 4) || (zu == 77)) //|| (zu == 13) || (zu == 12) || (zu == 11)
					   && (col < max_block) && (row < max_block) && (col >= 0)
					   && (row >= 0) && (xpos >= 0)) 
		{
			pushx = xpos;
			pushx -= (sin(heading*piover180) * 8.1f);
			pushrow = (pushx + ground_radius) / (2 * ground_radius);

			xpos = lastx;
			xpos -= (float)sin(heading*piover180) * player_speed;	

			if (zu == 4)
			{
				if (World->block[col][pushrow] == 0)
				{
					//play push sound
					//	Sound.Play( &pushsound, 0 );
					World->block[col][row] = 0;
					World->block[col][pushrow] = 4;
				}
				else
				{
					xpos = lastx;
					row = (xpos + ground_radius) / (2 * ground_radius);
				}
			}
		}
		else
		{
			xpos = lastx;
			row = (xpos + ground_radius)/(2*ground_radius);
		//	heading--;
		}
	}
	/*****************************************/

	if (keys['D'] && !kd)
	{
		float lastx, lastz;
		lastx = xpos;
		lastz = zpos;
		xpos += (float)sin(heading*piover180) * PLAYER_CLIP;
		zpos += (float)cos(heading*piover180) * PLAYER_CLIP;
		
		// perform collision detection 
		row = (lastx + ground_radius)/(2*ground_radius);
		col = (zpos + ground_radius)/(2*ground_radius);
		int zu = World->block[col][row];	
		if (((zu == 0) || (zu == 66) || (zu == 6) || (zu == 20)) //|| (zu == 13) || (zu == 12) || (zu == 11)
					   && (col < max_block) && (row < max_block) && (col >= 0)
					   && (row >= 0) && (zpos >= 0)) { 						zpos = lastz;
			zpos += (float)cos(heading*piover180) * player_speed;		}
		else
		{
			zpos = lastz;	
			col = (zpos + ground_radius)/(2*ground_radius);
		//	heading++;
		}
		row = (xpos + ground_radius)/(2*ground_radius);
		col = (lastz + ground_radius)/(2*ground_radius);
		zu = World->block[col][row];	
		if (((zu == 0) || (zu == 66) || (zu == 6) || (zu == 20)) //|| (zu == 13) || (zu == 12) || (zu == 11)
					   && (col < max_block) && (row < max_block) && (col >= 0)
					   && (row >= 0) && (xpos >= 0)) { 						xpos = lastx;
			xpos += (float)sin(heading*piover180) * player_speed;	}
		else
		{
			xpos = lastx;
			row = (xpos + ground_radius)/(2*ground_radius);
	//		heading++;
		}
	}
	if (keys['S'] && !kl)
	{
		float lastx, lastz;
		lastx = xpos;
		lastz = zpos;
		xpos += (float)sin((heading - 90)*piover180) * PLAYER_CLIP;
		zpos += (float)cos((heading - 90)*piover180) * PLAYER_CLIP;
		
		// perform collision detection 
		row = (lastx + ground_radius)/(2*ground_radius);
		col = (zpos + ground_radius)/(2*ground_radius);
		int zu = World->block[col][row];	
		if (((zu == 0) || (zu == 66) || (zu == 6) || (zu == 20)) //|| (zu == 13) || (zu == 12) || (zu == 11)
					   && (col < max_block) && (row < max_block) && (col >= 0)
					   && (row >= 0) && (zpos >= 0)) { 						zpos = lastz;
			zpos += (float)cos((heading - 90)*piover180) * player_speed;		}
		else
		{
			zpos = lastz;	
			col = (zpos + ground_radius)/(2*ground_radius);
		}
		row = (xpos + ground_radius)/(2*ground_radius);
		col = (lastz + ground_radius)/(2*ground_radius);
		zu = World->block[col][row];	
		if (((zu == 0) || (zu == 66) || (zu == 6) || (zu == 20)) //|| (zu == 13) || (zu == 12) || (zu == 11)
					   && (col < max_block) && (row < max_block) && (col >= 0)
					   && (row >= 0) && (xpos >= 0)) { 						xpos = lastx;
			xpos += (float)sin((heading - 90)*piover180) * player_speed;	}
		else
		{
			xpos = lastx;
			row = (xpos + ground_radius)/(2*ground_radius);
		}

	}
	if (keys['F'] && !kr)
	{
		float lastx, lastz;
		lastx = xpos;
		lastz = zpos;
		xpos += (float)sin((heading + 90)*piover180) * PLAYER_CLIP;	
		zpos += (float)cos((heading + 90)*piover180) * PLAYER_CLIP;	
		// perform collision detection 
		row = (lastx + ground_radius)/(2*ground_radius);
		col = (zpos + ground_radius)/(2*ground_radius);
		int zu = World->block[col][row];	
		if (((zu == 0) || (zu == 66) || (zu == 6) || (zu == 20)) //|| (zu == 13) || (zu == 12) || (zu == 11)
					   && (col < max_block) && (row < max_block) && (col >= 0)
					   && (row >= 0) && (zpos >= 0)) { 						zpos = lastz;
			zpos += (float)cos((heading + 90)*piover180) * player_speed;		}
		else
		{
			zpos = lastz;	
			col = (zpos + ground_radius)/(2*ground_radius);
		}
		row = (xpos + ground_radius)/(2*ground_radius);
		col = (lastz + ground_radius)/(2*ground_radius);
		zu = World->block[col][row];	
		if (((zu == 0) || (zu == 66) || (zu == 6) || (zu == 20)) //|| (zu == 13) || (zu == 12) || (zu == 11)
					   && (col < max_block) && (row < max_block) && (col >= 0)
					   && (row >= 0) && (xpos >= 0)) { 						xpos = lastx;
			xpos += (float)sin((heading + 90)*piover180) * player_speed;	}
		else
		{
			xpos = lastx;
			row = (xpos + ground_radius)/(2*ground_radius);
		}
	} 

}


bool g_player::Is_Shooting()
{
	return flame;
}

void g_player::Fragged_Em()
{
	kills++;
}

void g_player::Shot()
{
	flame = false;
}

float g_player::Get_xpos()
{
	return xpos;
}

float g_player::Get_zpos()
{
	return zpos;
}