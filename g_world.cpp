//g_world.cpp
// World Objects (trees, walls, floor tiles, etc.)

#include "g_world.h"

g_world::g_world()
{
	tex_on = true;
	music_on = false;
	lights_on = true;
}

int g_world::get_Level()
{
	return world_level;
}

void g_world::init_Block ( int level )
{
	int i, j;
	world_level = level;
	switch( level )
	{

	case 0:

		for( i = 0; i < max_block; i++ )
		{
			for( j = 0; j < max_block; j++ )
			{
				block[i][j] = Level_Zero[i][j];
			}
		}
		break;

	case 1:

		for( i = 0; i < max_block; i++ )
		{
			for( j = 0; j < max_block; j++ )
			{
				block[i][j] = Level_Zero[i][j];
			}
		}
		break;

	case 2:

		for( i = 0; i < max_block; i++ )
		{
			for( j = 0; j < max_block; j++ )
			{
				block[i][j] = Level_Zero[i][j];
			}
		}
		break;

	}; // end switch

}

void g_world::make_Clouds( GLfloat cloud_move[2], TEXTURE *cloud_plane )
{
		glEnable(GL_TEXTURE_2D);
		//Create the cloud quad list:
		float cloudpoints[16][3] = 
		{
			{ 20.0f,  0.19f,  20.0f}, //0
			{-20.0f,  0.19f,  20.0f}, //1
			{-20.0f,  0.19f, -20.0f}, //3
			{ 20.0f,  0.19f, -20.0f}, //2
			{ 20.0f,  0.29f,  20.0f}, //0
			{-20.0f,  0.29f,  20.0f}, //1
			{-20.0f,  0.29f, -20.0f}, //3
			{ 20.0f,  0.29f, -20.0f}, //2
			{ 20.0f,  0.39f,  20.0f}, //0
			{-20.0f,  0.39f,  20.0f}, //1
			{-20.0f,  0.39f, -20.0f}, //3
			{ 20.0f,  0.39f, -20.0f}, //2
			{ 20.0f,  0.49f,  20.0f}, //0
			{-20.0f,  0.49f,  20.0f}, //1
			{-20.0f,  0.49f, -20.0f}, //3
			{ 20.0f,  0.49f, -20.0f}, //2
		};

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);			// alpha blend clouds
		glColor3f(1.0f, 1.0f, 1.0f);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, cloudpoints);

		glBindTexture(GL_TEXTURE_2D, cloud_plane->ID);
		glTranslatef(0.0, 0.1, 0.0);
		glBegin(GL_QUADS);
			// Cloud Face 1
			glNormal3f( 0.0, -1.0, 0.0);
			glColor3f(0.6, 0.6, 0.6 );
			glTexCoord2f( cloud_move[1],	   (cloud_move[1]-1.0f));	glArrayElement(4);
			glTexCoord2f( cloud_move[1],		cloud_move[1]);			glArrayElement(5);
			glTexCoord2f((cloud_move[1]-1.0f),  cloud_move[1]);			glArrayElement(6);
			glTexCoord2f((cloud_move[1]-1.0f), (cloud_move[1]-1.0f));	glArrayElement(7);
			// Cloud Face 2
			glNormal3f( 0.0, -1.0, 0.0);
			glColor3f(0.4, 0.4, 0.4 );
			glTexCoord2f( cloud_move[0],	   (cloud_move[0]-1.0f));	glArrayElement(0);
			glTexCoord2f( cloud_move[0],		cloud_move[0]);			glArrayElement(1);
			glTexCoord2f((cloud_move[0]-1.0f),	cloud_move[0]);			glArrayElement(2);
			glTexCoord2f((cloud_move[0]-1.0f), (cloud_move[0]-1.0f));	glArrayElement(3);
		glEnd();
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
}

// Render a colored cube.
void g_world::make_Cube( int color, int texture_ID, bool textures_on )
{
//	glEnable(GL_CULL_FACE);
   	GLfloat mat_ambient[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	if( color == 585 ){
		glTranslatef(0.0, -2.0, 0.0 );
		glScalef(1.0, 0.5, 1.0);
		color = 5;
	}
	else {
		glTranslatef(0.0,  4.5, 0.0 );
		glScalef(1.0, 2.0, 1.0);
	}
	if( texture_ID == 999 )
	{
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glDisable(GL_LIGHTING);
	}
	else
	{
		if( textures_on  ) {
			glEnable(GL_TEXTURE_2D);
		}
		glBindTexture(GL_TEXTURE_2D, texture_ID); 
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	glEnable(GL_DEPTH_TEST);
    glBegin(GL_QUADS);
	         
       float m = -ground_radius;
	   float p =  ground_radius;

	   switch(color){
	   case 1: //brown
		   glColor3f(0.5, 0.3, 0.0); break;
	   case 2: //yellow
           glColor3f(1.0, 1.0, 0.0); break;
	   case 3: //orange
		   glColor3f(0.5, 0.5, 0.0); break;
	   case 4: //blue
		   glColor3f(0.0, 0.0, 1.0); break;
	   case 5: //white
		   glColor3f(1.0, 1.0, 1.0); break;
	   case 6: //red
		   glColor4f(1.0, 0.0, 0.0, 0.3); break;
	   case 7: //magenta
		   glColor3f(1.0, 0.0, 1.0); break;
	   case 8: //cyan
		   glColor3f(0.0, 1.0, 1.0); break;
	   case 9: //purple
		   glColor3f(0.5, 0.0, 0.5); break;
	   }

       glNormal3f( -1.0, 0.0, 0.0);
	   glTexCoord3f( 0,  0, 0); glVertex3f( m, m, m );
	   glTexCoord3f( 1,  0, 0); glVertex3f( m, m, p );
	   glTexCoord3f( 1,  1, 0); glVertex3f( m, p, p );
	   glTexCoord3f( 0,  1, 0); glVertex3f( m, p, m );

	   glNormal3f(1.0, 0.0, 0.0);
	   glTexCoord3f( 0,  0, 0);glVertex3f( p, m, m );
	   glTexCoord3f( 1,  0, 0);glVertex3f( p, m, p );
	   glTexCoord3f( 1,  1, 0);glVertex3f( p, p, p );
	   glTexCoord3f( 0,  1, 0);glVertex3f( p, p, m );

	   glNormal3f(0.0, 1.0, 0.0);
	   glTexCoord3f( 0,  0, 0);glVertex3f( m, p, m );
	   glTexCoord3f( 1,  0, 0);glVertex3f( p, p, m );
	   glTexCoord3f( 1,  1, 0);glVertex3f( p, p, p );
	   glTexCoord3f( 0,  1, 0);glVertex3f( m, p, p );
 
	   glNormal3f(0.0, 0.0, 1.0);
	   glTexCoord3f( 0,  0, 0);glVertex3f( m, m, m );
	   glTexCoord3f( 1,  0, 0);glVertex3f( m, p, m );
	   glTexCoord3f( 1,  1, 0);glVertex3f( p, p, m );
	   glTexCoord3f( 0,  1, 0);glVertex3f( p, m, m );

	   glNormal3f(0.0, 0.0, -1.0);
	   glTexCoord3f( 0,  0, 0);glVertex3f( m, m, p);
	   glTexCoord3f( 1,  0, 0);glVertex3f( m, p, p);
	   glTexCoord3f( 1,  1, 0);glVertex3f( p, p, p);
	   glTexCoord3f( 0,  1, 0);glVertex3f( p, m, p);

    glEnd();
	glDisable(GL_BLEND);
//	glDisable(GL_CULL_FACE);
	//glEnable(GL_LIGHTING);
	glScalef(1.0, 1.0, 1.0);
}

// Render a floor quad.
void g_world::make_Floor( int texture_ID, bool textures_on )
{
	float m = -2.0;
	float p =  2.0;

	if( textures_on ) {
		glEnable(GL_TEXTURE_2D);
	}
	
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glBegin(GL_QUADS);
	{
		glNormal3f(0.0, 1.0, 0.0);
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord3f( 0,  0, 0);
		glVertex3f( m, m, m );
		glTexCoord3f( 1,  0, 0);
		glVertex3f( p, m, m );
		glTexCoord3f( 1,  1, 0);
		glVertex3f( p, m, p );
		glTexCoord3f( 0,  1, 0);
		glVertex3f( m, m, p );
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

// Render ammo box.
void g_world::make_Fuel( int detail, bool textures_on )
{
  static GLfloat s;					// Slice angle.
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glDisable(GL_LIGHTING);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glScalef(3.5, 2.5, 3.5);
  glTranslatef(0.0, -1.5, 0.0);
  glBegin(GL_QUADS);
  glColor4f(0.0, 0.0, 1.0, 0.4);
  for (s = 0; s < 2*PI; s += 2*PI/detail) {
    glVertex3f(cos(s), 0, sin(s));
    glVertex3f(cos(s + 2*PI/detail), 0, sin(s + 2*PI/detail));
    glVertex3f(cos(s + 2*PI/detail), 1, sin(s + 2*PI/detail));
    glVertex3f(cos(s), 1, sin(s));
  }
  glEnd();
  glDisable(GL_BLEND);
  if( textures_on  ) {
    glEnable(GL_TEXTURE_2D);
  }
  glEnable(GL_LIGHTING);
}

void g_world::make_Gun( void )
{
    GLUquadricObj *D;
	D = gluNewQuadric();
	gluCylinder( D, 0.1, 0.1, 15.0, 20.0, 2.0);
	glDisable(GL_TEXTURE_2D);
	gluDeleteQuadric(D);
}

void g_world::make_Life( float life_radius, bool textures_on )
{
  static GLfloat vdata[8][3] =
  {
    {life_radius, 0, life_radius},	                /* 0 */
    {life_radius, 0, -life_radius},	      	        /* 1 */
    {-life_radius, 0, life_radius},      		/* 2 */
    {-life_radius, 0, -life_radius},     		/* 3 */
    {life_radius, life_radius*2, life_radius},	        /* 4 */
    {life_radius, life_radius*2, -life_radius},	/* 5 */
    {-life_radius, life_radius*2, life_radius},	/* 6 */
    {-life_radius, life_radius*2, -life_radius},	/* 7 */
  };
  
  /* draw box */
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glTranslatef(0.0, 0.0, 13.0);
  glBegin(GL_QUADS);
  glColor4f(1.0, 0.0, 0.0, .5);
  /* front */
  glTexCoord2f(1, 1);	glVertex3fv(vdata[2]);
  glTexCoord2f(0, 1);	glVertex3fv(vdata[0]);
  glTexCoord2f(0, 0);	glVertex3fv(vdata[4]);
  glTexCoord2f(1, 0);	glVertex3fv(vdata[6]);
  /* left */
  glTexCoord2f(0, 1);	glVertex3fv(vdata[3]);
  glTexCoord2f(1, 1);	glVertex3fv(vdata[2]);
  glTexCoord2f(1, 0);	glVertex3fv(vdata[6]);
  glTexCoord2f(0, 0);	glVertex3fv(vdata[7]);
  /* back */
  glTexCoord2f(1, 1);	glVertex3fv(vdata[1]);
  glTexCoord2f(0, 1);	glVertex3fv(vdata[3]);
  glTexCoord2f(0, 0);	glVertex3fv(vdata[7]);
  glTexCoord2f(1, 0);	glVertex3fv(vdata[5]);
  /* right */
  glTexCoord2f(0, 1);	glVertex3fv(vdata[0]);
  glTexCoord2f(1, 1);	glVertex3fv(vdata[1]);
  glTexCoord2f(1, 0);	glVertex3fv(vdata[5]);
  glTexCoord2f(0, 0);	glVertex3fv(vdata[4]);
  /* top */
  glTexCoord2f(0, 0);	glVertex3fv(vdata[6]);
  glTexCoord2f(0, 1);	glVertex3fv(vdata[4]);
  glTexCoord2f(1, 1);	glVertex3fv(vdata[5]);
  glTexCoord2f(1, 0);	glVertex3fv(vdata[7]);
  glEnd();
  glDisable(GL_BLEND);
  if( textures_on ) {
     glEnable(GL_TEXTURE_2D);
  }
}

void g_world::make_Pyramid( void )
{
	glBegin(GL_TRIANGLES);								// Start Drawing A Triangle
		glColor3f(1.0f,0.0f,0.0f);						// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Front)
		glColor3f(0.0f,1.0f,0.0f);						// Grn
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Left Of Triangle (Front)
		glColor3f(0.0f,0.0f,1.0f);						// Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Right Of Triangle (Front)
		glColor3f(1.0f,0.0f,0.0f);						// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Right)
		glColor3f(0.0f,0.0f,1.0f);						// Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Left Of Triangle (Right)
		glColor3f(0.0f,1.0f,0.0f);						// Grn
		glVertex3f( 1.0f,-1.0f, -1.0f);					// Right Of Triangle (Right)
		glColor3f(1.0f,0.0f,0.0f);						// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Back)
		glColor3f(0.0f,1.0f,0.0f);						// Grn
		glVertex3f( 1.0f,-1.0f, -1.0f);					// Left Of Triangle (Back)
		glColor3f(0.0f,0.0f,1.0f);						// Blue
		glVertex3f(-1.0f,-1.0f, -1.0f);					// Right Of Triangle (Back)
		glColor3f(1.0f,0.0f,0.0f);						// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Left)
		glColor3f(0.0f,0.0f,1.0f);						// Blue
		glVertex3f(-1.0f,-1.0f,-1.0f);					// Left Of Triangle (Left)
		glColor3f(0.0f,1.0f,0.0f);						// Grn
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Right Of Triangle (Left)
	glEnd();											// Done Drawing The Pyramid
}

void g_world::make_SkyBox( TEXTURE *skybox )
{
	TEXTURE *skyboxptr = skybox;
	glEnable(GL_TEXTURE_2D);
	
	//Create the sky cube list:
	float skypoints[8][3] = 
	{
		{-1.0f, -1.0f, -1.0f}, //0
		{ 1.0f, -1.0f, -1.0f}, //1
		{ 1.0f,  1.0f, -1.0f}, //2
		{-1.0f,  1.0f, -1.0f}, //3
		{-1.0f, -1.0f,  1.0f}, //4
		{ 1.0f, -1.0f,  1.0f}, //5
		{ 1.0f,  1.0f,  1.0f}, //6
		{-1.0f,  1.0f,  1.0f}, //7
	};	//The sky cube is created it this way to avoid gaps between the sky sides

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glDisable(GL_FOG);
	glColor3f(1.0f, 1.0f, 1.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, skypoints);

	glBindTexture(GL_TEXTURE_2D, skyboxptr->ID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBegin(GL_QUADS);		
		//Right face
		glNormal3f( -1.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 0.0f); glArrayElement(1);
		glTexCoord2f(1.0f, 1.0f); glArrayElement(2);
		glTexCoord2f(0.0f, 1.0f); glArrayElement(6);
		glTexCoord2f(0.0f, 0.0f); glArrayElement(5);
	glEnd();

	skyboxptr++;

	glBindTexture(GL_TEXTURE_2D, skyboxptr->ID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBegin(GL_QUADS);
		//Left face
		glNormal3f( 0.0, 0.0, 1.0);
		glTexCoord2f(0.0f, 0.0f); glArrayElement(0);
		glTexCoord2f(1.0f, 0.0f); glArrayElement(4);
		glTexCoord2f(1.0f, 1.0f); glArrayElement(7);
		glTexCoord2f(0.0f, 1.0f); glArrayElement(3);
	glEnd();
	
	skyboxptr++;

	glBindTexture(GL_TEXTURE_2D, skyboxptr->ID );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBegin(GL_QUADS);
		//Floor face
		glNormal3f( 0.0, 1.0, 0.0);
		glTexCoord2f(0.0f, 0.0f); glArrayElement(4);
		glTexCoord2f(1.0f, 0.0f); glArrayElement(5);
		glTexCoord2f(1.0f, 1.0f); glArrayElement(6);
		glTexCoord2f(0.0f, 1.0f); glArrayElement(7);
	glEnd();

	skyboxptr++;

	glBindTexture(GL_TEXTURE_2D, skyboxptr->ID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
		//Sky face
		glNormal3f( 0.0, -1.0, 0.0);
		glTexCoord2f(1.0f, 0.0f); glArrayElement(0);
		glTexCoord2f(1.0f, 1.0f); glArrayElement(3);
		glTexCoord2f(0.0f, 1.0f); glArrayElement(2);
		glTexCoord2f(0.0f, 0.0f); glArrayElement(1);
	glEnd();

	skyboxptr++;

	glBindTexture(GL_TEXTURE_2D, skyboxptr->ID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);			
		//Back face
		glNormal3f( 0.0, 0.0, -1.0);
		glTexCoord2f(1.0f, 0.0f); glArrayElement(3);
		glTexCoord2f(1.0f, 1.0f); glArrayElement(7);
		glTexCoord2f(0.0f, 1.0f); glArrayElement(6);
		glTexCoord2f(0.0f, 0.0f); glArrayElement(2);
	glEnd();

	skyboxptr++;

	glBindTexture(GL_TEXTURE_2D, skyboxptr->ID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);		
		//Front face
		glNormal3f( 1.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f); glArrayElement(0);
		glTexCoord2f(0.0f, 1.0f); glArrayElement(1);
		glTexCoord2f(0.0f, 0.0f); glArrayElement(5);
		glTexCoord2f(1.0f, 0.0f); glArrayElement(4);
	glEnd();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}
// Render a tree.
void g_world::make_Tree( int texture_ID, bool textures_on )
{
	glTranslatef( 0.0, -2.0, 0.0 );
	if( textures_on )
		glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_ID); 
    GLUquadricObj *D;
	D = gluNewQuadric();
	gluQuadricTexture(D, GL_TRUE);
	glScalef(8.0, 4.0, 8.0);
    glRotatef( -90.0, 1.0, 0.0, 0.0);
	glColor3f( 0.0, 0.6, 0.0 );
	gluCylinder( D, 0.5, 0.0, 4.0, 30.0, 2.0);
	glTranslatef( 0.0, 0.0, -0.5 );
	glColor3f( 0.5, 0.3, 0.0 );
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	gluCylinder( D, 0.12, 0.12, 0.50, 30.0, 1.0);
	gluDeleteQuadric(D);
}


// Render a floor quad.
void g_world::make_Land( int texture_ID, bool textures_on, float normal )
{
	float m = -ground_radius;
	float p =  ground_radius;
	if( textures_on ) {
		glEnable(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, texture_ID );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBegin(GL_QUADS);
	{
		glNormal3f(0.0, normal, 0.0);
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord3f( 0,  0, 0);
		glVertex3f( -4, -4, -4 );
		glTexCoord3f( 100,  0, 0);
		glVertex3f( 480, -4, -4 );
		glTexCoord3f( 100,  100, 0);
		glVertex3f( 480, -4, 480 );
		glTexCoord3f( 0,  100, 0);
		glVertex3f( -4, -4, 480 );
	}
	glEnd();
}

// Render a floor quad.
void g_world::make_Quad( int texture_ID, bool textures_on, float normal )
{
	float m = -ground_radius;
	float p =  ground_radius;
	if( textures_on ) {
		glEnable(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, texture_ID );
	glBegin(GL_QUADS);
	{
		glNormal3f(0.0, normal, 0.0);
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord3f( 0,  0, 0);
		glVertex3f( -4, -4, -4 );
		glTexCoord3f( 1.0,  0, 0);
		glVertex3f( 4, -4, -4 );
		glTexCoord3f( 1.0,  1.0, 0);
		glVertex3f( 4, -4, 4 );
		glTexCoord3f( 0,  1.0, 0);
		glVertex3f( -4, -4, 4 );
	}
	glEnd();
}


g_fuel::g_fuel( void )
{
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
void g_fuel::SetPosition( int r, int c )
{
	row = r;
	col = c;
}

int g_fuel::GetCol( void )
{
	return col;
}

int g_fuel::GetRow( void )
{
	return row;
}

void g_fuel::SetColliderTransform( void )
{
	collider->setTransform(mv_matrix);
}