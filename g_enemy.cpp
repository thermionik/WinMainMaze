// g_enemy.cpp

#include "g_util.h"
#include "g_enemy.h"

g_enemy::g_enemy()
{
	xpos = -18.0;
	zpos = -10.0;
	//hit = 0;
	life = 50;
	kill = false;
	MONSTER_SPEED = 0.7;
	MONSTER_CLIP  = 8.1;
	moving_forward=true;
	strafe_left=true;
		heading = 0;

	collider = newCollisionModel3D();
	if(collider == NULL )
		exit(1);

	collider->addTriangle( -2.0f, -2.0f, -2.0f, -2.0f, -2.0f,  2.0f, -2.0f,  2.0f,  2.0f );
	collider->addTriangle( -2.0f, -2.0f,  2.0f, -2.0f,  2.0f,  2.0f, -2.0f,  2.0f, -2.0f );

	collider->addTriangle(  2.0f, -2.0f, -2.0f,  2.0f, -2.0f,  2.0f,  2.0f,  2.0f,  2.0f );
	collider->addTriangle(  2.0f, -2.0f,  2.0f,  2.0f,  2.0f,  2.0f,  2.0f,  2.0f, -2.0f );

	collider->addTriangle( -2.0f, -2.0f, -2.0f,  2.0f, -2.0f, -2.0f,  2.0f, -2.0f,  2.0f );
	collider->addTriangle(  2.0f, -2.0f, -2.0f,  2.0f, -2.0f,  2.0f, -2.0f, -2.0f,  2.0f );

	collider->addTriangle( -2.0f,  2.0f, -2.0f,  2.0f,  2.0f, -2.0f,  2.0f,  2.0f,  2.0f );
	collider->addTriangle(  2.0f,  2.0f, -2.0f,  2.0f,  2.0f,  2.0f, -2.0f,  2.0f,  2.0f );

	collider->addTriangle( -2.0f, -2.0f, -2.0f, -2.0f,  2.0f, -2.0f,  2.0f,  2.0f, -2.0f );
	collider->addTriangle( -2.0f,  2.0f, -2.0f,  2.0f,  2.0f, -2.0f,  2.0f, -2.0f, -2.0f );

	collider->addTriangle( -2.0f, -2.0f,  2.0f, -2.0f,  2.0f,  2.0f,  2.0f,  2.0f,  2.0f );
	collider->addTriangle( -2.0f,  2.0f,  2.0f,  2.0f,  2.0f,  2.0f,  2.0f, -2.0f,  2.0f );

	collider->finalize ();


}

void g_enemy::SetColliderTransform( void )
{
	collider->setTransform(mv_matrix);
}

void g_enemy::Die( void )
{
	kill = true;
	moving_forward = false;
}

bool g_enemy::IsDead( void )
{
	return kill;
}


void g_enemy::Hurt( int damage )
{
	life -= damage;
	if ( life <= 0 )
	{
		Die();
	}
}

void g_enemy::Respawn( g_world *World )
{
	kill = false;
	life = 50;
	moving_forward = true;

	static GLfloat random = 0;
	static GLint target_x = 0;
	static GLint target_z = 0;

	random = ((float)rand()/RAND_MAX);
	target_x = 80*random;
	random = ((float)rand()/RAND_MAX);
	target_z = 80*random;
	if (World->block[target_z][target_x] == 0)
	{
		xpos = -target_x;
		zpos = -target_z;
	}
	else
	{
		Respawn( World );
	}

}

void g_enemy::Update_AI( g_world *World, CollisionModel3D *model_player, 
						g_framerate *FrameRate ) //, g_player *Player )
{
	float monster_speed = MONSTER_SPEED*FrameRate->speedfactor;

	if (moving_forward)
	{

		int pushcol, pushrow;
		lastx = xpos;
		lastz = zpos;
		xpos -= (sin(heading*piover180) * MONSTER_CLIP);
		zpos -= (cos(heading*piover180) * MONSTER_CLIP);

		/* perform collision detection */
		
		row = (-lastx + ground_radius)/(2*ground_radius);
		col = (-zpos  + ground_radius)/(2*ground_radius);
		int zu = World->block[col][row];	
		if (((zu == 0) || (zu == 66) || (zu == 6) || (zu == 4) || (zu == 77))//|| (zu == 13) || (zu == 12) || (zu == 11)
					   && (col < max_block) && (row < max_block) && (col >= 0)
					   && (row >= 0) && (-zpos >= 0)) 
		{
			pushz = zpos;
			pushz -= (cos(heading*piover180) * 8.1);
			zpos = lastz;
			zpos -= (cos(heading*piover180) * monster_speed);
			pushcol = (-pushz + ground_radius)/(2*ground_radius);
			if(zu == 4)
			{
				if(World->block[pushcol][row] == 0)
				{
					//play push sound
					World->block[col][row] = 0;
					World->block[pushcol][row] = 4;
				}
				else
				{
					zpos = lastz;
					col = (-zpos + ground_radius)/(2*ground_radius);
					int k = 1+(int) (200*rand()/(RAND_MAX+1.0));
					if ( k % 2 )
						heading+=90;
					else
						heading-=90;
				}
			}
		}
		else
		{
			zpos = lastz;	
			col = (-zpos + ground_radius)/(2*ground_radius);
			int k = 1+(int) (200*rand()/(RAND_MAX+1.0));
			if ( k % 2 )
				heading+=90;
			else
				heading-=90;
		}
		row = (-xpos + ground_radius)/(2*ground_radius);
		col = (-lastz + ground_radius)/(2*ground_radius);
		zu = World->block[col][row];	
		if (((zu == 0) || (zu == 66) || (zu == 6) || (zu == 4) || (zu == 77)) //|| (zu == 13) || (zu == 12) || (zu == 11)
					   && (col < max_block) && (row < max_block) && (col >= 0)
					   && (row >= 0) && (-xpos >= 0)) 
		{ 
			pushx = xpos;
			pushx -= (sin(heading*piover180) * 8.1f);
			xpos = lastx;
			xpos -= (sin(heading*piover180) * monster_speed);
			pushrow = (-pushx + ground_radius)/(2*ground_radius);
			if(zu == 4)
			{
				if(World->block[col][pushrow] == 0)
				{
					//play push sound
					World->block[col][row] = 0;
					World->block[col][pushrow] = 4;
				}
				else
				{
					xpos = lastx;
					row = (-xpos + ground_radius)/(2*ground_radius);
					int k = 1+(int) (200*rand()/(RAND_MAX+1.0));
					if ( k % 2 )
						heading+=90;
					else
						heading-=90;
				}
			}
		}
		else
		{
			xpos = lastx;
			row = (-xpos + ground_radius)/(2*ground_radius);
			int k = 1+(int) (200*rand()/(RAND_MAX+1.0));
			if ( k % 2 )
				heading+=90;
			else
				heading-=90;
		}
	}

	/*****************************************/

	if (moving_backward)
	{
		float lastx, lastz;
		lastx = xpos;
		lastz = zpos;
		xpos += (float)sin(heading*piover180) * MONSTER_CLIP;
		zpos += (float)cos(heading*piover180) * MONSTER_CLIP;
		
		// perform collision detection 
		row = (lastx + ground_radius)/(2*ground_radius);
		col = (zpos + ground_radius)/(2*ground_radius);
		int zu = World->block[col][row];	
		if (((zu == 0) || (zu == 66) || (zu == 6) || (zu == 20)) //|| (zu == 13) || (zu == 12) || (zu == 11)
					   && (col < max_block) && (row < max_block) && (col >= 0)
					   && (row >= 0) && (zpos >= 0)) { 						zpos = lastz;
			zpos += (float)cos(heading*piover180) * monster_speed;		}
		else
		{
			zpos = lastz;	
			col = (zpos + ground_radius)/(2*ground_radius);
			heading++;
		}
		row = (xpos + ground_radius)/(2*ground_radius);
		col = (lastz + ground_radius)/(2*ground_radius);
		zu = World->block[col][row];	
		if (((zu == 0) || (zu == 66) || (zu == 6) || (zu == 20)) //|| (zu == 13) || (zu == 12) || (zu == 11)
					   && (col < max_block) && (row < max_block) && (col >= 0)
					   && (row >= 0) && (xpos >= 0)) { 						xpos = lastx;
			xpos += (float)sin(heading*piover180) * monster_speed;	}
		else
		{
			xpos = lastx;
			row = (xpos + ground_radius)/(2*ground_radius);
			heading++;
		}
	}
	if (strafe_left)
	{
		float lastx, lastz;
		lastx = xpos;
		lastz = zpos;
		xpos += (float)sin((heading - 90)*piover180) * MONSTER_CLIP;
		zpos += (float)cos((heading - 90)*piover180) * MONSTER_CLIP;
		
		// perform collision detection 
		row = (lastx + ground_radius)/(2*ground_radius);
		col = (zpos + ground_radius)/(2*ground_radius);
		int zu = World->block[col][row];	
		if (((zu == 0) || (zu == 66) || (zu == 6) || (zu == 20)) //|| (zu == 13) || (zu == 12) || (zu == 11)
					   && (col < max_block) && (row < max_block) && (col >= 0)
					   && (row >= 0) && (zpos >= 0)) { 						zpos = lastz;
			zpos += (float)cos((heading - 90)*piover180) * monster_speed;		}
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
			xpos += (float)sin((heading - 90)*piover180) * monster_speed;	}
		else
		{
			xpos = lastx;
			row = (xpos + ground_radius)/(2*ground_radius);
		}

	}
	if (strafe_right)
	{
		float lastx, lastz;
		lastx = xpos;
		lastz = zpos;
		xpos += (float)sin((heading + 90)*piover180) * MONSTER_CLIP;	
		zpos += (float)cos((heading + 90)*piover180) * MONSTER_CLIP;	
		// perform collision detection 
		row = (lastx + ground_radius)/(2*ground_radius);
		col = (zpos + ground_radius)/(2*ground_radius);
		int zu = World->block[col][row];	
		if (((zu == 0) || (zu == 66) || (zu == 6) || (zu == 20)) //|| (zu == 13) || (zu == 12) || (zu == 11)
					   && (col < max_block) && (row < max_block) && (col >= 0)
					   && (row >= 0) && (zpos >= 0)) { 						zpos = lastz;
			zpos += (float)cos((heading + 90)*piover180) * monster_speed;		}
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
			xpos += (float)sin((heading + 90)*piover180) * monster_speed;	}
		else
		{
			xpos = lastx;
			row = (xpos + ground_radius)/(2*ground_radius);
		}
	} 
}

void g_enemy::Rotate()
{
	GLfloat sceneroty = heading - 90;
	glRotatef(sceneroty,0,1.0f,0);
}

void g_enemy::Translate()
{
	GLfloat xtrans = xpos;
	GLfloat ztrans = zpos;
	glTranslatef(-xtrans, 0.8, -ztrans);
}

bool g_enemy::Is_Shooting()
{
	return shooting;
}

int g_enemy::Get_Row()
{
	return row;
}

int g_enemy::Get_Col()
{
	return col;
}

float g_enemy::Get_xpos()
{
	return xpos;
}

float g_enemy::Get_zpos()
{
	return zpos;
}
